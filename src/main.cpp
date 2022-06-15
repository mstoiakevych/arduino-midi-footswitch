#include "MIDIUSB.h"
#include "MidiButton.h"
#include <Bounce2.h>

// Button 1
#define LED1_PIN 16
#define BTN1_PIN 10

// Button 2
#define LED2_PIN 2
#define BTN2_PIN 15

// Common settings
#define DEBOUNCE_INTERVAL 50
#define CHANNEL 15
#define BUTTONS_NUM 2
#define PRESETS_NUM 4

byte currentPreset = 1;
byte offset = 0;

MidiButton buttons[BUTTONS_NUM * PRESETS_NUM] = {
        MidiButton(BTN1_PIN, LED1_PIN, CHANNEL, 80, DEBOUNCE_INTERVAL),
        MidiButton(BTN2_PIN, LED2_PIN, CHANNEL, 81, DEBOUNCE_INTERVAL),

        MidiButton(BTN1_PIN, LED1_PIN, CHANNEL, 82, DEBOUNCE_INTERVAL),
        MidiButton(BTN2_PIN, LED2_PIN, CHANNEL, 83, DEBOUNCE_INTERVAL),

        MidiButton(BTN1_PIN, LED1_PIN, CHANNEL, 84, DEBOUNCE_INTERVAL),
        MidiButton(BTN2_PIN, LED2_PIN, CHANNEL, 85, DEBOUNCE_INTERVAL),

        MidiButton(BTN1_PIN, LED1_PIN, CHANNEL, 86, DEBOUNCE_INTERVAL),
        MidiButton(BTN2_PIN, LED2_PIN, CHANNEL, 87, DEBOUNCE_INTERVAL),
};

bool doNextPreset = false;
bool doPreviousPreset = false;

void nextPreset() {
    if (currentPreset >= PRESETS_NUM) {
        currentPreset = 1;
        offset = 0;
    } else {
        currentPreset++;
        offset += BUTTONS_NUM;
    }

    doNextPreset = false;
}

void prevPreset() {
    if (currentPreset <= 1) {
        currentPreset = PRESETS_NUM;
        offset = BUTTONS_NUM * (PRESETS_NUM - 1);
    } else {
        currentPreset--;
        offset -= BUTTONS_NUM;
    }

    doPreviousPreset = false;
}

void setup() {
    Serial.begin(9600);
    for (auto & button : buttons) button.setup();
}

void loop() {

    for (int i = offset; i < BUTTONS_NUM * currentPreset; ++i) buttons[i].update();

    if (buttons[offset].isPressed() && buttons[offset + 1].hasButtonJustPressed()) doNextPreset = true;
    else if (buttons[offset + 1].isPressed() && buttons[offset].hasButtonJustPressed()) doPreviousPreset = true;

    if (doNextPreset) {
        if (!buttons[offset].isPressed() && !buttons[offset + 1].isPressed()) {
            buttons[offset].cancel();

            nextPreset();
            for (int i = offset; i < BUTTONS_NUM * currentPreset; ++i) buttons[i].reset();
        }
        return;
    } else if (doPreviousPreset) {
        if (!buttons[offset].isPressed() && !buttons[offset + 1].isPressed()) {
            buttons[offset + 1].cancel();

            prevPreset();
            for (int i = offset; i < BUTTONS_NUM * currentPreset; ++i) buttons[i].reset();
        }
        return;
    }

    for (int i = offset; i < BUTTONS_NUM * currentPreset; ++i) {

        if (buttons[i].hasButtonJustPressed()) buttons[i].handlePress();
        if (buttons[i].hasButtonJustReleased()) buttons[i].handleRelease();
    }
}