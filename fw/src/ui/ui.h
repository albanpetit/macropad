#pragma once
#include <lvgl.h>
#include "../storage/storage.h"

enum class Screen { Home, Settings };

class UI {
public:
    void begin(AppConfig* cfg);
    void update();

    void showHome();
    void showSettings();

    void onLayerChanged(uint8_t layer);
    void onEncoderDelta(int8_t delta);
    void onEncoderPress();

private:
    AppConfig*  _cfg    = nullptr;
    Screen      _active = Screen::Home;

    lv_obj_t* _screenHome     = nullptr;
    lv_obj_t* _screenSettings = nullptr;

    void _buildHome();
    void _buildSettings();
};
