#include "FallingNotesTracker.h"
#include <iostream>
#include <unordered_map>

FallingNotesTracker::FallingNotesTracker(KeyboardInfo info,
                                         TrackingPointStrategy *strategy)
    : Tracker(info, strategy)
{

    this->bottomRangeOfNoteDetection = 5;
    this->upperRangeOfNoteDetection = 5;
};

std::vector<MidiKeysEvent>
FallingNotesTracker::generateMidiEvents(const std::vector<PixelLine> &collection)
{

    struct NoteOnEvent {
        uint8_t key;
        uint8_t freshness;
        bool left;
    };

    std::vector<MidiKeysEvent> events;

    if (collection.empty()) {
        return events;
    }

    this->linesColored = collection;

    uint32_t absoluteTick = 0;
    uint32_t numLinesTotal = 0;
    std::vector<uint8_t> notesToRemoveFromMap;
    std::unordered_map<uint8_t, NoteOnEvent> myMap;
    std::vector<std::tuple<uint8_t, bool>> previousEvents;

    for (const auto &pixelLine : collection) {
        numLinesTotal += 1;

        auto onEvents = this->getNoteOnEvents(pixelLine.getLine());

        for (const auto &onEvent : onEvents) {

            auto [key, leftEvent] = onEvent;

            bool foundOnPreviousEventList = false;
            for (uint64_t ev = 0; ev < previousEvents.size(); ev++) {
                if (std::get<0>(previousEvents[ev]) == key) {
                    foundOnPreviousEventList = true;
                    break;
                }
            }

            if (foundOnPreviousEventList) {
                auto it = myMap.find(key);

                // if I find find the key in my status vector
                if (it != myMap.end()) {
                    if (it->second.freshness == 1)
                        it->second.freshness += 1;
                } else {
                    NoteOnEvent newkey{key, 32, leftEvent};
                    myMap.insert({key, newkey});

                    MidiKeysEvent mEvent{absoluteTick, key, 50, leftEvent};
                    events.push_back(mEvent);
                }
            }
        }

        for (auto &mapElement : myMap) {
            myMap[mapElement.first].freshness -= 1;

            if (mapElement.second.freshness == 0) {
                MidiKeysEvent mEvent{absoluteTick, mapElement.second.key, 0,
                                     mapElement.second.left};

                events.push_back(mEvent);
                notesToRemoveFromMap.push_back(mapElement.first);
            }
        }

        for (auto key : notesToRemoveFromMap) {
            myMap.erase(key);
        }

        notesToRemoveFromMap.clear();
        absoluteTick += 1;
        previousEvents = onEvents;

        std::cout << "Processed line #" << numLinesTotal << std::endl;
    }

    markShortNotes(events);
    this->printColoredImage();
    return events;
}

std::pair<bool, bool>
FallingNotesTracker::isThisANoteONEvent(const std::vector<uint8_t> &possibleNote,
                                        bool expectBemol)
{

    if (possibleNote.size() < 3) {
        return {false, false};
    }

    std::vector<RGBColor> rgbColors = RGBColor::fromBytes(possibleNote);
    RGBColor averageRGBColor{0, 0, 0};

    if (!RGBColor::areFirstTwoPixelsDifferentFromLastTwoPixels(rgbColors)) {
        averageRGBColor = RGBColor::calculateAverage(rgbColors);
    }

    bool isLeftHandKeyPressed = false;
    bool isRightHandKeyPressed = false;

    RGBColor leftHandColor =
        (expectBemol ? keyboardInfo.noteColors.getLeftHandBlackKeyColor()
                     : keyboardInfo.noteColors.getLeftHandWhiteKeyColor());
    RGBColor rightHandColor =
        (expectBemol ? keyboardInfo.noteColors.getRightHandBlackKeyColor()
                     : keyboardInfo.noteColors.getRightHandWhiteKeyColor());

    uint8_t tolerance = 24;

    isLeftHandKeyPressed =
        leftHandColor.isColorCloseEnoughToReference(averageRGBColor, tolerance);
    isRightHandKeyPressed =
        rightHandColor.isColorCloseEnoughToReference(averageRGBColor, tolerance);

    return {isLeftHandKeyPressed, isRightHandKeyPressed};
}
// snap notes close to each other
void FallingNotesTracker::markShortNotes(std::vector<MidiKeysEvent> &events)
{
    uint32_t previousTick = 0;

    for (auto &event : events) {

        uint32_t fixedTick = event.tick;

        // note has been marked as short already, so forget about it
        if (event.note == 255)
            continue;

        if (std::abs(static_cast<int32_t>(event.tick) -
                     static_cast<int32_t>(previousTick)) <= 20) {
            fixedTick = previousTick;
        } else {
            previousTick = event.tick;
        }

        event.tick = fixedTick;
    }
}
