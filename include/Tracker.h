#ifndef TRACKER_H
#define TRACKER_H

#include "KeyboardInfo.h"
#include "PixelLine.h"
#include "TrackingPointStrategy.h"

#include <array>
#include <vector>

class Tracker
{
  public:
    Tracker(KeyboardInfo info, TrackingPointStrategy *strategy);
    virtual ~Tracker() = default;

    virtual std::vector<MidiKeysEvent>
    generateMidiEvents(const std::vector<PixelLine> &collection) = 0;
    void printColoredImage();

  protected:
    std::vector<std::tuple<uint8_t, bool>>
    getNoteOnEvents(const std::vector<uint8_t> &pixelLine);

    virtual std::pair<bool, bool>
    isThisANoteONEvent(const std::vector<uint8_t> &possibleNote, bool expectBemol) = 0;
    void cleanUpShortNotes(std::vector<MidiKeysEvent> &events, uint32_t stepSpan);
    void snapCloseNotesTogether(std::vector<MidiKeysEvent> &events, uint32_t tolerance);

    KeyboardInfo keyboardInfo;
    uint32_t bottomRangeOfNoteDetection = 5;
    uint32_t upperRangeOfNoteDetection = 5;
    std::array<double, 128> notesTrackingPoints;
    std::vector<PixelLine> linesColored;
    std::vector<PixelLine> linesTracked;

  private:
    void markShortNotes(std::vector<MidiKeysEvent> &events, uint32_t stepSpan);
};

#endif