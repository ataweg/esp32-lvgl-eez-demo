#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *page1;
    lv_obj_t *page2;
    lv_obj_t *page3;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_PAGE1 = 2,
    SCREEN_ID_PAGE2 = 3,
    SCREEN_ID_PAGE3 = 4,
};

void create_screen_main();
void tick_screen_main();

void create_screen_page1();
void tick_screen_page1();

void create_screen_page2();
void tick_screen_page2();

void create_screen_page3();
void tick_screen_page3();

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/