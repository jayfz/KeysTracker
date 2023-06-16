#include "Tracker.h"
#include <fstream>

Tracker::Tracker(KeyboardInfo info, TrackingPointStrategy *strategy) : keyboardInfo(info)
{
    this->notesTrackingPoints = strategy->calculatePoints(this->keyboardInfo);
};

void Tracker::printColoredImage()
{
    PixelLine::saveBigFormatFrame(this->linesColored, "debug");
}

std::vector<std::tuple<uint8_t, bool>>
Tracker::getNoteOnEvents(const std::vector<uint8_t> &pixelLine)
{
    static uint32_t linesProcessed = 0;
    std::vector<std::tuple<uint8_t, bool>> noteOnEvents;
    std::vector<uint8_t> possibleNote;
    possibleNote.reserve(
        (this->bottomRangeOfNoteDetection + this->upperRangeOfNoteDetection) * 3);

    for (uint8_t currentMidiNote = 0;
         currentMidiNote < this->keyboardInfo.numberOfTotalNotesInKeyboard;
         currentMidiNote++) {

        uint32_t lowerBound = (uint32_t)(this->notesTrackingPoints[currentMidiNote] -
                                         this->bottomRangeOfNoteDetection);
        uint32_t upperBound = (uint32_t)(this->notesTrackingPoints[currentMidiNote] +
                                         this->upperRangeOfNoteDetection);

        // absolute bounds in bytes
        lowerBound *= 3;
        upperBound *= 3;

        for (uint32_t lowerBoundStart = lowerBound; lowerBoundStart < upperBound;
             lowerBoundStart++) {
            possibleNote.push_back(pixelLine[lowerBoundStart]);
        }

        bool isBemol = KeyboardInfo::isBlackKey(currentMidiNote);
        auto noteCheckResult = this->isThisANoteONEvent(possibleNote, isBemol);
        possibleNote.clear();

        // if we detected a left hand note press or a right hand note press

        if (noteCheckResult.first || noteCheckResult.second) {

            uint32_t debugColor = noteCheckResult.first ? 100 : 200;

            for (uint32_t lowerBoundStart = lowerBound; lowerBoundStart < upperBound;
                 lowerBoundStart++) {
                this->linesColored[linesProcessed].at(lowerBoundStart) = debugColor;
            }

            noteOnEvents.push_back({currentMidiNote, noteCheckResult.first});
        }
    }

    linesProcessed += 1;
    return noteOnEvents;
}