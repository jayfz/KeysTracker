#include "OverlappingNotesStrategy.h"

std::array<double, 128> OverlappingNotesStrategy::calculatePoints(KeyboardInfo info)
{

    std::array<double, 128> trackingPoints{0};
    double whiteNoteHalfWidth = info.whiteNoteWidth / 2;

    for (uint32_t octave = 0; octave < info.numberOfOctaves; octave += 1) {

        uint32_t startPixel = info.firstOctaveAt + (info.octaveWidth * octave);
        double baseNote = (octave * info.keysInAnOctave);

        trackingPoints[static_cast<int>(Key::C) + baseNote] =
            startPixel + whiteNoteHalfWidth;
        trackingPoints[static_cast<int>(Key::Db) + baseNote] =
            startPixel + whiteNoteHalfWidth + (info.blackNoteWidth / 2);
        trackingPoints[static_cast<int>(Key::D) + baseNote] =
            startPixel + info.whiteNoteWidth + whiteNoteHalfWidth;
        trackingPoints[static_cast<int>(Key::Eb) + baseNote] =
            startPixel +
            ((info.whiteNoteWidth * 2) + whiteNoteHalfWidth - (info.blackNoteWidth / 2));
        trackingPoints[static_cast<int>(Key::E) + baseNote] =
            startPixel + (info.whiteNoteWidth * 2) + whiteNoteHalfWidth;
        trackingPoints[static_cast<int>(Key::F) + baseNote] =
            startPixel + (info.whiteNoteWidth * 3) + whiteNoteHalfWidth;
        trackingPoints[static_cast<int>(Key::Gb) + baseNote] =
            startPixel +
            ((info.whiteNoteWidth * 3) + whiteNoteHalfWidth + (info.blackNoteWidth / 2));
        trackingPoints[static_cast<int>(Key::G) + baseNote] =
            startPixel + (info.whiteNoteWidth * 4) + whiteNoteHalfWidth;
        trackingPoints[static_cast<int>(Key::Ab) + baseNote] =
            startPixel + info.whiteNoteWidth * 5;
        trackingPoints[static_cast<int>(Key::A) + baseNote] =
            startPixel + (info.whiteNoteWidth * 5) + whiteNoteHalfWidth;
        trackingPoints[static_cast<int>(Key::Bb) + baseNote] =
            startPixel +
            ((info.whiteNoteWidth * 6) + whiteNoteHalfWidth - (info.blackNoteWidth / 2));
        trackingPoints[static_cast<int>(Key::B) + baseNote] =
            startPixel + (info.whiteNoteWidth * 6) + whiteNoteHalfWidth;
    }

    return trackingPoints;
}
