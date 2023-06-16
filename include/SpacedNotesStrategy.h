#ifndef SPACED_NOTES_STRATEGY_H
#define SPACED_NOTES_STRATEGY_H

#include "TrackingPointStrategy.h"

class SpacedNotesStrategy : public TrackingPointStrategy
{
  public:
    SpacedNotesStrategy(std::string firstOctavePositions);
    std::array<double, 128> calculatePoints(KeyboardInfo info) override;

  private:
    std::vector<uint32_t> firstOctavePositions;
};
#endif
