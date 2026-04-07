#include <USB.h>
#include <USBMIDI.h>
#include <driver/adc.h>

USBMIDI MIDI("Casio Piano Pedals");

const int PIN_LEFT = GPIO_NUM_5;
const int PIN_MIDDLE = GPIO_NUM_6;
const adc1_channel_t ADC_RIGHT = ADC1_CHANNEL_3;

const int CC_LEFT_PEDAL = 67;
const int CC_MIDDLE_PEDAL = 66;
const int CC_RIGHT_PEDAL = 64;

bool leftPressed = false;
bool middlePressed = false;
int lastRightValue = -1;

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

    MIDI.begin();
    USB.begin();

    Serial.println("Casio Piano Pedals USB MIDI Controller Ready");
}

void loop() {
    bool leftNow = digitalRead(PIN_LEFT) == HIGH;
    if (leftNow != leftPressed) {
        leftPressed = leftNow;
        MIDI.controlChange(CC_LEFT_PEDAL, leftNow ? 127 : 0, 1);
        Serial.printf("Left pedal: %s\n", leftNow ? "pressed" : "released");
    }

    bool middleNow = digitalRead(PIN_MIDDLE) == HIGH;
    if (middleNow != middlePressed) {
        middlePressed = middleNow;
        MIDI.controlChange(CC_MIDDLE_PEDAL, middleNow ? 127 : 0, 1);
        Serial.printf("Middle pedal: %s\n", middleNow ? "pressed" : "released");
    }

    int rightRaw = adc1_get_raw(ADC_RIGHT);
    int rightValue = map(rightRaw, 850, 4090, 127, 0);
    rightValue = constrain(rightValue, 0, 127);

    if (abs(rightValue - lastRightValue) > 2) {
        lastRightValue = rightValue;
        MIDI.controlChange(CC_RIGHT_PEDAL, rightValue, 1);
        Serial.printf("Right pedal: %d (raw: %d)\n", rightValue, rightRaw);
    }
}
