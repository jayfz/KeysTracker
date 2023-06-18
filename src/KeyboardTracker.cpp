#include "KeyboardTracker.h"
#include <iostream>

KeyboardTracker::KeyboardTracker(KeyboardInfo info, TrackingPointStrategy *strategy,
                                 uint32_t pickFramesDivisibleByNumber)
    : Tracker(info, strategy), pickFramesDivisibleByNumber(pickFramesDivisibleByNumber)
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
        bool left;
    };

    uint32_t absoluteTick = 0;
    uint32_t tickStep = 8 * this->pickFramesDivisibleByNumber;
    std::vector<std::tuple<uint8_t, bool>> previousEvents;
    int numLinesTotal = 0;

    for (uint32_t index = 0; index < collection.size(); index += 1) {

        if (index % this->pickFramesDivisibleByNumber != 0) {
            continue;
        }
        auto onEvents = this->getNoteOnEvents(collection[index].getLine());

        for (const auto &onEvent : onEvents) {

            auto [key, leftEvent] = onEvent;

            bool foundOnPreviousEventList = false;
            for (uint64_t ev = 0; ev < previousEvents.size(); ev++) {
                if (std::get<0>(previousEvents[ev]) == key &&
                    std::get<1>(previousEvents[ev]) == leftEvent) {
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
                if (std::get<0>(onEvents[ev]) == key &&
                    std::get<1>(onEvents[ev]) == leftEvent) {
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
        // std::cout << "Processed line #" << numLinesTotal << std::endl;
    }

    this->cleanUpShortNotes(result, tickStep * 2);
    // this->snapCloseNotesTogether(result, 20);
    this->printColoredImage();
    std::cout << "finished..." << std::endl;

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

    double tolerance = 0.2; // needs to be a parameter, old was 0.15

    isLeftHandKeyPressed = leftHandColor.isColorProportionsCloseEnoughToReference(
        averageRGBColor, tolerance);
    isRightHandKeyPressed = rightHandColor.isColorProportionsCloseEnoughToReference(
        averageRGBColor, tolerance);

    if (isLeftHandKeyPressed && isRightHandKeyPressed) {
        std::cout << "both pressed! " << std::endl;
        return {false, false};
    }

    return {isLeftHandKeyPressed, isRightHandKeyPressed};
}
