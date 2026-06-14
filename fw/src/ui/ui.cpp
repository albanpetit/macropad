#include "ui.h"

// ─── Helpers ──────────────────────────────────────────────────────────────────

static lv_style_t s_screen, s_hdr, s_item, s_item_sel, s_ftr;
static bool styles_init = false;

static void initStyles() {
    if (styles_init) return;
    styles_init = true;

    lv_style_init(&s_screen);
    lv_style_set_bg_color(&s_screen, UI_BLACK);
    lv_style_set_bg_opa(&s_screen, LV_OPA_COVER);
    lv_style_set_border_width(&s_screen, 0);
    lv_style_set_pad_all(&s_screen, 0);
    lv_style_set_radius(&s_screen, 0);

    lv_style_init(&s_hdr);
    lv_style_set_bg_color(&s_hdr, UI_YELLOW);
    lv_style_set_bg_opa(&s_hdr, LV_OPA_COVER);
    lv_style_set_border_width(&s_hdr, 0);
    lv_style_set_radius(&s_hdr, 0);
    lv_style_set_pad_all(&s_hdr, 0);

    lv_style_init(&s_item);
    lv_style_set_bg_color(&s_item, lv_color_hex(0x141414));
    lv_style_set_bg_opa(&s_item, LV_OPA_COVER);
    lv_style_set_border_width(&s_item, 0);
    lv_style_set_radius(&s_item, 6);
    lv_style_set_pad_left(&s_item, 16);
    lv_style_set_pad_right(&s_item, 12);
    lv_style_set_pad_top(&s_item, 0);

    lv_style_init(&s_item_sel);
    lv_style_set_bg_color(&s_item_sel, UI_YELLOW);
    lv_style_set_bg_opa(&s_item_sel, LV_OPA_COVER);
    lv_style_set_border_width(&s_item_sel, 0);
    lv_style_set_radius(&s_item_sel, 6);
    lv_style_set_pad_left(&s_item_sel, 16);
    lv_style_set_pad_right(&s_item_sel, 12);
    lv_style_set_pad_top(&s_item_sel, 0);

    lv_style_init(&s_ftr);
    lv_style_set_bg_color(&s_ftr, lv_color_hex(0x0A0A0A));
    lv_style_set_bg_opa(&s_ftr, LV_OPA_COVER);
    lv_style_set_border_width(&s_ftr, 1);
    lv_style_set_border_side(&s_ftr, LV_BORDER_SIDE_TOP);
    lv_style_set_border_color(&s_ftr, lv_color_hex(0x2A2A2A));
    lv_style_set_radius(&s_ftr, 0);
    lv_style_set_pad_all(&s_ftr, 0);
}

// ─── Build ────────────────────────────────────────────────────────────────────

void UI::_build() {
    initStyles();

    _screen = lv_obj_create(nullptr);
    lv_obj_add_style(_screen, &s_screen, 0);

    // ── Header ────────────────────────────────────────────────────────────────
    lv_obj_t* hdr = lv_obj_create(_screen);
    lv_obj_set_size(hdr, LCD_WIDTH, 56);
    lv_obj_set_pos(hdr, 0, 0);
    lv_obj_add_style(hdr, &s_hdr, 0);

    lv_obj_t* hTitle = lv_label_create(hdr);
    lv_label_set_text(hTitle, "MACROPAD");
    lv_obj_set_style_text_font(hTitle, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(hTitle, UI_BLACK, 0);
    lv_obj_align(hTitle, LV_ALIGN_LEFT_MID, 14, 0);

    // Profil actif dans le header (à droite)
    _lblActive = lv_label_create(hdr);
    lv_obj_set_style_text_font(_lblActive, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(_lblActive, lv_color_hex(0x333333), 0);
    lv_obj_align(_lblActive, LV_ALIGN_RIGHT_MID, -12, 0);

    // ── Zone liste ────────────────────────────────────────────────────────────
    lv_obj_t* body = lv_obj_create(_screen);
    lv_obj_set_size(body, LCD_WIDTH, LCD_HEIGHT - 56 - 44);
    lv_obj_set_pos(body, 0, 56);
    lv_obj_add_style(body, &s_screen, 0);
    lv_obj_set_flex_flow(body, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(body, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(body, 10, 0);
    lv_obj_set_style_pad_row(body, 6, 0);
    lv_obj_set_scroll_dir(body, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(body, LV_SCROLLBAR_MODE_OFF);

    uint8_t n = _cfg ? _cfg->numLayers : 0;
    for (uint8_t i = 0; i < n && i < MAX_LAYERS; i++) {
        lv_obj_t* row = lv_obj_create(body);
        lv_obj_set_size(row, LCD_WIDTH - 20, 52);
        lv_obj_add_style(row, &s_item, 0);

        // Bande couleur gauche
        auto& col = _cfg->layers[i].color;
        lv_obj_t* band = lv_obj_create(row);
        lv_obj_set_size(band, 4, 32);
        lv_obj_set_style_bg_color(band, lv_color_make(col[0], col[1], col[2]), 0);
        lv_obj_set_style_bg_opa(band, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(band, 0, 0);
        lv_obj_set_style_radius(band, 2, 0);
        lv_obj_align(band, LV_ALIGN_LEFT_MID, 0, 0);

        // Nom du profil
        lv_obj_t* lbl = lv_label_create(row);
        lv_label_set_text(lbl, _cfg->layers[i].name);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_16, 0);
        lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 12, 0);

        // Check actif
        lv_obj_t* chk = lv_label_create(row);
        lv_label_set_text(chk, "");
        lv_obj_set_style_text_font(chk, &lv_font_montserrat_16, 0);
        lv_obj_align(chk, LV_ALIGN_RIGHT_MID, 0, 0);

        _items[i] = row;
    }

    // ── Footer ────────────────────────────────────────────────────────────────
    lv_obj_t* ftr = lv_obj_create(_screen);
    lv_obj_set_size(ftr, LCD_WIDTH, 44);
    lv_obj_align(ftr, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_style(ftr, &s_ftr, 0);

    lv_obj_t* hint = lv_label_create(ftr);
    lv_label_set_text(hint, LV_SYMBOL_UP "/" LV_SYMBOL_DOWN "  naviguer       " LV_SYMBOL_OK "  activer");
    lv_obj_set_style_text_color(hint, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_12, 0);
    lv_obj_align(hint, LV_ALIGN_CENTER, 0, 0);

    lv_scr_load(_screen);
    _refresh();
}

// ─── Refresh ──────────────────────────────────────────────────────────────────

void UI::_refresh() {
    if (!_cfg) return;

    // Profil actif dans le header
    if (_lblActive)
        lv_label_set_text(_lblActive, _cfg->layers[_cfg->activeLayer].name);

    for (uint8_t i = 0; i < _cfg->numLayers && i < MAX_LAYERS; i++) {
        if (!_items[i]) continue;
        bool sel    = (i == _cursor);
        bool active = (i == _cfg->activeLayer);

        // Style du conteneur
        lv_obj_remove_style_all(_items[i]);
        lv_obj_add_style(_items[i], sel ? &s_item_sel : &s_item, 0);

        // Label (child 1)
        lv_obj_t* lbl = lv_obj_get_child(_items[i], 1);
        if (lbl) lv_obj_set_style_text_color(lbl, sel ? UI_BLACK : UI_WHITE, 0);

        // Check (child 2)
        lv_obj_t* chk = lv_obj_get_child(_items[i], 2);
        if (chk) {
            lv_label_set_text(chk, active ? LV_SYMBOL_OK : "");
            lv_obj_set_style_text_color(chk, sel ? UI_BLACK : UI_YELLOW, 0);
        }

        if (sel) lv_obj_scroll_to_view(_items[i], LV_ANIM_ON);
    }
}

// ─── Interface publique ───────────────────────────────────────────────────────

void UI::begin(AppConfig* cfg) {
    _cfg    = cfg;
    _cursor = cfg ? cfg->activeLayer : 0;
    _lblActive = nullptr;
    _build();
}

void UI::update() {}

bool UI::layerChanged() {
    if (_changed) { _changed = false; return true; }
    return false;
}

void UI::onEncoderDelta(int8_t delta) {
    if (!_cfg || _cfg->numLayers == 0) return;
    int next = (int)_cursor + delta;
    if (next < 0) next = _cfg->numLayers - 1;
    if (next >= (int)_cfg->numLayers) next = 0;
    _cursor = (uint8_t)next;
    _refresh();
}

void UI::onEncoderPress() {
    if (!_cfg || _cursor >= _cfg->numLayers) return;
    _cfg->activeLayer = _cursor;
    _changed = true;
    _refresh();
}
