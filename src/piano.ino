#include <WiFi.h>
#include <WebServer.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include "secrets.h"

WebServer server(80);

const int PIN_LEFT = GPIO_NUM_5;
const int PIN_MIDDLE = GPIO_NUM_6;
const adc1_channel_t ADC_RIGHT = ADC1_CHANNEL_3;

void init_pins() {
    adc1_config_width(ADC_WIDTH_MAX);
    adc1_config_channel_atten(ADC_RIGHT, ADC_ATTEN_DB_12);
    pinMode(PIN_LEFT, INPUT_PULLDOWN);
    pinMode(PIN_MIDDLE, INPUT_PULLDOWN);    
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
        int adc_right = adc1_get_raw(ADC_RIGHT);
        int pin_left = digitalRead(PIN_LEFT);
        int pin_middle = digitalRead(PIN_MIDDLE);

        char json[256];
        snprintf(json, sizeof(json),
            "{\"LEFT\":%d,\"MIDDLE\":%d,\"RIGHT\":%d,\"rssi\":%d}",
            pin_left, pin_middle, adc_right, WiFi.RSSI());
        server.send(200, "application/json", json);
    });

    server.begin();
}

void loop() {
    server.handleClient();
}
