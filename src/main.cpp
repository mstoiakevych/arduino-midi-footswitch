#include "MIDIUSB.h"
#include "MidiButton.h"
#include <Bounce2.h>

#define LED 16
#define BTN1 10
#define BTN2 15
#define INTERVAL 50
#define CHANNEL 15
#define BUTTONS_NUM 2

MidiButton buttons[BUTTONS_NUM] = {
        MidiButton(BTN1, CHANNEL, 80, INTERVAL, LED),
        MidiButton(BTN2, CHANNEL, 81, INTERVAL, 5)
};

void setup() {
    Serial.begin(9600);
    for (auto & button : buttons) button.setup();
}

void loop() {
//    for (auto & button : buttons) button.update();
    for (auto & button : buttons) button.handleClick();
//    for (auto & button : buttons) {
//        if (button.hasButtonJustPressed()) {
//            currentTimestamp = millis();
//
//            switch (mode) {
//                case Switch:
//                    button.toggle();
//
//                    break;
//                case Momentary:
//                    button.enable();
//                    break;
//            }
//
//            lastTimestamp = currentTimestamp;
//        }
//
//        if (button.hasButtonJustReleased()) {
//            switch (mode) {
//                case Switch:
//                    break;
//                case Momentary:
//                    button.disable();
//                    break;
//            }
//        }
//    }
}

/*
if (modeJustSwitched) {
        if (buttons[0]._isPressed() || buttons[1].hasButtonJustReleased()) return;
        if (buttons[1]._isPressed() || buttons[0].hasButtonJustReleased()) return;

        modeJustSwitched = false;

        switch (mode) {

            case Switch: // Momentary -> Switch
                buttons[0].disable();
                break;
            case Momentary: // Switch -> Momentary
                buttons[0].toggle(false);
                break;
        }

        for (auto & button : buttons) button.reset();
    }

    if (buttons[0]._isPressed() && buttons[1].hasButtonJustPressed()) {
        modeJustSwitched = true;

        switch (mode) {
            case Switch:
                mode = SwitchMode::Momentary;
                break;
            case Momentary:
                mode = SwitchMode::Switch;
                break;
        }

        return;
    }
*/