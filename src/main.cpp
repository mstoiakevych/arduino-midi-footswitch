#include "MIDIUSB.h"
#include "MidiButton.h"
#include <Bounce2.h>

// Button 1
#define LED1_PIN 16
#define BTN1_PIN 10

// Button 2
#define BTN2_PIN 15

// Common settings
#define DEBOUNCE_INTERVAL 50
#define CHANNEL 15
#define BUTTONS_NUM 2
#define PRESETS_NUM 4

byte currentPreset = 0;

byte presetControls[BUTTONS_NUM][PRESETS_NUM] = {
        {80, 82, 84, 86},
        {81, 83, 85, 87}
};

MidiButton buttons[BUTTONS_NUM] = {
        MidiButton(BTN1_PIN, CHANNEL, DEBOUNCE_INTERVAL, LED1_PIN),
        MidiButton(BTN2_PIN, CHANNEL, DEBOUNCE_INTERVAL, 5)
};

void nextPreset() {
    if (currentPreset >= PRESETS_NUM - 1) currentPreset = 0;
    else currentPreset++;
}

void prevPreset() {
    if (currentPreset <= 0) currentPreset = PRESETS_NUM - 1;
    currentPreset--;
}

void setup() {
    for (auto & button : buttons) button.setup();
}

void loop() {
    if (buttons[0].isPressed() && buttons[1].hasButtonJustPressed()) nextPreset();
    if (buttons[1].isPressed() && buttons[0].hasButtonJustPressed()) prevPreset();

    for (int i = 0; i < BUTTONS_NUM; ++i) {
        buttons[i].update();

        if (buttons[i].hasButtonJustPressed()) buttons[i].handlePress(presetControls[i][currentPreset]);
        if (buttons[i].hasButtonJustReleased()) buttons[i].handleRelease(presetControls[i][currentPreset]);
    }
}