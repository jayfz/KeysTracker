#include <iostream>
#include <iomanip>
#include <MidiFile.h>
#include <vector>
#include <MidiEvent.h>
#include "H264Decoder.h"
#include "Utils.h"
#include "Keyboard.h"

int main()
{

    int octaveLength = 310;
    int firstOctaveStartsAt = 187;

    std::array<RGB, 4> noteColors;

    constexpr auto lhwk = static_cast<int>(Keyboard::NoteColorIndex::LeftHandWhiteKey);
    constexpr auto lhbk = static_cast<int>(Keyboard::NoteColorIndex::LeftHandBlackKey);
    constexpr auto rhwk = static_cast<int>(Keyboard::NoteColorIndex::RightHandWhiteKey);
    constexpr auto rhbk = static_cast<int>(Keyboard::NoteColorIndex::RightHandBlackKey);

    noteColors[lhwk] = {151, 116, 160}; //#9774a0
    noteColors[lhbk] = {103, 80, 109};  //#67506d
    // noteColors[rhwk] = {185, 146, 89};  //#b99259
    noteColors[rhwk] = {177, 144, 94}; //#b99259
    noteColors[rhbk] = {177, 109, 48}; //#b16d30

    Keyboard keyboard(octaveLength, firstOctaveStartsAt, noteColors);
    ManagedMidiFile midiFile("./didThisAllPayOff.mid");

    H264Decoder decoder("./test.h264", 3);

    if (decoder.wasInitializedCorrectly())
    {
        decoder.decode(keyboard, midiFile);
    }

    return EXIT_SUCCESS;
};