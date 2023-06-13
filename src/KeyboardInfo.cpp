#include "KeyboardInfo.h"

uint8_t KeyboardInfo::whiteKeysInOctave = 7;
uint8_t KeyboardInfo::blackKeysInOctave = 5;
double KeyboardInfo::blackNoteWhiteNoteRatio = 5.0 / 7.0;
uint8_t KeyboardInfo::keysInAnOctave = 12;

KeyboardInfo::KeyboardInfo(uint32_t fullWidth, uint32_t numberOfOctaves,
                           uint32_t firstOctaveAt, KeyboardNotesColors noteColors)
    : fullWidth(fullWidth),
      numberOfOctaves(numberOfOctaves),
      firstOctaveAt(firstOctaveAt),
      noteColors(noteColors)
{
    this->numberOfTotalNotesInKeyboard =
        this->numberOfOctaves * (this->whiteKeysInOctave + this->blackKeysInOctave);
    this->octaveWidth = this->fullWidth / this->numberOfOctaves;
    this->whiteNoteWidth = this->octaveWidth / this->whiteKeysInOctave;
    this->blackNoteWidth = this->whiteNoteWidth * this->blackNoteWhiteNoteRatio;
}

bool KeyboardInfo::isBlackKey(uint8_t note)
{

    Key key = static_cast<Key>(note % 12);

    switch (key) {
    case Key::Db:
    case Key::Eb:
    case Key::Gb:
    case Key::Ab:
    case Key::Bb:
        return true;
    default:
        return false;
    }
}

MidiKeysEvent::MidiKeysEvent(uint32_t tick, uint8_t note, uint8_t velocity,
                             bool isLeftEvent)
    : tick(tick), note(note), velocity(velocity), isLeftEvent(isLeftEvent)
{
}
