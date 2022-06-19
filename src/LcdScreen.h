#include "../.pio/libdeps/leonardo/LiquidCrystal/src/LiquidCrystal.h"
#include "LcdMidiSymbols.h"

enum ScreenMode {
    Start, Main
};

class LcdScreen {
private:
    LiquidCrystal lcd;
    ScreenMode screenMode;
    int startScreenTime;

    void drawButtonState(SwitchMode mode, bool isPressed) {
        if (mode == SwitchMode::Momentary) lcd.write(buttonMomentaryId);
        else lcd.write(isPressed ? buttonOnId : buttonOffId);
    }

    void startScreen() {
        lcd.clear();

        lcd.setCursor(2, 0);
        lcd.print("IZITONE MIDI");

        lcd.setCursor(3, 1);
        lcd.print("FOOTSWITCH");

        lcd.setCursor(0, 0);
        lcd.write(noteId);

        lcd.setCursor(15, 0);
        lcd.write(noteId);

        lcd.setCursor(1, 1);
        lcd.write(noteId);

        lcd.setCursor(14, 1);
        lcd.write(noteId);
    }

    void presetScreen() {
        lcd.clear();

        lcd.setCursor(4, 0);
        lcd.print("Preset");
        setPreset(1);

        lcd.setCursor(1, 1);
        lcd.print(1);
        setButtonState(1, SwitchMode::Switch, false);

        lcd.setCursor(5, 1);
        lcd.print(2);
        setButtonState(2, SwitchMode::Switch, false);

        lcd.setCursor(9, 1);
        lcd.print(3);
        setButtonState(3, SwitchMode::Switch, false);

        lcd.setCursor(13, 1);
        lcd.print(4);
        setButtonState(4, SwitchMode::Switch, false);
    }

public:
    LcdScreen(byte rsPin, byte enPin, byte d4Pin, byte d5Pin, byte d6Pin, byte d7Pin, int startScreenTime)
    : lcd(LiquidCrystal(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin))
    , screenMode(ScreenMode::Start)
    , startScreenTime(startScreenTime)
    {
    }

    void setup() {
        lcd.begin(16, 2);

        lcd.createChar(noteId, note);
        lcd.createChar(buttonOnId, buttonOn);
        lcd.createChar(buttonOffId, buttonOff);
        lcd.createChar(buttonMomentaryId, buttonMomentary);
    }

    void start() {
        startScreen();
        delay(startScreenTime);

        screenMode = ScreenMode::Main;
        presetScreen();
    }

    void setButtonState(byte buttonNum, SwitchMode mode, bool isPressed) {
        if (screenMode != ScreenMode::Main) return;

        switch (buttonNum) {
            case 1: lcd.setCursor(2, 1); break;
            case 2: lcd.setCursor(6, 1); break;
            case 3: lcd.setCursor(10, 1); break;
            case 4: lcd.setCursor(14, 1); break;
            default: return;
        }

        drawButtonState(mode, isPressed);
    }

    void setPreset(byte presetNum) {
        lcd.setCursor(11, 0);
        lcd.print(presetNum);
    }
};

