#include "ui.h"

void UI::begin(AppConfig* cfg) {
    _cfg = cfg;
    _buildHome();
    _buildSettings();
    showHome();
}

void UI::update() {
    // Placeholder for periodic UI logic (animations, status updates)
}

void UI::showHome() {
    _active = Screen::Home;
    lv_scr_load(_screenHome);
}

void UI::showSettings() {
    _active = Screen::Settings;
    lv_scr_load(_screenSettings);
}

void UI::onLayerChanged(uint8_t layer) {
    if (!_cfg) return;
    _cfg->activeLayer = layer;
    // TODO: update layer indicator label on home screen
}

void UI::onEncoderDelta(int8_t delta) {
    if (_active == Screen::Settings)
        lv_group_focus_next(lv_group_get_default());  // navigate settings
}

void UI::onEncoderPress() {
    if (_active == Screen::Home)
        showSettings();
    else
        showHome();
}

// ─── Home screen ─────────────────────────────────────────────────────────────

void UI::_buildHome() {
    _screenHome = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(_screenHome, lv_color_black(), 0);

    // Layer name label
    lv_obj_t* lblLayer = lv_label_create(_screenHome);
    lv_label_set_text(lblLayer, _cfg ? _cfg->layers[_cfg->activeLayer].name : "---");
    lv_obj_align(lblLayer, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_font(lblLayer, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(lblLayer, lv_color_white(), 0);

    // Hint
    lv_obj_t* lblHint = lv_label_create(_screenHome);
    lv_label_set_text(lblHint, "Press encoder for settings");
    lv_obj_align(lblHint, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_text_font(lblHint, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(lblHint, lv_color_hex(0x888888), 0);
}

// ─── Settings screen ──────────────────────────────────────────────────────────

void UI::_buildSettings() {
    _screenSettings = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(_screenSettings, lv_color_black(), 0);

    lv_obj_t* title = lv_label_create(_screenSettings);
    lv_label_set_text(title, "Settings");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);

    // Brightness slider
    lv_obj_t* lblBright = lv_label_create(_screenSettings);
    lv_label_set_text(lblBright, "Brightness");
    lv_obj_align(lblBright, LV_ALIGN_TOP_LEFT, 10, 50);
    lv_obj_set_style_text_color(lblBright, lv_color_white(), 0);

    lv_obj_t* slider = lv_slider_create(_screenSettings);
    lv_slider_set_range(slider, 0, 255);
    lv_slider_set_value(slider, _cfg ? _cfg->brightness : 128, LV_ANIM_OFF);
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, LV_ALIGN_TOP_LEFT, 10, 75);

    // TODO: add layer selector, key binding editor
}
