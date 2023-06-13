#ifndef OVERLAPPING_NOTES_STRATEGY_H
#define OVERLAPPING_NOTES_STRATEGY_H

#include "TrackingPointStrategy.h"

class OverlappingNotesStrategy : public TrackingPointStrategy
{
  public:
    virtual std::array<double, 128> calculatePoints(KeyboardInfo info) override;
};

#endif