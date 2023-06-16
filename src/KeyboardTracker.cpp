#include "KeyboardTracker.h"
#include <iostream>

KeyboardTracker::KeyboardTracker(KeyboardInfo info, TrackingPointStrategy *strategy)
    : Tracker(info, strategy)
{
    this->bottomRangeOfNoteDetection = 3;
    this->upperRangeOfNoteDetection = 3;
}
std::vector<MidiKeysEvent>
KeyboardTracker::generateMidiEvents(const std::vector<PixelLine> &collection)
{

    std::vector<MidiKeysEvent> result;
    struct NoteOnEvent {
        uint8_t key;
        bool left; // was a left key pressed? no, then it was the right one
    };

    this->linesColored = collection;
    this->linesTracked = collection;

    uint32_t absoluteTick = 0;
    uint32_t tickStep = 8;
    std::vector<std::tuple<uint8_t, bool>> previousEvents;
    int numLinesTotal = 0;
    for (const auto &pixelLine : collection) {

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

            if (!foundOnPreviousEventList) {
                result.emplace_back(absoluteTick, key, 50, leftEvent);
            }
        }

        for (const auto &onEvent : previousEvents) {
            auto [key, leftEvent] = onEvent;

            bool foundOnLatestEventList = false;
            for (uint64_t ev = 0; ev < onEvents.size(); ev++) {
                if (std::get<0>(onEvents[ev]) == key) {
                    foundOnLatestEventList = true;
                    break;
                }
            }

            if (!foundOnLatestEventList) {
                result.emplace_back(absoluteTick, key, 0, leftEvent);
            }
        }

        numLinesTotal += 1;
        absoluteTick += tickStep;
        previousEvents = onEvents;

        std::cout << "Processed line #" << numLinesTotal << std::endl;
    }
    this->printColoredImage();

    return result;
}

std::pair<bool, bool>
KeyboardTracker::isThisANoteONEvent(const std::vector<uint8_t> &possibleNote,
                                    bool expectBemol)
{
    if (possibleNote.size() < 3) {
        return {false, false};
    }

    std::vector<RGBColor> rgbColors = RGBColor::fromBytes(possibleNote);
    RGBColor averageRGBColor = RGBColor::calculateAverage(rgbColors);

    bool isLeftHandKeyPressed = false;
    bool isRightHandKeyPressed = false;

    RGBColor leftHandColor =
        (expectBemol ? this->keyboardInfo.noteColors.getLeftHandBlackKeyColor()
                     : keyboardInfo.noteColors.getLeftHandWhiteKeyColor());
    RGBColor rightHandColor =
        (expectBemol ? this->keyboardInfo.noteColors.getRightHandBlackKeyColor()
                     : this->keyboardInfo.noteColors.getRightHandWhiteKeyColor());

    uint8_t tolerance = 36;

    isLeftHandKeyPressed =
        leftHandColor.isColorCloseEnoughToReference(averageRGBColor, tolerance);
    isRightHandKeyPressed =
        rightHandColor.isColorCloseEnoughToReference(averageRGBColor, tolerance);

    return {isLeftHandKeyPressed, isRightHandKeyPressed};
}
