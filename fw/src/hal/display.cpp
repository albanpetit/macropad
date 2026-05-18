#include "display.h"

Display*           Display::_instance = nullptr;
lv_disp_draw_buf_t Display::_drawBuf;
lv_color_t         Display::_buf1[LCD_WIDTH * 20];
lv_color_t         Display::_buf2[LCD_WIDTH * 20];
lv_disp_drv_t      Display::_dispDrv;

void Display::begin() {
    _instance = this;

    pinMode(PIN_LCD_BL, OUTPUT);
    setBacklight(255);

    _tft.begin();
    _tft.setRotation(LCD_ROTATION);
    _tft.fillScreen(TFT_BLACK);

    lv_init();
    lv_disp_draw_buf_init(&_drawBuf, _buf1, _buf2, LCD_WIDTH * 20);

    lv_disp_drv_init(&_dispDrv);
    _dispDrv.hor_res  = LCD_WIDTH;
    _dispDrv.ver_res  = LCD_HEIGHT;
    _dispDrv.flush_cb = _flushCb;
    _dispDrv.draw_buf = &_drawBuf;
    lv_disp_drv_register(&_dispDrv);
}

void Display::tick() {
    lv_tick_inc(UI_TICK_MS);
}

void Display::update() {
    lv_task_handler();
}

void Display::setBacklight(uint8_t brightness) {
    analogWrite(PIN_LCD_BL, brightness);
}

void Display::_flushCb(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_p) {
    if (!_instance) { lv_disp_flush_ready(drv); return; }

    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    _instance->_tft.startWrite();
    _instance->_tft.setAddrWindow(area->x1, area->y1, w, h);
    _instance->_tft.pushColors((uint16_t*)color_p, w * h, true);
    _instance->_tft.endWrite();

    lv_disp_flush_ready(drv);
}
