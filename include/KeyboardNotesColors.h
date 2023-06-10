#ifndef KEYBOARD_NOTES_COLORS_H
#define KEYBOARD_NOTES_COLORS_H

#include "RGBColor.h"
#include <string>
class KeyboardNotesColors
{

public:
    KeyboardNotesColors(std::string leftHandWhiteKey,
                        std::string leftHandBlackKey,
                        std::string rightHandWhitekKey,
                        std::string rightHandBlackkKey);

    RGBColor getLeftHandWhiteKeyColor() const;
    RGBColor getLeftHandBlackKeyColor() const;
    RGBColor getRightHandWhiteKeyColor() const;
    RGBColor getRightHandBlackKeyColor() const;

private:
    RGBColor leftHandWhiteKey;
    RGBColor leftHandBlackKey;
    RGBColor rightHandWhiteKey;
    RGBColor rightHandBlackKey;
};

#endif