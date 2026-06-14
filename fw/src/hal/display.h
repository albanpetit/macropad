#pragma once
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "../config/config.h"

class Display {
public:
    void begin();
    void tick();
    void update();
    void setBacklight(uint8_t brightness);
    void fillRaw(uint16_t color, uint32_t count);

private:
    TFT_eSPI _tft;

    static Display*            _instance;
    static lv_disp_draw_buf_t  _drawBuf;
    static lv_color_t          _buf1[LCD_WIDTH * 20];
    static lv_color_t          _buf2[LCD_WIDTH * 20];
    static lv_disp_drv_t       _dispDrv;

    void _customInit();

    static void _flushCb(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_p);
};
