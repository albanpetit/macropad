#include "display.h"
#include <hardware/spi.h>
#include <hardware/gpio.h>

Display*           Display::_instance = nullptr;
lv_disp_draw_buf_t Display::_drawBuf;
lv_color_t         Display::_buf1[LCD_WIDTH * 20];
lv_color_t         Display::_buf2[LCD_WIDTH * 20];
lv_disp_drv_t      Display::_dispDrv;

// ─── SPI pico-sdk pur — 8 bits polling, pas de DMA, pas d'arduino SPI ────────

static void spiInit() {
    spi_init(spi1, 15000000);  // 15 MHz — max spec ST7789 (66 ns/cycle)
    spi_set_format(spi1, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(PIN_LCD_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_LCD_SCLK, GPIO_FUNC_SPI);
    // PIN_LCD_BL, CS, DC, RST gérés en GPIO pur
}

static void spiCmd(uint8_t cmd) {
    gpio_put(PIN_LCD_CS, 0);
    gpio_put(PIN_LCD_DC, 0);
    spi_write_blocking(spi1, &cmd, 1);
    gpio_put(PIN_LCD_CS, 1);
}

static void spiData(uint8_t d) {
    gpio_put(PIN_LCD_CS, 0);
    gpio_put(PIN_LCD_DC, 1);
    spi_write_blocking(spi1, &d, 1);
    gpio_put(PIN_LCD_CS, 1);
}

static void setAddrRaw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint8_t buf[4];

    spiCmd(0x2A);
    gpio_put(PIN_LCD_CS, 0); gpio_put(PIN_LCD_DC, 1);
    buf[0]=x1>>8; buf[1]=x1&0xFF; buf[2]=x2>>8; buf[3]=x2&0xFF;
    spi_write_blocking(spi1, buf, 4);
    gpio_put(PIN_LCD_CS, 1);

    spiCmd(0x2B);
    gpio_put(PIN_LCD_CS, 0); gpio_put(PIN_LCD_DC, 1);
    buf[0]=y1>>8; buf[1]=y1&0xFF; buf[2]=y2>>8; buf[3]=y2&0xFF;
    spi_write_blocking(spi1, buf, 4);
    gpio_put(PIN_LCD_CS, 1);

    spiCmd(0x2C);
}

void Display::fillRaw(uint16_t color, uint32_t count) {
    uint8_t buf[2] = { (uint8_t)(color >> 8), (uint8_t)(color & 0xFF) };
    gpio_put(PIN_LCD_CS, 0);
    gpio_put(PIN_LCD_DC, 1);
    for (uint32_t i = 0; i < count; i++)
        spi_write_blocking(spi1, buf, 2);
    gpio_put(PIN_LCD_CS, 1);
}

// ─── Init propriétaire ST7789V2-HSD ──────────────────────────────────────────

void Display::_customInit() {
    spiInit();

    gpio_put(PIN_LCD_RST, 1); sleep_ms(100);
    gpio_put(PIN_LCD_RST, 0); sleep_ms(200);
    gpio_put(PIN_LCD_RST, 1); sleep_ms(500);

    spiCmd(0x11); sleep_ms(120);

    spiCmd(0x36); spiData(0x00);
    spiCmd(0x3A); spiData(0x05);

    spiCmd(0xB2);
    spiData(0x0C); spiData(0x0C); spiData(0x00); spiData(0x33); spiData(0x33);
    spiCmd(0xB7); spiData(0x35);
    spiCmd(0xBB); spiData(0x20);
    spiCmd(0xC0); spiData(0x2C);
    spiCmd(0xC2); spiData(0x01);
    spiCmd(0xC3); spiData(0x0B);
    spiCmd(0xC4); spiData(0x20);
    spiCmd(0xC6); spiData(0x0F);
    spiCmd(0xD0); spiData(0xA4); spiData(0xA1);

    spiCmd(0xE0);
    spiData(0xD0); spiData(0x03); spiData(0x09); spiData(0x0E);
    spiData(0x11); spiData(0x3D); spiData(0x47); spiData(0x55);
    spiData(0x53); spiData(0x1A); spiData(0x16); spiData(0x14);
    spiData(0x1F); spiData(0x22);

    spiCmd(0xE1);
    spiData(0xD0); spiData(0x02); spiData(0x08); spiData(0x0D);
    spiData(0x12); spiData(0x2C); spiData(0x43); spiData(0x55);
    spiData(0x53); spiData(0x1E); spiData(0x1B); spiData(0x19);
    spiData(0x20); spiData(0x22);

    spiCmd(0x21); sleep_ms(10);   // INVON — requis pour ce panel IPS ST7789V2-HSD
    spiCmd(0x29); sleep_ms(10);

    spiCmd(0x36); spiData(0x00);  // portrait 240×320

    setAddrRaw(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
    fillRaw(0x0000, (uint32_t)LCD_WIDTH * LCD_HEIGHT);
}

void Display::begin() {
    _instance = this;

    gpio_init(PIN_LCD_BL);  gpio_set_dir(PIN_LCD_BL, GPIO_OUT); gpio_put(PIN_LCD_BL, 1);
    gpio_init(PIN_LCD_CS);  gpio_set_dir(PIN_LCD_CS, GPIO_OUT); gpio_put(PIN_LCD_CS, 1);
    gpio_init(PIN_LCD_DC);  gpio_set_dir(PIN_LCD_DC, GPIO_OUT); gpio_put(PIN_LCD_DC, 1);
    gpio_init(PIN_LCD_RST); gpio_set_dir(PIN_LCD_RST, GPIO_OUT); gpio_put(PIN_LCD_RST, 1);

    _customInit();

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
    // PWM simple via GPIO (pas de conflit SPI)
    gpio_put(PIN_LCD_BL, brightness > 0 ? 1 : 0);
}

void Display::_flushCb(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_p) {
    if (!_instance) { lv_disp_flush_ready(drv); return; }

    uint32_t count = (uint32_t)(area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
    uint16_t* colors = (uint16_t*)color_p;

    setAddrRaw(area->x1, area->y1, area->x2, area->y2);

    // Prépare le buffer avec bytes swappés, puis un seul spi_write_blocking
    static uint8_t txbuf[LCD_WIDTH * 20 * 2];
    for (uint32_t i = 0; i < count; i++) {
        txbuf[i*2]   = colors[i] >> 8;
        txbuf[i*2+1] = colors[i] & 0xFF;
    }
    gpio_put(PIN_LCD_CS, 0);
    gpio_put(PIN_LCD_DC, 1);
    spi_write_blocking(spi1, txbuf, count * 2);
    gpio_put(PIN_LCD_CS, 1);

    lv_disp_flush_ready(drv);
}
