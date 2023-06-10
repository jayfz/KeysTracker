#include "KeyboardNotesColors.h"

KeyboardNotesColors::KeyboardNotesColors(char *leftHandWhiteKey,
                                         char *leftHandBlackKey,
                                         char *rightHandWhiteKey,
                                         char *rightHandBlackKey) : leftHandWhiteKey(RGBColor::fromString(leftHandWhiteKey)),
                                                                    leftHandBlackKey(RGBColor::fromString(leftHandBlackKey)),
                                                                    rightHandWhiteKey(RGBColor::fromString(rightHandWhiteKey)),
                                                                    rightHandBlackKey(RGBColor::fromString(rightHandBlackKey))
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
    return this->rightHandWhiteKey;
}
RGBColor KeyboardNotesColors::getRightHandBlackKeyColor() const
{
    return this->rightHandBlackKey;
}