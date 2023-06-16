#ifndef SIMPLE_NOTES_STRATEGY_H
#define SIMPLE_NOTES_STRATEGY_H

#include "TrackingPointStrategy.h"

class SimpleNotesStrategy : public TrackingPointStrategy
{
  public:
    SimpleNotesStrategy() = default;
    std::array<double, 128> calculatePoints(KeyboardInfo info) override;
};
#endif
