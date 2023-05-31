#include <cmath>
#include <array>
#include <iostream>
#include "Keyboard.h"
#include "Utils.h"
#include <tuple>
#include <map>
#include <fstream>
#include <cstring>

Keyboard::Keyboard(uint32_t octaveWidthInPixels, uint32_t firstOctaveAt, std::array<RGB, 4> noteColors, TrackMode mode) : octaveWidthInPixels(octaveWidthInPixels), firstOctaveAt(firstOctaveAt), noteColors(noteColors), mode(mode)
{

    double whiteNoteWidth = static_cast<double>(this->octaveWidthInPixels / numOfWhiteKeysInOctave);

    if (this->mode == TrackMode::FallingNotes)
    {
        double whiteNoteHalfWidth = whiteNoteWidth / 2;

        notesHalfWidths[static_cast<int>(Key::C)] = whiteNoteHalfWidth;
        notesHalfWidths[static_cast<int>(Key::Db)] = whiteNoteWidth;
        notesHalfWidths[static_cast<int>(Key::D)] = whiteNoteWidth + whiteNoteHalfWidth;
        notesHalfWidths[static_cast<int>(Key::Eb)] = whiteNoteWidth * 2;
        notesHalfWidths[static_cast<int>(Key::E)] = (whiteNoteWidth * 2) + whiteNoteHalfWidth;
        notesHalfWidths[static_cast<int>(Key::F)] = (whiteNoteWidth * 3) + whiteNoteHalfWidth;
        notesHalfWidths[static_cast<int>(Key::Gb)] = whiteNoteWidth * 4;
        notesHalfWidths[static_cast<int>(Key::G)] = (whiteNoteWidth * 4) + whiteNoteHalfWidth;
        notesHalfWidths[static_cast<int>(Key::Ab)] = whiteNoteWidth * 5;
        notesHalfWidths[static_cast<int>(Key::A)] = (whiteNoteWidth * 5) + whiteNoteHalfWidth;
        notesHalfWidths[static_cast<int>(Key::Bb)] = whiteNoteWidth * 6;
        notesHalfWidths[static_cast<int>(Key::B)] = (whiteNoteWidth * 6) + whiteNoteHalfWidth;

        this->bottomRangeOfNoteDetection = 5;
        this->upperRangeOfNoteDetection = 5;
    }

    if (this->mode == TrackMode::TrackKeys)
    {
        constexpr double percentageOfBlackKeys = 0.46511627907;
        constexpr double percentageOfWhiteKeys = 1 - percentageOfBlackKeys;
        constexpr double percentageOfOneBlackKey = percentageOfBlackKeys / numOfBlackKeysInOctave;
        constexpr double percentageOfOneWhiteKey = percentageOfWhiteKeys / numOfWhiteKeysInOctave;
        uint8_t currentPosition = 5;

        notesHalfWidths[static_cast<int>(Key::C)] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::Db)] = currentPosition;

        currentPosition += percentageOfOneBlackKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::D)] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::Eb)] = currentPosition;

        currentPosition += percentageOfOneBlackKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::E)] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::F)] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::Gb)] = currentPosition;

        currentPosition += percentageOfOneBlackKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::G)] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::Ab)] = currentPosition;

        currentPosition += percentageOfOneBlackKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::A)] = currentPosition;

        currentPosition += percentageOfOneWhiteKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::Bb)] = currentPosition;

        currentPosition += percentageOfOneBlackKey * octaveWidthInPixels;
        notesHalfWidths[static_cast<int>(Key::B)] = currentPosition;

        this->bottomRangeOfNoteDetection = 0;
        this->upperRangeOfNoteDetection = 10;
    }
}

Keyboard::~Keyboard()
{
    for (auto &pointer : this->linesColored)
    {
        if (pointer)
        {
            delete[] pointer;
            pointer = nullptr;
        }
    }
}
// uint32_t Keyboard::getNote(uint32_t noteXPosition)
// {

//     // should give you a number between 21 and  108 (a full keyboard is 88 keys
//     // lowest C starts at 24

//     if (noteXPosition < firstOctaveAt)
//     {
//         return 0;
//     }

//     // a number between 1 and 8,9,10?
//     uint32_t octave = this->octaveNumber(noteXPosition);

//     // a number between 0 and 309 //310
//     uint32_t noteXInFunctionOfAnOctaveWidth = (noteXPosition - firstOctaveAt) % octaveWidthInPixels;

//     uint32_t key = static_cast<int>(Key::C);
//     bool foundKey = false;

//     for (; key <= static_cast<int>(Key::B); key++)
//     {
//         const double lowerBound = this->notesHalfWidths[key] - this->rangeOfNoteDetection;
//         const double upperBound = this->notesHalfWidths[key] + this->rangeOfNoteDetection;

//         if (noteXInFunctionOfAnOctaveWidth >= lowerBound && noteXInFunctionOfAnOctaveWidth <= upperBound)
//         {
//             foundKey = true;
//             break;
//         }
//     }

//     if (foundKey)
//     {
//         return 24 + (12 * octave) + key;
//     }

//     return 0;
// }

uint32_t Keyboard::octaveNumber(uint32_t noteXPosition)
{
    if (noteXPosition < firstOctaveAt)
    {
        return 0;
    }

    uint32_t octave = static_cast<uint32_t>(ceil((double)(noteXPosition - firstOctaveAt) / octaveWidthInPixels));

    octave -= 1; // make it 0 based;

    return octave;
}

std::pair<bool, bool> Keyboard::isThisANoteONEvent(const std::vector<uint8_t> &possibleNote, bool expectBemol)
{

    if (possibleNote.size() < 3)
    {
        return {false, false};
    }

    std::vector<RGB> rgbColors;

    for (uint64_t i = 0; i < possibleNote.size(); i += 3)
    {
        auto red = possibleNote[i];
        auto green = possibleNote[i + 1];
        auto blue = possibleNote[i + 2];

        rgbColors.push_back({red, green, blue});
    }

    RGB averageRGBColor;

    if (areFirstTwoPixelsDifferentFromLastTwoPixels(rgbColors) && this->mode == TrackMode::FallingNotes)
    {
        averageRGBColor = {0, 0, 0};
    }
    else
    {
        averageRGBColor = calculateAverageRGB(rgbColors);
    }

    int leftHand = 0;
    int rightHand = 0;
    bool isLeftHandKeyPressed = false;
    bool isRightHandKeyPressed = false;

    leftHand = static_cast<int>(expectBemol ? NoteColorIndex::LeftHandBlackKey : NoteColorIndex::LeftHandWhiteKey);
    rightHand = static_cast<int>(expectBemol ? NoteColorIndex::RightHandBlackKey : NoteColorIndex::RightHandWhiteKey);

    bool beStrict = this->mode == TrackMode::FallingNotes ? true : false;

    isLeftHandKeyPressed = isColorCloseEnoughToReferenceRGB(this->noteColors[leftHand], averageRGBColor, beStrict);

    isRightHandKeyPressed = isColorCloseEnoughToReferenceRGB(this->noteColors[rightHand], averageRGBColor, beStrict);

    return {isLeftHandKeyPressed, isRightHandKeyPressed};
}

uint32_t Keyboard::numOfOctavesInFrame(uint32_t frameLength)
{

    // not ceil!
    uint32_t howManyOctaves = ((double)(frameLength - this->firstOctaveAt) / this->octaveWidthInPixels);

    if ((howManyOctaves * octaveWidthInPixels) > frameLength)
    {
        return howManyOctaves - 1;
    }

    return howManyOctaves;
}

std::vector<std::tuple<uint8_t, bool>> Keyboard::getNoteOnEvents(const std::vector<uint8_t> &pixelLine)
{
    static int lineAnalyzed = 1;
    auto numOctaves = this->numOfOctavesInFrame(pixelLine.size() / 3);
    std::vector<std::tuple<uint8_t, bool>> noteOnEvents;
    std::vector<uint8_t> possibleNote;
    possibleNote.reserve((this->bottomRangeOfNoteDetection + this->upperRangeOfNoteDetection) * 3);
    // uint32_t temp = 0;

    auto pointerToLine = new uint8_t[pixelLine.size()];
    this->linesColored.push_back(pointerToLine);
    std::memcpy(pointerToLine, pixelLine.data(), pixelLine.size());

    for (uint32_t currentOctave = 0; currentOctave < numOctaves; currentOctave++)
    {
        int note = static_cast<int>(Key::C);

        for (; note <= static_cast<int>(Key::B); note++)
        {
            // temp = 24 + (12 * currentOctave) + note;
            //  relative bounds
            uint32_t lowerBound = static_cast<uint32_t>(this->notesHalfWidths[note] - this->bottomRangeOfNoteDetection);
            uint32_t upperBound = static_cast<uint32_t>(this->notesHalfWidths[note] + this->upperRangeOfNoteDetection);

            // absolute bounds in pixels
            lowerBound += this->firstOctaveAt + (currentOctave * this->octaveWidthInPixels);
            upperBound += this->firstOctaveAt + (currentOctave * this->octaveWidthInPixels);

            // absolute bounds in bytes
            lowerBound *= 3;
            upperBound *= 3;

            for (uint32_t lowerBoundStart = lowerBound; lowerBoundStart < upperBound; lowerBoundStart++)
            {
                possibleNote.push_back(pixelLine[lowerBoundStart]);
            }

            std::pair<bool, bool> noteCheckResult{false, false};

            bool isBemol = Keyboard::isBlackKey(static_cast<Key>(note));
            noteCheckResult = this->isThisANoteONEvent(possibleNote, isBemol);
            possibleNote.clear();

            // if we detected a left hand note press or a right hand note press

            if (noteCheckResult.first || noteCheckResult.second)
            {
                for (uint32_t lowerBoundStart = lowerBound; lowerBoundStart < upperBound; lowerBoundStart++)
                {
                    pointerToLine[lowerBoundStart] = 125;
                }

                uint32_t currentMidiNote = 24 + (12 * currentOctave) + note;
                noteOnEvents.push_back({currentMidiNote, noteCheckResult.first});
            }
        }
    }

    lineAnalyzed += 1;

    return noteOnEvents;
}

bool Keyboard::isBlackKey(Key note)
{
    switch (note)
    {
    case Key::Db:
    case Key::Eb:
    case Key::Gb:
    case Key::Ab:
    case Key::Bb:
        return true;
    default:
        return false;
    }
}

void Keyboard::generateMidiEvents(const std::vector<std::unique_ptr<RawFrame>> &collection, std::vector<MidiKeyboardEvent> &events)
{
    switch (this->mode)
    {
    case TrackMode::TrackKeys:
        this->generateMidiEventsByTrackingKeys(collection, events);
        break;
    case TrackMode::FallingNotes:
        this->generateMidiEventsByTrackingFallingNotes(collection, events);
        break;
    }
}

void Keyboard::generateMidiEventsByTrackingFallingNotes(const std::vector<std::unique_ptr<RawFrame>> &collection, std::vector<MidiKeyboardEvent> &events)
{

    struct NoteOnEvent
    {
        uint8_t key;
        uint8_t freshness;
        bool left; // was a left key pressed? no, then it was the right one
    };

    uint32_t absoluteTick = 0;
    std::vector<uint8_t> pixelLine;
    std::vector<uint8_t> notesToRemoveFromMap;
    std::map<uint8_t, NoteOnEvent> myMap;
    class std::vector<std::tuple<uint8_t, bool>> previousEvents;

    // runs on every frame
    int numLinesTotal = 0;
    for (const auto &frame : collection)
    {
        pixelLine.reserve(frame->getWidth() * 3);

        // runs on every line
        for (int line = (frame->getHeight() - 1); line >= 0; line--)
        {
            numLinesTotal += 1;
            // runs for every byte in the line
            for (uint32_t byte = 0; byte < frame->getWidth() * 3; byte++)
            {
                auto colorByte = (frame->getData() + (frame->getWidth() * 3 * line) + byte);

                pixelLine.push_back(*colorByte);
            }

            auto onEvents = this->getNoteOnEvents(pixelLine);
            pixelLine.clear();

            for (const auto &onEvent : onEvents)
            {

                auto [key, leftEvent] = onEvent;

                bool foundOnPreviousEventList = false;
                for (uint64_t ev = 0; ev < previousEvents.size(); ev++)
                {
                    if (std::get<0>(previousEvents[ev]) == key)
                    {
                        foundOnPreviousEventList = true;
                        break;
                    }
                }

                if (foundOnPreviousEventList)
                {
                    auto it = myMap.find(key);

                    // if I find find the key in my status vector
                    if (it != myMap.end())
                    {
                        if (it->second.freshness == 1)
                            it->second.freshness += 1;
                    }
                    else
                    {
                        NoteOnEvent newkey{key, 32, leftEvent};
                        myMap.insert({key, newkey});

                        MidiKeyboardEvent mEvent{absoluteTick, key, 50, leftEvent};
                        events.push_back(mEvent);
                    }
                }
            }

            for (auto &mapElement : myMap)
            {
                myMap[mapElement.first].freshness -= 1;

                if (mapElement.second.freshness == 0)
                {
                    MidiKeyboardEvent mEvent{absoluteTick, mapElement.second.key, 0, mapElement.second.left};

                    events.push_back(mEvent);
                    notesToRemoveFromMap.push_back(mapElement.first);
                }
            }

            for (auto key : notesToRemoveFromMap)
            {
                myMap.erase(key);
            }

            notesToRemoveFromMap.clear();
            absoluteTick += 1;
            previousEvents = onEvents;
        }

        std::cout << "Processed lines in frame #" << frame->getFrameNumber() << std::endl;
    }

    this->printColoredImage();
}

void Keyboard::generateMidiEventsByTrackingKeys(const std::vector<std::unique_ptr<RawFrame>> &collection, std::vector<MidiKeyboardEvent> &events)
{
    struct NoteOnEvent
    {
        uint8_t key;
        bool left; // was a left key pressed? no, then it was the right one
    };

    uint32_t absoluteTick = 0;
    uint32_t tickStep = this->mode == TrackMode::FallingNotes ? 1 : 8;
    std::vector<uint8_t> pixelLine;
    class std::vector<std::tuple<uint8_t, bool>> previousEvents;

    // runs on every frame
    int numLinesTotal = 0;
    for (const auto &frame : collection)
    {
        pixelLine.reserve(frame->getWidth() * 3);

        // runs on every line
        for (int line = (frame->getHeight() - 1); line >= 0; line--)
        {
            numLinesTotal += 1;
            // runs for every byte in the line
            for (uint32_t byte = 0; byte < frame->getWidth() * 3; byte++)
            {
                auto colorByte = (frame->getData() + (frame->getWidth() * 3 * line) + byte);

                pixelLine.push_back(*colorByte);
            }

            auto onEvents = this->getNoteOnEvents(pixelLine);
            pixelLine.clear();

            for (const auto &onEvent : onEvents)
            {

                auto [key, leftEvent] = onEvent;

                bool foundOnPreviousEventList = false;
                for (uint64_t ev = 0; ev < previousEvents.size(); ev++)
                {
                    if (std::get<0>(previousEvents[ev]) == key)
                    {
                        foundOnPreviousEventList = true;
                        break;
                    }
                }

                if (!foundOnPreviousEventList)
                {
                    events.emplace_back(absoluteTick, key, 50, leftEvent);
                }
            }

            for (const auto &onEvent : previousEvents)
            {
                auto [key, leftEvent] = onEvent;

                bool foundOnLatestEventList = false;
                for (uint64_t ev = 0; ev < onEvents.size(); ev++)
                {
                    if (std::get<0>(onEvents[ev]) == key)
                    {
                        foundOnLatestEventList = true;
                        break;
                    }
                }

                if (!foundOnLatestEventList)
                {
                    events.emplace_back(absoluteTick, key, 0, leftEvent);
                }
            }

            absoluteTick += tickStep;
            previousEvents = onEvents;
        }

        std::cout << "Processed lines in frame #" << frame->getFrameNumber() << std::endl;
    }

    this->printColoredImage();
}

void Keyboard::printColoredImage()
{

    if (this->linesColored.size() == 0)
    {
        return; // nothing to do
    }
    else
    {
        std::cout << linesColored.size() << " size of lines colored" << std::endl;
    }

    // auto width = 1920;
    auto height = 1;

    std::string outFileName = "./raw-frames/colored-frame.ppm";

    std::ofstream outputFrameFile(outFileName, std::ofstream::binary);

    outputFrameFile << "P6\n"
                    << RawFrame::width << " " << height * this->linesColored.size() << "\n"
                    << 255 << "\n";

    for (auto it = this->linesColored.rbegin(); it != this->linesColored.rend(); it++)
    {
        const auto &rawFrame = *it;
        outputFrameFile.write(reinterpret_cast<char *>(rawFrame), RawFrame::width * height * 3);
    }

    outputFrameFile.flush();
}

void Keyboard::markShortNotes(std::vector<MidiKeyboardEvent> &events)
{
    // key: key
    // value: tick, index,
    std::map<uint8_t, std::tuple<int, int>> temp;

    uint32_t index = 0;
    // for (auto &event : events)
    for (uint32_t i = 0; i < events.size(); i++)
    {
        auto event = events[i];
        auto it = temp.find(event.key);

        if (it == temp.end())
        {
            temp[event.key] = {event.tick, index};
        }
        else
        {
            if (event.velocity > 0)
            {
                std::cout << "Ok, this should have not happened" << std::endl;
                break;
            }

            int tempTick = std::get<0>(it->second);
            int tempIndex = std::get<1>(it->second);
            if (std::abs(static_cast<int32_t>(tempTick) - static_cast<int32_t>(event.tick) < 2))
            {
                // 255 means to me, ignore these events, both on and off
                events[tempIndex].key = 255;
                event.key = 255;
            }

            temp.erase(event.key);
        }

        index += 1;
    }
}

MidiKeyboardEvent::MidiKeyboardEvent(uint32_t tick, uint8_t key, uint8_t velocity, bool left) : tick(tick), key(key), velocity(velocity), left(left) {}