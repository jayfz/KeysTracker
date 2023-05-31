#include <iostream>
#include <iomanip>
#include <MidiFile.h>
#include <vector>
#include <MidiEvent.h>
#include "H264Decoder.h"
#include "Utils.h"
#include "Keyboard.h"
#include "ManagedMidiFile.h"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cout << "Incorrect usage. Only pass a single h264 file as input and mode (easy or hard)" << std::endl;
        return EXIT_FAILURE;
    }

    std::string h264FileName = argv[1];
    TrackMode trackMode = std::string(argv[2]) == "keys" ? TrackMode::TrackKeys : TrackMode::FallingNotes;
    int octaveLength = std::stoi(argv[3]);        // 310;
    int firstOctaveStartsAt = std::stoi(argv[4]); // 187;

    RawFrame::height = (trackMode == TrackMode::TrackKeys) ? 1 : 8;
    uint8_t yCoordsPercentage = (trackMode == TrackMode::TrackKeys) ? 85 : 50;

    std::array<RGB, 4> noteColors;

    constexpr auto lhwk = static_cast<int>(Keyboard::NoteColorIndex::LeftHandWhiteKey);
    constexpr auto lhbk = static_cast<int>(Keyboard::NoteColorIndex::LeftHandBlackKey);
    constexpr auto rhwk = static_cast<int>(Keyboard::NoteColorIndex::RightHandWhiteKey);
    constexpr auto rhbk = static_cast<int>(Keyboard::NoteColorIndex::RightHandBlackKey);

    noteColors[lhwk] = {151, 116, 160}; //#9774a0
    noteColors[lhbk] = {103, 80, 109};  //#67506d
    noteColors[rhwk] = {177, 144, 94};  //#b99259
    noteColors[rhbk] = {177, 109, 48};  //#b16d30

    // noteColors[lhwk] = {188, 87, 99};
    // noteColors[lhbk] = {188, 87, 99};
    // noteColors[rhwk] = {136, 195, 86};
    // noteColors[rhbk] = {92, 155, 42};

    Keyboard keyboard(octaveLength, firstOctaveStartsAt, noteColors, trackMode);
    ManagedMidiFile midiFile("./didThisAllPayOff.mid");
    H264Decoder decoder(h264FileName, 3);

    if (decoder.wasInitializedCorrectly())
    {

        decoder.decode(yCoordsPercentage);
        std::vector<MidiKeyboardEvent> events;

        keyboard.generateMidiEvents(decoder.getFrameCollection(), events);

        if (trackMode == TrackMode::FallingNotes)
            Keyboard::markShortNotes(events);

        uint32_t previousTick = 0;

        for (uint32_t i = 0; i < events.size(); i++)
        {
            MidiKeyboardEvent event = events[i];
            uint32_t fixedTick = event.tick;

            // note has been marked as short, so forget about it
            if (event.key == 255)
                continue;

            if (std::abs(static_cast<int32_t>(event.tick) - static_cast<int32_t>(previousTick)) <= 20)
            {
                fixedTick = previousTick;
            }
            else
            {
                previousTick = event.tick;
            }

            if (event.left)
                midiFile.addLeftHandEvent(fixedTick, event.key + 0, event.velocity);
            else
                midiFile.addRightHandEvent(fixedTick, event.key + 0, event.velocity);
        }

        midiFile.save();
    }

    return EXIT_SUCCESS;
};