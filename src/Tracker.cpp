#include "Tracker.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

Tracker::Tracker(KeyboardInfo info, TrackingPointStrategy *strategy) : keyboardInfo(info)
{
    this->notesTrackingPoints = strategy->calculatePoints(this->keyboardInfo);
};

void Tracker::printColoredImage()
{
    PixelLine::saveBigFormatFrame(this->linesColored, "debug");
    PixelLine::saveBigFormatFrame(this->linesTracked, "tracked");
}

std::vector<std::tuple<uint8_t, bool>>
Tracker::getNoteOnEvents(const std::vector<uint8_t> &pixelLine)
{
    static uint32_t linesProcessed = 0;
    std::vector<std::tuple<uint8_t, bool>> noteOnEvents;
    std::vector<uint8_t> possibleNote;

    possibleNote.reserve(
        (this->bottomRangeOfNoteDetection + this->upperRangeOfNoteDetection) * 3);

    this->linesColored.emplace_back(pixelLine.data(), pixelLine.size());
    this->linesTracked.emplace_back(pixelLine.data(), pixelLine.size());

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

        uint32_t debugColor = noteCheckResult.first ? 100 : 200;

        for (uint32_t lowerBoundStart = lowerBound; lowerBoundStart < upperBound;
             lowerBoundStart++) {
            this->linesTracked.back().at(lowerBoundStart) = debugColor;

            if (noteCheckResult.first || noteCheckResult.second) {
                this->linesColored.back().at(lowerBoundStart) = debugColor;
            }
        }

        if (noteCheckResult.first || noteCheckResult.second) {
            noteOnEvents.push_back({currentMidiNote, noteCheckResult.first});
        }
    }

    linesProcessed += 1;
    return noteOnEvents;
}

void Tracker::markShortNotes(std::vector<MidiKeysEvent> &events, uint32_t stepSpan)
{

    // key: note
    // value: tick, index,
    std::unordered_map<int, std::tuple<int, int>> temp;

    uint32_t index = 0;
    for (uint32_t i = 0; i < events.size(); i++) {
        auto event = events[i];
        int eventNote = event.isLeftEvent ? (((int)event.note) * -1) : event.note;
        auto it = temp.find(eventNote);

        if (it == temp.end()) {
            temp[eventNote] = {event.tick, index};
        } else {
            if (event.velocity > 0) {
                std::cout << "Ok, this should have not happened: " << std::endl;
                std::cout << "note: " << (int)event.note << std::endl;
                std::cout << "tick: " << event.tick << std::endl;
                std::cout << "isLeftEvent: " << event.isLeftEvent << std::endl;
                break;
            }

            uint32_t tempTick = std::get<0>(it->second);
            int tempIndex = std::get<1>(it->second);
            if (std::abs((int)tempTick - (int)event.tick) < stepSpan) {
                // 255 means to me, ignore these events, both on and off
                events[tempIndex].note = 255;
                events[i].note = 255;
            }

            temp.erase(eventNote);
        }

        index += 1;
    }
}

void Tracker::cleanUpShortNotes(std::vector<MidiKeysEvent> &events, uint32_t stepSpan)
{
    this->markShortNotes(events, stepSpan);

    for (auto it = events.begin(); it != events.end();) {
        if (it->note == 255)
            it = events.erase(it);
        else
            ++it;
    }
}

void Tracker::snapCloseNotesTogether(std::vector<MidiKeysEvent> &events,
                                     uint32_t tolerance)
{
    // tolerance 20!
    uint32_t previousTick = 0;

    for (auto &event : events) {

        uint32_t fixedTick = event.tick;

        if (std::abs((int)(event.tick) - (int)(previousTick)) <= tolerance) {
            fixedTick = previousTick;
        } else {
            previousTick = event.tick;
        }

        event.tick = fixedTick;
    }
}