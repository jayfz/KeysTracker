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
    std::string h264FileName;

    if (argc != 2)
    {
        std::cout << "Incorrect usage. Only pass a single h264 file as input." << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        h264FileName = argv[1];
    }

    int octaveLength = 310;
    int firstOctaveStartsAt = 187;

    std::array<RGB, 4> noteColors;

    constexpr auto lhwk = static_cast<int>(Keyboard::NoteColorIndex::LeftHandWhiteKey);
    constexpr auto lhbk = static_cast<int>(Keyboard::NoteColorIndex::LeftHandBlackKey);
    constexpr auto rhwk = static_cast<int>(Keyboard::NoteColorIndex::RightHandWhiteKey);
    constexpr auto rhbk = static_cast<int>(Keyboard::NoteColorIndex::RightHandBlackKey);

    noteColors[lhwk] = {151, 116, 160}; //#9774a0
    noteColors[lhbk] = {103, 80, 109};  //#67506d
    noteColors[rhwk] = {177, 144, 94};  //#b99259
    noteColors[rhbk] = {177, 109, 48};  //#b16d30

    Keyboard keyboard(octaveLength, firstOctaveStartsAt, noteColors);
    ManagedMidiFile midiFile("./didThisAllPayOff.mid");
    H264Decoder decoder(h264FileName, 3);

    if (decoder.wasInitializedCorrectly())
    {
        RawFrame::height = 8;
        uint8_t yCoordsPercentage = 50;
        decoder.decode(yCoordsPercentage);
        std::vector<MidiKeyboardEvent> events;

        keyboard.generateMidiEvents(decoder.getFrameCollection(), events);
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
                midiFile.addLeftHandEvent(fixedTick, event.key + 12, event.velocity);
            else
                midiFile.addRightHandEvent(fixedTick, event.key + 12, event.velocity);
        }

        midiFile.save();
    }

    return EXIT_SUCCESS;
};