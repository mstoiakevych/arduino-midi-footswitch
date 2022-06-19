#include "MIDIUSB.h"
#include "MidiButton.h"
#include <Bounce2.h>

// Buttons

// Button 1
#define LED1_PIN 9
#define BTN1_PIN 10

// Button 2
#define LED2_PIN 8
#define BTN2_PIN 16

// Button 3
#define LED3_PIN 7
#define BTN3_PIN 14

// Button 4
#define LED4_PIN 6
#define BTN4_PIN 15

// LCD Screen
#define RS_PIN A0
#define EN_PIN A1
#define D4_PIN A2
#define D5_PIN A3
#define D6_PIN 3
#define D7_PIN 4

// Common settings
#define DEBOUNCE_INTERVAL 50
#define CHANNEL 15
#define BUTTONS_NUM 4
#define PRESETS_NUM 4

byte currentPreset = 1;
byte offset = 0;

LcdScreen screen(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN, 3000);

MidiButton buttons[BUTTONS_NUM * PRESETS_NUM] = {
        MidiButton(1, BTN1_PIN, LED1_PIN, CHANNEL, 80, DEBOUNCE_INTERVAL, screen),
        MidiButton(2, BTN2_PIN, LED2_PIN, CHANNEL, 81, DEBOUNCE_INTERVAL, screen),
        MidiButton(3, BTN3_PIN, LED3_PIN, CHANNEL, 82, DEBOUNCE_INTERVAL, screen),
        MidiButton(4, BTN4_PIN, LED4_PIN, CHANNEL, 83, DEBOUNCE_INTERVAL, screen),

        MidiButton(1, BTN1_PIN, LED1_PIN, CHANNEL, 84, DEBOUNCE_INTERVAL, screen),
        MidiButton(2, BTN2_PIN, LED2_PIN, CHANNEL, 85, DEBOUNCE_INTERVAL, screen),
        MidiButton(3, BTN3_PIN, LED3_PIN, CHANNEL, 86, DEBOUNCE_INTERVAL, screen),
        MidiButton(4, BTN4_PIN, LED4_PIN, CHANNEL, 87, DEBOUNCE_INTERVAL, screen),

        MidiButton(1, BTN1_PIN, LED1_PIN, CHANNEL, 88, DEBOUNCE_INTERVAL, screen),
        MidiButton(2, BTN2_PIN, LED2_PIN, CHANNEL, 89, DEBOUNCE_INTERVAL, screen),
        MidiButton(3, BTN3_PIN, LED3_PIN, CHANNEL, 90, DEBOUNCE_INTERVAL, screen),
        MidiButton(4, BTN4_PIN, LED4_PIN, CHANNEL, 91, DEBOUNCE_INTERVAL, screen),

        MidiButton(1, BTN1_PIN, LED1_PIN, CHANNEL, 92, DEBOUNCE_INTERVAL, screen),
        MidiButton(2, BTN2_PIN, LED2_PIN, CHANNEL, 93, DEBOUNCE_INTERVAL, screen),
        MidiButton(3, BTN3_PIN, LED3_PIN, CHANNEL, 94, DEBOUNCE_INTERVAL, screen),
        MidiButton(4, BTN4_PIN, LED4_PIN, CHANNEL, 95, DEBOUNCE_INTERVAL, screen),
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
    screen.setup();
    for (auto & button : buttons) button.setup();

    screen.start();
}

void loop() {
    for (int i = offset; i < BUTTONS_NUM * currentPreset; ++i) buttons[i].update();

    if (buttons[offset].isPressed() && buttons[offset + 1].hasButtonJustPressed()) doNextPreset = true;
    else if (buttons[offset + 1].isPressed() && buttons[offset].hasButtonJustPressed()) doPreviousPreset = true;

    if (doNextPreset) {
        if (!buttons[offset].isPressed() && !buttons[offset + 1].isPressed()) {
            buttons[offset].cancel();

            nextPreset();
            screen.setPreset(currentPreset);
            for (int i = offset; i < BUTTONS_NUM * currentPreset; ++i) buttons[i].reset();
        }
        return;
    } else if (doPreviousPreset) {
        if (!buttons[offset].isPressed() && !buttons[offset + 1].isPressed()) {
            buttons[offset + 1].cancel();

            prevPreset();
            screen.setPreset(currentPreset);
            for (int i = offset; i < BUTTONS_NUM * currentPreset; ++i) buttons[i].reset();
        }
        return;
    }

    for (int i = offset; i < BUTTONS_NUM * currentPreset; ++i) {

        if (buttons[i].hasButtonJustPressed()) buttons[i].handlePress();
        if (buttons[i].hasButtonJustReleased()) buttons[i].handleRelease();
    }
}