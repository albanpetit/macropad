#pragma once
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "../config/config.h"
#include "../config/keymap.h"

struct AppConfig {
    uint8_t brightness;
    uint8_t activeLayer;
    uint8_t numLayers;
    Layer   layers[MAX_LAYERS];
};

class Storage {
public:
    bool begin();
    bool load(AppConfig& cfg);
    bool save(const AppConfig& cfg);
    void loadDefaults(AppConfig& cfg);

private:
    bool _parseLayer(JsonObject obj, Layer& layer);
    void _serializeLayer(JsonObject obj, const Layer& layer);
};
