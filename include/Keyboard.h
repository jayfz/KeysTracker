#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>
#include <vector>
#include <array>
#include <tuple>
#include <memory>

#include "RawFrame.h"
#include "RGBColor.h"
#include "KeyboardNotesColors.h"

struct MidiKeyboardEvent
{
    uint32_t tick;
    uint8_t key;
    uint8_t velocity;
    bool left;
    MidiKeyboardEvent(uint32_t tick, uint8_t key, uint8_t velocity, bool left);
};

enum class TrackMode
{
    TrackKeys,
    FallingNotes
};

class Keyboard
{
public:
    Keyboard(double octaveWidthInPixels, uint32_t firstOctaveAt, KeyboardNotesColors noteColors, TrackMode mode);
    ~Keyboard();
    uint32_t getNote(uint32_t width);
    std::vector<std::tuple<uint8_t, bool>> getNoteOnEvents(const std::vector<uint8_t> &pixelLine);
    std::pair<bool, bool> isThisANoteONEvent(const std::vector<uint8_t> &possibleNote, bool expectBemol);
    uint32_t numOfOctavesInFrame(uint32_t frameLength);
    void generateMidiEvents(const std::vector<std::unique_ptr<RawFrame>> &collection, std::vector<MidiKeyboardEvent> &events);
    void printColoredImage();

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

    static bool isBlackKey(Key note);
    static void markShortNotes(std::vector<MidiKeyboardEvent> &events);

private:
    double octaveWidthInPixels;
    uint32_t firstOctaveAt;
    KeyboardNotesColors noteColors;
    TrackMode mode;
    std::array<double, 12> notesHalfWidths;
    uint32_t bottomRangeOfNoteDetection;
    uint32_t upperRangeOfNoteDetection;
    static const uint32_t numOfWhiteKeysInOctave = 7;
    static const uint32_t numOfBlackKeysInOctave = 5;

    void generateMidiEventsByTrackingKeys(const std::vector<std::unique_ptr<RawFrame>> &collection, std::vector<MidiKeyboardEvent> &events);
    void generateMidiEventsByTrackingFallingNotes(const std::vector<std::unique_ptr<RawFrame>> &collection, std::vector<MidiKeyboardEvent> &events);

    // debug
    std::vector<uint8_t *> linesColored;
};
#endif