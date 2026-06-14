#if defined(LV_CONF_INCLUDE_SIMPLE)

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0

#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (48 * 1024U)  // 48 KB for LVGL heap

#define LV_DISP_DEF_REFR_PERIOD 16  // ~60 fps

#define LV_INDEV_DEF_READ_PERIOD 30

#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF 1

// Fonts
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

// Widgets
#define LV_USE_ARC        1
#define LV_USE_BAR        1
#define LV_USE_BTN        1
#define LV_USE_BTNMATRIX  1
#define LV_USE_LABEL      1
#define LV_USE_LIST       1
#define LV_USE_ROLLER     1
#define LV_USE_SLIDER     1
#define LV_USE_SWITCH     1
#define LV_USE_TABLE      1
#define LV_USE_TEXTAREA   1

// Themes
#define LV_USE_THEME_DEFAULT 1
#define LV_THEME_DEFAULT_DARK 1

// Animation
#define LV_USE_ANIMATION 1

#endif // LV_CONF_H
#endif // LV_CONF_INCLUDE_SIMPLE
