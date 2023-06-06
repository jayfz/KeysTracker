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
    if (argc != 10)
    {
        std::cout << "Incorrect usage. " + std::string(argv[1]) << std::endl;
        return EXIT_FAILURE;
    }

    std::string h264FileName = argv[1];
    TrackMode trackMode = std::string(argv[2]) == "keys" ? TrackMode::TrackKeys : TrackMode::FallingNotes;
    int firstOctaveStartsAt = std::stoi(argv[3]); // 187;
    int octavesLength = std::stoi(argv[4]);       // 310;
    int numOfOctaves = std::stoi(argv[5]);

    RawFrame::height = (trackMode == TrackMode::TrackKeys) ? 1 : 8;
    uint8_t yCoordsPercentage = (trackMode == TrackMode::TrackKeys) ? 80 : 50;

    std::array<RGB, 4> noteColors;

    constexpr auto lhwk = static_cast<int>(Keyboard::NoteColorIndex::LeftHandWhiteKey);
    constexpr auto lhbk = static_cast<int>(Keyboard::NoteColorIndex::LeftHandBlackKey);
    constexpr auto rhwk = static_cast<int>(Keyboard::NoteColorIndex::RightHandWhiteKey);
    constexpr auto rhbk = static_cast<int>(Keyboard::NoteColorIndex::RightHandBlackKey);

    noteColors[lhwk] = stringToRGB(argv[6]);
    noteColors[lhbk] = stringToRGB(argv[7]);
    noteColors[rhwk] = stringToRGB(argv[8]);
    noteColors[rhbk] = stringToRGB(argv[9]);

    double octaveWidthInPixels = static_cast<double>(octavesLength) / numOfOctaves;
    Keyboard keyboard(octaveWidthInPixels, firstOctaveStartsAt, noteColors, trackMode);
    ManagedMidiFile midiFile("./didThisAllPayOff.mid");
    H264Decoder decoder(h264FileName, 3);

    if (decoder.wasInitializedCorrectly())
    {

        decoder.decode(yCoordsPercentage);
        std::vector<MidiKeyboardEvent> events;

        keyboard.generateMidiEvents(decoder.getFrameCollection(), events);
        // mark short notes for deletion
        if (trackMode == TrackMode::FallingNotes)
        {
            Keyboard::markShortNotes(events);

            uint32_t previousTick = 0;

            // snap notes close to each other and add them to the correct list (left and right)
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
        }
        if (trackMode == TrackMode::TrackKeys)
        {
            for (const auto &event : events)
            {
                if (event.left)
                    midiFile.addLeftHandEvent(event.tick, event.key + 0, event.velocity);
                else
                    midiFile.addRightHandEvent(event.tick, event.key + 0, event.velocity);
            }
        }

        midiFile.save();
    }

    return EXIT_SUCCESS;
};