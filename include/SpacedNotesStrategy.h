#ifndef SPACED_NOTES_STRATEGY_H
#define SPACED_NOTES_STRATEGY_H

#include "TrackingPointStrategy.h"

class SpacedNotesStrategy : public TrackingPointStrategy
{

  public:
    std::array<double, 128> calculatePoints(KeyboardInfo info) override;
};
#endif
