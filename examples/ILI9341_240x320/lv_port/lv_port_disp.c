/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"

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

/*********************
 *      DEFINES
 *********************/
#define TAG "lv_port_disp"


#define MY_DISP_HOR_RES 320 /* 240 */
#define MY_DISP_VER_RES 240 /* 320 */

#define LINES_TO_DRAW               24

#define DISP_BUF_SIZE_CUSTOM (MY_DISP_HOR_RES * 16) /* 10240 */
//#define DISP_BUF_SIZE_CUSTOM (128000 /* 17000 */)


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);
// static void disp_flush(lv_display_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
// static void disp_clear(lv_display_t *disp_drv);

/**********************
 *  STATIC VARIABLES
 **********************/
DMA_ATTR lv_color_t *buf1 = NULL;
DMA_ATTR lv_color_t *buf2 = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();  // empty function, nothing impelemented

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/
    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

     ESP_LOGI(TAG, "MALLOC_CAP_SPIRAM SIZE: %d", heap_caps_get_total_size(MALLOC_CAP_DMA));
     ESP_LOGI(TAG, "DISP_BUF_SIZE_CUSTOM SIZE: %d", 2 * (DISP_BUF_SIZE_CUSTOM * ( ( LV_COLOR_DEPTH + 7 ) / 8 )));

   // allocate memory for display buffers
   uint32_t buf_size_bytes = DISP_BUF_SIZE_CUSTOM * ( ( LV_COLOR_DEPTH + 7 ) / 8 );
   ESP_LOGI( TAG, "Display buffer size: %d", buf_size_bytes );

#if USE_STATIC_DISPLAY_BUFFER
   // !!! this will result in a crash
   uint8_t disp_buf1[ buf_size_bytes ];
 #if USE_DOUBLE_DISPLAY_BUFFER
   uint8_t disp_buf2[ buf_size_bytes ];
 #else
   uint8_t * disp_buf2 = NULL;
 #endif
#else
   uint8_t* disp_buf1 = ( uint8_t* )heap_caps_malloc( buf_size_bytes, MALLOC_CAP_DMA );
   LV_ASSERT_MSG( disp_buf1 != NULL, "Can't allocate display buffer 1" );
 #if USE_DOUBLE_DISPLAY_BUFFER
   // Use double buffered
   uint8_t* disp_buf2 = ( uint8_t* )heap_caps_malloc( buf_size_bytes, MALLOC_CAP_DMA );
   LV_ASSERT_MSG( disp_buf2 != NULL, "Can't allocate display buffer 2" );
 #else
   uint8_t* disp_buf2 = NULL;
 #endif
#endif
    /* Initialize the working buffer depending on the selected display.
     * NOTE: buf2 == NULL when using monochrome displays. */

    /*-----------------------------------
     * Create the display in LVGL
     *----------------------------------*/
    /*Set also the resolution of the display*/
   lv_display_t * disp = lv_display_create( MY_DISP_HOR_RES, MY_DISP_VER_RES );
   lv_display_set_buffers( disp, (void *)disp_buf1, (void *)disp_buf2, buf_size_bytes, LV_DISPLAY_RENDER_MODE_PARTIAL );

    uint32_t size_in_px = buf_size_bytes;

#if defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_IL3820 || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_JD79653A || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_UC8151D || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_SSD1306
    /* Actual size in pixels, not bytes. */
    size_in_px *= 8;
#endif

    /*Used to copy the buffer's content to the display*/
   lv_display_set_flush_cb( disp, disp_driver_flush );

    /* When using a monochrome display we need to register the callbacks:
     * - rounder_cb
     * - set_px_cb */
#ifdef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    disp_drv.rounder_cb = disp_driver_rounder;
    disp_drv.set_px_cb  = disp_driver_set_px;
#endif

    ESP_LOGI(TAG, "Display hor size: %d, ver size: %d", LV_HOR_RES, LV_VER_RES);
    ESP_LOGI(TAG, "Display buffer size: %d", DISP_BUF_SIZE_CUSTOM);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
