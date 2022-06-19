#include "Bounce2.h"
#include "SwitchMode.h"
#include "LcdScreen.h"

class MidiButton {
private:
    Bounce2::Button bounce;
    byte buttonNum;
    byte pin;
    byte lightPin;
    byte channel;
    byte control;
    byte interval;
    bool isSwitched;
    SwitchMode switchMode;
    LcdScreen& screen;

    unsigned long lastTimestamp;
    unsigned long currentTimestamp;
public:
    MidiButton() = delete;

    MidiButton(byte buttonNum, byte pin, byte lightPin, byte channel, byte control, byte interval, LcdScreen& screen)
    : bounce(Bounce2::Button())
    , buttonNum(buttonNum)
    , pin(pin)
    , lightPin(lightPin)
    , channel(channel)
    , control(control)
    , interval(interval)
    , isSwitched(false)
    , switchMode(SwitchMode::Switch)
    , screen(screen)
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

        if (isSwitched) enable(switchLight); else disable(switchLight);
    }

    void enable(bool enableLight = true) {
        isSwitched = true;
        MIDI_::enableControlChange(channel, control);
        if (enableLight) digitalWrite(lightPin, HIGH);
    }

    void disable(bool disableLight = true) {
        isSwitched = false;
        MIDI_::disableControlChange(channel, control);
        if (disableLight) digitalWrite(lightPin, LOW);
    }

    void handlePress() {
        switch (switchMode) {
            case Switch:
                break;
            case Momentary:
                enable(control);
                screen.setButtonState(buttonNum, switchMode, isSwitched);
                break;
        }
    }

    void handleRelease() {
        currentTimestamp = millis();

        switch (switchMode) {
            case Switch:
                toggle(control);
                screen.setButtonState(buttonNum, switchMode, isSwitched);

                if (currentTimestamp - lastTimestamp < 350) {
                    switchMode = Momentary;
                    disable();
                    screen.setButtonState(buttonNum, switchMode, false);
                    break;
                }
                break;
            case Momentary:
                disable();
                if (currentTimestamp - lastTimestamp < 350) {
                    switchMode = Switch;
                    screen.setButtonState(buttonNum, switchMode, isSwitched);
                }
                break;
        }

        lastTimestamp = currentTimestamp;
    }

    void cancel() {
        if (switchMode == SwitchMode::Momentary) disable(control);
    }

    void reset(bool switchLight = true) const {
        if (isSwitched) {
            if (switchLight) digitalWrite(lightPin, HIGH);
        } else {
            if (switchLight) digitalWrite(lightPin, LOW);
        }

        screen.setButtonState(buttonNum, switchMode, isSwitched);
    }
};