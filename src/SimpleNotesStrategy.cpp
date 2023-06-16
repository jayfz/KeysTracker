
#include "SimpleNotesStrategy.h"
#include <cmath>

std::array<double, 128> SimpleNotesStrategy::calculatePoints(KeyboardInfo info)
{

    std::array<double, 128> trackingPoints{0};

    double averageNoteWidth = info.octaveWidth / info.keysInAnOctave;
    double middle = averageNoteWidth / 2;

    for (uint32_t octave = 0; octave < info.numberOfOctaves; octave += 1) {

        double startPixel = info.firstOctaveAt + (info.octaveWidth * octave);
        uint32_t baseNote = (octave * info.keysInAnOctave);
        double currentPosition = 0;

        for (uint32_t key = 0; key < info.keysInAnOctave; key += 1) {

            currentPosition = std::ceil(startPixel + middle + (averageNoteWidth * key));
            trackingPoints[key + baseNote] = currentPosition;
        }
    }

    return trackingPoints;
}
