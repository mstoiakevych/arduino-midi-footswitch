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
    byte control;
    byte interval;
    byte lightPin;
    bool isSwitched = false;

    unsigned long lastTimestamp;
    unsigned long currentTimestamp;
public:
    SwitchMode switchMode;

    MidiButton() = delete;

    MidiButton(byte pin, byte channel, byte control, byte interval, byte lightPin) {
        this->bounce = Bounce2::Button();
        this->pin = pin;
        this->channel = channel;
        this->control = control;
        this->interval = interval;
        this->lightPin = lightPin;
        pinMode(lightPin, OUTPUT);

        this->switchMode = SwitchMode::Switch;
        this->lastTimestamp = millis();
    }

    void setup() {
        this->bounce.attach(this->pin, INPUT_PULLUP);
        this->bounce.interval(this->interval);
        this->bounce.setPressedState(LOW); // because of INPUT_PULLUP
    }

    void update() {
        this->bounce.update();
    }

    bool hasButtonJustPressed() {
        return this->bounce.pressed();
    }

    bool hasButtonJustReleased() {
        return this->bounce.released();
    }

    bool isPressed() const {
        return bounce.isPressed();
    }

    void toggle(bool switchLight = true) {
        isSwitched = !isSwitched;

        if (isSwitched) {
            MIDI_::enableControlChange(channel, this->control);
            if (switchLight) digitalWrite(this->lightPin, HIGH);
        } else {
            MIDI_::disableControlChange(channel, this->control);
            if (switchLight) digitalWrite(this->lightPin, LOW);
        }
    }

    void enable(bool enableLight = true) const {
        MIDI_::enableControlChange(channel, this->control);
        if (enableLight) digitalWrite(this->lightPin, HIGH);
    }

    void disable(bool disableLight = true) const {
        MIDI_::disableControlChange(channel, this->control);
        if (disableLight) digitalWrite(this->lightPin, LOW);
    }

    void handleClick() {
        this->bounce.update();

        if (hasButtonJustPressed()) {

            switch (switchMode) {
                case Switch:
                    toggle();
                    break;
                case Momentary:
                    enable();
                    break;
            }
        } else if (hasButtonJustReleased()) {
            currentTimestamp = millis();

            switch (switchMode) {
                case Switch:
                    if (currentTimestamp - lastTimestamp < 350) {
                        switchMode = Momentary;
                        this->resetSwitched();
                    }
                    break;
                case Momentary:
                    this->disable();
                    if (currentTimestamp - lastTimestamp < 350) {
                        switchMode = Switch;
                    }
                    break;
            }
        }

        lastTimestamp = currentTimestamp;
    }

    void resetSwitched() {
        if (isSwitched) this->disable();
        isSwitched = false;
    }
};