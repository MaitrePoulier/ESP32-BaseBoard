#ifndef __LVGL_CODE_H__
#define __LVGL_CODE_H__

    #include "lvgl.h"

    void my_flush_cb(lv_display_t *, const lv_area_t *, uint8_t *);
    void my_touchpad_read(lv_indev_t *, lv_indev_data_t *);

    // UI Creation Function
    void create_test_ui(void); 

    uint32_t my_tick(void);
    void ScreenInit(void);
    void touch_calibrate(void);

#endif