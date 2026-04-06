#include <WiFi.h>
#include <WebServer.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include "secrets.h"

WebServer server(80);

const int PIN_ADC1_4 = 32;
const int PIN_ADC1_5 = 33;
const int PIN_ADC1_6 = 34;

void init_pins() {
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_12);
    pinMode(PIN_ADC1_5, INPUT);
    pinMode(PIN_ADC1_6, INPUT);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    init_pins();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());

    server.on("/", []() {
        int adc1_4_raw = adc1_get_raw(ADC1_CHANNEL_4);
        int adc1_5_raw = digitalRead(PIN_ADC1_5);
        int adc1_6_raw = digitalRead(PIN_ADC1_6);

        char json[256];
        snprintf(json, sizeof(json),
            "{\"ADC1_4\":%d,\"ADC1_5\":%d,\"ADC1_6\":%d,\"rssi\":%d}",
            adc1_4_raw, adc1_5_raw, adc1_6_raw, WiFi.RSSI());
        server.send(200, "application/json", json);
    });

    server.begin();
}

void loop() {
    server.handleClient();
}
