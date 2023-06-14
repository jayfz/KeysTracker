#ifndef FRAME_PROCESSOR_H
#define FRAME_PROCESSOR_H

#include "PixelLine.h"
#include <cstdint>
#include <vector>
class FrameProcessor
{
  public:
    FrameProcessor() = default;
    virtual ~FrameProcessor() = default;
    virtual void doProcessFrame(uint8_t *decodedFrame, uint32_t width,
                                uint32_t height) = 0;
    std::vector<PixelLine> &getLines() { return this->lines; }

  protected:
    std::vector<PixelLine> lines;
};
#endif