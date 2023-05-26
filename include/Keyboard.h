#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>
#include <vector>
#include <array>
#include <tuple>
#include <memory>

#include "RawFrame.h"
#include "Utils.h"

struct MidiKeyboardEvent
{
    uint32_t tick;
    uint8_t key;
    uint8_t velocity;
    bool left;
};

class Keyboard
{
public:
    Keyboard(uint32_t octaveWidthInPixels, uint32_t firstOctaveAt, std::array<RGB, 4> noteColors);
    ~Keyboard();
    uint32_t getNote(uint32_t width);
    std::vector<std::tuple<uint8_t, bool>> getNoteOnEvents(const std::vector<uint8_t> &pixelLine);

    std::pair<bool, bool> isThisANoteONEvent(const std::vector<uint8_t> &possibleNote, bool expectBemol);

    uint32_t numOfOctavesInFrame(uint32_t frameLength);
    void generateMidiEvents(const std::vector<std::unique_ptr<RawFrame>> &collection, std::vector<MidiKeyboardEvent> &events);
    void printColoredImage();

    static void markShortNotes(std::vector<MidiKeyboardEvent> &events);

    enum class Key
    {
        C,
        Db,
        D,
        Eb,
        E,
        F,
        Gb,
        G,
        Ab,
        A,
        Bb,
        B
    };

    enum class NoteColorIndex
    {
        LeftHandWhiteKey,
        LeftHandBlackKey,
        RightHandWhiteKey,
        RightHandBlackKey
    };

private:
    uint32_t octaveWidthInPixels;
    uint32_t firstOctaveAt;
    std::array<RGB, 4> noteColors;
    std::array<double, 12> notesHalfWidths;
    static const uint32_t rangeOfNoteDetection = 5;
    static const uint32_t numOfWhiteKeysInOctave = 7;
    uint32_t octaveNumber(uint32_t width);

    // debug
    std::vector<uint8_t *> linesColored;
};
#endif