//
// Created by BlakkBerry on 10/06/2022.
//

#include "Bounce2.h"
#include "SwitchMode.h"

class MidiButton {
private:
    Bounce2::Button bounce;
    byte pin;
    byte channel;
    byte interval;
    byte lightPin;
    bool isSwitched = false;
    SwitchMode switchMode = SwitchMode::Switch;

    unsigned long lastTimestamp;
    unsigned long currentTimestamp;
public:

    MidiButton() = delete;

    MidiButton(byte pin, byte channel, byte interval, byte lightPin) {
        this->bounce = Bounce2::Button();
        this->pin = pin;
        this->channel = channel;
        this->interval = interval;
        this->lightPin = lightPin;
        pinMode(lightPin, OUTPUT);

        lastTimestamp = millis();
    }

    void setup() {
        bounce.attach(pin, INPUT_PULLUP);
        bounce.interval(interval);
        bounce.setPressedState(LOW); // because of INPUT_PULLUP
    }

    void update() {
        this->bounce.update();
    }

    bool hasButtonJustPressed() {
        return bounce.pressed();
    }

    bool hasButtonJustReleased() {
        return bounce.released();
    }

    bool isPressed() const {
        return bounce.isPressed();
    }

    void toggle(byte control, bool switchLight = true) {
        isSwitched = !isSwitched;

        if (isSwitched) {
            MIDI_::enableControlChange(channel, control);
            if (switchLight) digitalWrite(lightPin, HIGH);
        } else {
            MIDI_::disableControlChange(channel, control);
            if (switchLight) digitalWrite(lightPin, LOW);
        }
    }

    void enable(byte control, bool enableLight = true) const {
        MIDI_::enableControlChange(channel, control);
        if (enableLight) digitalWrite(lightPin, HIGH);
    }

    void disable(byte control, bool disableLight = true) const {
        MIDI_::disableControlChange(channel, control);
        if (disableLight) digitalWrite(lightPin, LOW);
    }

    void resetSwitched(byte control) {
        if (isSwitched) disable(control);
        isSwitched = false;
    }

    void handlePress(byte control) {
        switch (switchMode) {
            case Switch:
                toggle(control);
                break;
            case Momentary:
                enable(control);
                break;
        }
    }

    void handleRelease(byte control) {
        currentTimestamp = millis();

        switch (switchMode) {
            case Switch:
                if (currentTimestamp - lastTimestamp < 350) {
                    switchMode = Momentary;
                    resetSwitched(control);
                }
                break;
            case Momentary:
                disable(control);
                if (currentTimestamp - lastTimestamp < 350) {
                    switchMode = Switch;
                }
                break;
        }

        lastTimestamp = currentTimestamp;
    }
};