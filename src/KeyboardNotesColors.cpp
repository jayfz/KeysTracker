#include "KeyboardNotesColors.h"

KeyboardNotesColors::KeyboardNotesColors(char *leftHandWhiteKey,
                                         char *leftHandBlackKey,
                                         char *rightHandWhitekKey,
                                         char *rightHandBlackkKey) : leftHandWhiteKey(RGBColor::fromString(leftHandWhiteKey)),
                                                                     leftHandBlackKey(RGBColor::fromString(leftHandBlackKey)),
                                                                     rightHandWhitekKey(RGBColor::fromString(rightHandWhitekKey)),
                                                                     rightHandBlackkKey(RGBColor::fromString(rightHandBlackkKey))
{
}

RGBColor KeyboardNotesColors::getLeftHandWhiteKeyColor() const
{
    return this->leftHandWhiteKey;
}
RGBColor KeyboardNotesColors::getLeftHandBlackKeyColor() const
{
    return this->leftHandBlackKey;
}
RGBColor KeyboardNotesColors::getRightHandWhiteKeyColor() const
{
    return this->rightHandWhitekKey;
}
RGBColor KeyboardNotesColors::getRightHandBlackKeyColor() const
{
    return this->rightHandBlackkKey;
}