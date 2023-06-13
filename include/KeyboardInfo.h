#ifndef KEYBOARD_INFO_H
#define KEYBOARD_INFO_H

#include "KeyboardNotesColors.h"
#include <cstdint>

struct KeyboardInfo {

    static uint8_t whiteKeysInOctave;
    static uint8_t blackKeysInOctave;
    static double blackNoteWhiteNoteRatio;
    static uint8_t keysInAnOctave;

    uint32_t fullWidth;
    uint32_t numberOfOctaves;
    uint32_t firstOctaveAt;
    KeyboardNotesColors noteColors;

    uint8_t numberOfTotalNotesInKeyboard;
    double octaveWidth;
    double whiteNoteWidth;
    double blackNoteWidth;

    KeyboardInfo(uint32_t fullWidth, uint32_t numberOfOctaves, uint32_t firstOctaveAt,
                 KeyboardNotesColors noteColors);

    static bool isBlackKey(uint8_t note);
};

enum class Key { C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B };

struct MidiKeysEvent {
    uint32_t tick;
    uint8_t note;
    uint8_t velocity;
    bool isLeftEvent;
    MidiKeysEvent(uint32_t tick, uint8_t note, uint8_t velocity, bool isLeftEvent);
};

#endif