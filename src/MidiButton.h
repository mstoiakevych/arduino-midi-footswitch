#include "Bounce2.h"
#include "SwitchMode.h"

class MidiButton {
private:
    Bounce2::Button bounce;
    byte pin;
    byte lightPin;
    byte channel;
    byte control;
    byte interval;
    bool isSwitched;
    SwitchMode switchMode;

    unsigned long lastTimestamp;
    unsigned long currentTimestamp;
public:
    MidiButton() = delete;

    MidiButton(byte pin, byte lightPin, byte channel, byte control, byte interval)
    : bounce(Bounce2::Button())
    , pin(pin)
    , lightPin(lightPin)
    , channel(channel)
    , control(control)
    , interval(interval)
    , isSwitched(false)
    , switchMode(SwitchMode::Switch)
    , lastTimestamp(millis())
    {
        pinMode(lightPin, OUTPUT);
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

    void toggle(bool switchLight = true) {
        isSwitched = !isSwitched;

        if (isSwitched) {
            MIDI_::enableControlChange(channel, control);
            if (switchLight) digitalWrite(lightPin, HIGH);
        } else {
            MIDI_::disableControlChange(channel, control);
            if (switchLight) digitalWrite(lightPin, LOW);
        }
    }

    void enable(bool enableLight = true) const {
        MIDI_::enableControlChange(channel, control);
        if (enableLight) digitalWrite(lightPin, HIGH);
    }

    void disable(bool disableLight = true) const {
        MIDI_::disableControlChange(channel, control);
        if (disableLight) digitalWrite(lightPin, LOW);
    }

    void resetSwitched() {
        if (isSwitched) disable(control);
        isSwitched = false;
    }

    void handlePress() {
        switch (switchMode) {
            case Switch:
                toggle(control);
                break;
            case Momentary:
                enable(control);
                break;
        }
    }

    void handleRelease() {
        currentTimestamp = millis();

        switch (switchMode) {
            case Switch:
                if (currentTimestamp - lastTimestamp < 350) {
                    switchMode = Momentary;
                    resetSwitched();
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

    void cancel() {
        switch (switchMode) {
            case Switch:
                if (isSwitched) disable(control);
                else enable(control);

                isSwitched = !isSwitched;

                break;
            case Momentary:
                disable(control);
                break;
        }
    }

    void reset(bool switchLight = true) {
        if (isSwitched) {
            if (switchLight) digitalWrite(lightPin, HIGH);
        } else {
            if (switchLight) digitalWrite(lightPin, LOW);
        }
    }
};