/**
 * @file gui_task.c
 * @author catcatBlue (1625237402@qq.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright Copyright (c) 2022
 */

/*set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "gui_task.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "disp_spi.h"
#include "esp_log.h"

/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif
#include "lvgl_helpers.h"

#include "lv_port/lv_port.h"
#include "lvgl/demos/lv_demos.h"

#include "ui/ui.h"

/*********************
 *      DEFINES
 *********************/
#define TAG               "demo"
#define LV_TICK_PERIOD_MS 1

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void gui_task_init(void)
{
    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
    /* TODO: !!! Prehistoric giant pit!!! The priority should be raised */
    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 10, NULL, 1);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;

static void guiTask(void *pvParameter)
{
   ESP_LOGI( TAG, "Start GUI Task" );
    (void)pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();

    // /* Initialize SPI or I2C bus used by the drivers */
    // lvgl_driver_init();

    /* Display initialization */
    ESP_LOGI( TAG, "Display initialization" );
    lv_port_disp_init();

    /* Initialize SPI or I2C bus used by the drivers */
    /*
     * Since setting the display resolution is no longer globally defined,
     * LVGL driver initialization must be performed after specifying the LVGL resolution,
     * Because it includes setting the maximum SPI transfer size, the screen resolution parameter is required here
     */

    ESP_LOGI( TAG, "LVGL Driver initialization" );
    lvgl_driver_init();

    /* Input device initialization */
    ESP_LOGI( TAG, "Input device initialization" );
    lv_port_indev_init();

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name     = "periodic_gui"};
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    /* Create the demo application */
    ESP_LOGI( TAG, "Create the demo application" );
    ui_init();
    while (1) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(1));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            ui_tick();
            xSemaphoreGive(xGuiSemaphore);
        }
    }

    /* A task should NEVER return */
    free(buf1);
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    free(buf2);
#endif
    vTaskDelete(NULL);
}

static void lv_tick_task(void *arg)
{
    (void)arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}


#else /*Enable this file at the top*/
/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
