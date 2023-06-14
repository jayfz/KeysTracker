#ifndef TRACKING_POINT_STRATEGY_H
#define TRACKING_POINT_STRATEGY_H

#include "KeyboardInfo.h"
#include <array>

class TrackingPointStrategy
{
  public:
    virtual ~TrackingPointStrategy() = default;
    virtual std::array<double, 128> calculatePoints(KeyboardInfo info) = 0;
};
#endif