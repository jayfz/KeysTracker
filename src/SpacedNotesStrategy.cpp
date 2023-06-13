#include "SpacedNotesStrategy.h"

std::array<double, 128> SpacedNotesStrategy::calculatePoints(KeyboardInfo info)
{

    std::array<double, 128> trackingPoints{0};

    constexpr double percentageOfBlackKeys = 0.46511627907;
    constexpr double percentageOfWhiteKeys = 1 - percentageOfBlackKeys;
    constexpr double percentageOfOneBlackKey = percentageOfBlackKeys / 5;
    constexpr double percentageOfOneWhiteKey = percentageOfWhiteKeys / 7;

    for (uint32_t octave = 0; octave < info.numberOfOctaves; octave += 1) {

        double startPixel = info.firstOctaveAt + (info.octaveWidth * octave);
        uint32_t baseNote = (octave * info.keysInAnOctave);

        double currentPosition = 10 + startPixel;

        trackingPoints[static_cast<int>(Key::C) + baseNote] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::Db) + baseNote] = currentPosition;

        currentPosition += percentageOfOneBlackKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::D) + baseNote] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::Eb) + baseNote] = currentPosition;

        currentPosition += percentageOfOneBlackKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::E) + baseNote] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::F) + baseNote] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::Gb) + baseNote] = currentPosition;

        currentPosition += percentageOfOneBlackKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::G) + baseNote] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::Ab) + baseNote] = currentPosition;

        currentPosition += percentageOfOneBlackKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::A) + baseNote] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::Bb) + baseNote] = currentPosition;

        currentPosition += percentageOfOneBlackKey * info.octaveWidth;
        trackingPoints[static_cast<int>(Key::B) + baseNote] = currentPosition;
    }

    return trackingPoints;
}