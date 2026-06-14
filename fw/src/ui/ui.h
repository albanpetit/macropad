#pragma once
#include <lvgl.h>
#include "../storage/storage.h"

#define UI_YELLOW lv_color_hex(0xF9DB58)
#define UI_BLACK  lv_color_black()
#define UI_WHITE  lv_color_white()
#define UI_GRAY   lv_color_hex(0x444444)
#define UI_DIM    lv_color_hex(0x888888)
#define UI_ITEM   lv_color_hex(0x1A1A1A)

class UI {
public:
    void begin(AppConfig* cfg);
    void update();
    void onEncoderDelta(int8_t delta);
    void onEncoderPress();
    bool layerChanged();

private:
    AppConfig* _cfg      = nullptr;
    uint8_t    _cursor   = 0;
    bool       _changed  = false;

    lv_obj_t*  _screen    = nullptr;
    lv_obj_t*  _lblActive = nullptr;
    lv_obj_t*  _items[MAX_LAYERS] = {};

    void _build();
    void _refresh();
};
