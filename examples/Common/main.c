/* LVGL Example project
 *
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */

#include "main.h"
#include "gui_task.h"

/*********************
 *      DEFINES
 *********************/
#define TAG "main"

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void example_task1(void *pvParameters)
{
    static const char *ETAG  = "task1";
    uint32_t           count = 0;

    for (;;) {
        ++count;
        // ESP_LOGI(ETAG, "count = %d", count);
        // if ((count) == 10) {
        //     disp_spi_change_device_speed(20 * 1000 * 1000);
        // }
        // if ((count) == 15) {
        //     disp_spi_change_device_speed(16 * 1000 * 1000);
        // }
        ESP_LOGI(ETAG,
                 "free_heap_size = %d Byte,\t%.2f KByte,\t%.2f MByte",
                 esp_get_free_heap_size(),
                 esp_get_free_heap_size() / 1024.0,
                 esp_get_free_heap_size() / (1024.0 * 1024.0));
        vTaskDelay(pdMS_TO_TICKS(30 * 1000));
    }
    vTaskDelete(NULL);
}

/**********************
 *   APPLICATION MAIN
 **********************/
void app_main()
{
    xTaskCreate(example_task1,             /* Point to the task to be created */
                "example_task1",           /* Task text name, used for debugging */
                1024 * 8,                  /* Stack depth, in bytes */
                NULL,                      /* Parameters passed to the task */
                configMAX_PRIORITIES - 24, /* Task priority */
                NULL                       /* Handle to control the task, note the '&' */
    );
    gui_task_init();
}
