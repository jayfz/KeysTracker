#ifndef FALLING_NOTES_TRACKER_H
#define FALLING_NOTES_TRACKER_H

#include "Tracker.h"

class FallingNotesTracker : public Tracker
{
  public:
    FallingNotesTracker(KeyboardInfo info, TrackingPointStrategy *strategy);

    virtual std::vector<MidiKeysEvent>
    generateMidiEvents(const std::vector<PixelLine> &collection) override;

  protected:
    std::pair<bool, bool> isThisANoteONEvent(const std::vector<uint8_t> &possibleNote,
                                             bool expectBemol) override;
};
#endif