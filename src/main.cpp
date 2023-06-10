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
#include "ArgumentParser.h"
#include "ProgramOptions.h"

int main(int argc, char *argv[])
{
    ProgramOptions cliOptions(ArgumentParser(argc, argv));

    if (!cliOptions.areValid())
    {
        cliOptions.reportErrors();
        return EXIT_FAILURE;
    }

    KeyboardNotesColors noteColors(
        cliOptions.leftHandWhiteKeyColor,
        cliOptions.leftHandBlackKeyColor,
        cliOptions.rightHandWhiteKeyColor,
        cliOptions.rightHandBlackKeyColor);

    RawFrame::height = cliOptions.rawFrameHeight;
    RawFrame::width = cliOptions.rawFrameWidth;
    RawFrame::copyFromLine = cliOptions.rawFrameCopyFromLine;

    TrackMode trackMode = TrackMode::FallingNotes;

    if (cliOptions.trackMode == "falling-notes")
    {
        trackMode = TrackMode::FallingNotes;
    }

    if (cliOptions.trackMode == "keys")
    {
        trackMode = TrackMode::TrackKeys;
    }

    double octaveWidthInPixels = cliOptions.octavesLength / cliOptions.numOfOctaves;
    H264Decoder decoder(cliOptions.videoStreamFileName, cliOptions.numberOfFramesToSkip);

    if (decoder.wasInitializedCorrectly())
    {
        decoder.decode();
        std::vector<MidiKeyboardEvent> events;
        Keyboard keyboard(octaveWidthInPixels, cliOptions.firstOctaveAt, noteColors, trackMode);
        keyboard.generateMidiEvents(decoder.getFrameCollection(), events);
        ManagedMidiFile midiFile(cliOptions.outFileName);

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