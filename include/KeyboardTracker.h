#ifndef KEYBOARD_TRACKER_H
#define KEYBOARD_TRACKER_H

#include "Tracker.h"

class KeyboardTracker : public Tracker
{
  public:
    KeyboardTracker(KeyboardInfo info, TrackingPointStrategy *strategy,
                    uint32_t pickFramesDivisibleByNumber = 1);
    virtual std::vector<MidiKeysEvent>
    generateMidiEvents(const std::vector<PixelLine> &collection) override;

  protected:
    std::pair<bool, bool> isThisANoteONEvent(const std::vector<uint8_t> &possibleNote,
                                             bool expectBemol) override;
    uint32_t pickFramesDivisibleByNumber;
};
#endif
