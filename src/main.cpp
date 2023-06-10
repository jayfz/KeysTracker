#include <iostream>
#include <iomanip>
#include <MidiFile.h>
#include <vector>
#include <MidiEvent.h>
#include "H264Decoder.h"
#include "RGBColor.h"
#include "KeyboardNotesColors.h"
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
    double octavesLength = std::stod(argv[4]);    // 310;
    int numOfOctaves = std::stoi(argv[5]);

    if (trackMode == TrackMode::TrackKeys)
    {
        RawFrame::height = 1;
        RawFrame::copyFromYCoordsPercentage = 75;
        // RawFrame::copyFromYCoordsPercentage = 85;
    }

    if (trackMode == TrackMode::FallingNotes)
    {
        RawFrame::height = 8;
        RawFrame::copyFromYCoordsPercentage = 50;
    }

    KeyboardNotesColors noteColors(argv[6], argv[7], argv[8], argv[9]);
    double octaveWidthInPixels = octavesLength / numOfOctaves;
    Keyboard keyboard(octaveWidthInPixels, firstOctaveStartsAt, noteColors, trackMode);
    ManagedMidiFile midiFile("./didThisAllPayOff.mid");
    H264Decoder decoder(h264FileName, 30);

    if (decoder.wasInitializedCorrectly())
    {

        decoder.decode();
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