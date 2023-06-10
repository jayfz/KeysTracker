#ifndef KEYBOARD_NOTES_COLORS_H
#define KEYBOARD_NOTES_COLORS_H

#include "RGBColor.h"
class KeyboardNotesColors
{

public:
    KeyboardNotesColors(char *leftHandWhiteKey,
                        char *leftHandBlackKey,
                        char *rightHandWhitekKey,
                        char *rightHandBlackkKey);

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