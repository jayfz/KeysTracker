#include "SpacedNotesStrategy.h"
#include <iostream>
#include <sstream>

// deprecated?

SpacedNotesStrategy::SpacedNotesStrategy(std::string firstOctavePositions)
{

    std::istringstream positions(firstOctavePositions);
    std::string tempPosition;
    while (std::getline(positions, tempPosition, ',')) {
        this->firstOctavePositions.push_back(std::stoul(tempPosition));
    }

    if (firstOctavePositions.size() != 12) {
        std::cout << "SpacedNoteStrategy was not initialized correcy";
    }
}

std::array<double, 128> SpacedNotesStrategy::calculatePoints(KeyboardInfo info)
{

    std::array<double, 128> trackingPoints{0};

    for (auto &item : this->firstOctavePositions) {
        item -= info.firstOctaveAt;
    }

    for (uint32_t octave = 0; octave < info.numberOfOctaves; octave += 1) {

        double startPixel = info.firstOctaveAt + (info.octaveWidth * octave);
        uint32_t baseNote = (octave * info.keysInAnOctave);

        for (uint32_t key = 0; key < info.keysInAnOctave; key += 1) {

            uint32_t currentPosition = startPixel + this->firstOctavePositions[key];
            trackingPoints[key + baseNote] = currentPosition;
        }
    }

    return trackingPoints;
}