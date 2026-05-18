#include "storage.h"

bool Storage::begin() {
    if (!LittleFS.begin()) {
        Serial.println("[storage] LittleFS mount failed");
        return false;
    }
    return true;
}

bool Storage::load(AppConfig& cfg) {
    if (!LittleFS.exists(CONFIG_FILE)) {
        Serial.println("[storage] config.json not found, using defaults");
        loadDefaults(cfg);
        return false;
    }

    File f = LittleFS.open(CONFIG_FILE, "r");
    if (!f) { loadDefaults(cfg); return false; }

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, f);
    f.close();
    if (err) {
        Serial.printf("[storage] JSON parse error: %s\n", err.c_str());
        loadDefaults(cfg);
        return false;
    }

    cfg.brightness   = doc["brightness"]    | LED_DEFAULT_BRIGHTNESS;
    cfg.activeLayer  = doc["active_layer"]  | 0;

    JsonArray layers = doc["layers"].as<JsonArray>();
    cfg.numLayers = 0;
    for (JsonObject obj : layers) {
        if (cfg.numLayers >= MAX_LAYERS) break;
        _parseLayer(obj, cfg.layers[cfg.numLayers++]);
    }
    if (cfg.numLayers == 0) loadDefaults(cfg);

    Serial.printf("[storage] loaded %d layer(s)\n", cfg.numLayers);
    return true;
}

bool Storage::save(const AppConfig& cfg) {
    JsonDocument doc;
    doc["brightness"]   = cfg.brightness;
    doc["active_layer"] = cfg.activeLayer;

    JsonArray layers = doc["layers"].to<JsonArray>();
    for (uint8_t i = 0; i < cfg.numLayers; i++)
        _serializeLayer(layers.add<JsonObject>(), cfg.layers[i]);

    File f = LittleFS.open(CONFIG_FILE, "w");
    if (!f) { Serial.println("[storage] cannot write config.json"); return false; }
    serializeJson(doc, f);
    f.close();
    return true;
}

void Storage::loadDefaults(AppConfig& cfg) {
    cfg.brightness  = LED_DEFAULT_BRIGHTNESS;
    cfg.activeLayer = 0;
    cfg.numLayers   = 1;
    cfg.layers[0]   = DEFAULT_LAYER;
}

bool Storage::_parseLayer(JsonObject obj, Layer& layer) {
    strlcpy(layer.name, obj["name"] | "Layer", MAX_LAYER_NAME_LEN);
    JsonArray color = obj["color"].as<JsonArray>();
    layer.color[0] = color[0] | 0;
    layer.color[1] = color[1] | 0;
    layer.color[2] = color[2] | 255;

    JsonArray keys = obj["keys"].as<JsonArray>();
    uint8_t i = 0;
    for (JsonObject k : keys) {
        if (i >= MATRIX_KEY_COUNT) break;
        const char* type = k["type"] | "key";
        layer.keys[i].code = k["code"] | 0;
        if (strcmp(type, "consumer") == 0)      layer.keys[i].type = KEY_CONSUMER;
        else if (strcmp(type, "layer") == 0)    layer.keys[i].type = KEY_LAYER;
        else if (strcmp(type, "macro") == 0)    layer.keys[i].type = KEY_MACRO;
        else                                     layer.keys[i].type = KEY_HID;
        i++;
    }
    for (; i < MATRIX_KEY_COUNT; i++) { layer.keys[i] = { KEY_NONE, 0 }; }
    return true;
}

void Storage::_serializeLayer(JsonObject obj, const Layer& layer) {
    obj["name"] = layer.name;
    JsonArray color = obj["color"].to<JsonArray>();
    color.add(layer.color[0]);
    color.add(layer.color[1]);
    color.add(layer.color[2]);

    JsonArray keys = obj["keys"].to<JsonArray>();
    for (uint8_t i = 0; i < MATRIX_KEY_COUNT; i++) {
        JsonObject k = keys.add<JsonObject>();
        switch (layer.keys[i].type) {
            case KEY_CONSUMER: k["type"] = "consumer"; break;
            case KEY_LAYER:    k["type"] = "layer";    break;
            case KEY_MACRO:    k["type"] = "macro";    break;
            default:           k["type"] = "key";      break;
        }
        k["code"] = layer.keys[i].code;
    }
}
