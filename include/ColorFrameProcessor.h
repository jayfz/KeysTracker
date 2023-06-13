#ifndef COLOR_FRAME_PROCESSOR_H
#define COLOR_FRAME_PROCESSOR_H

#include "FrameProcessor.h"

class ColorFrameProcessor : public FrameProcessor
{
  public:
    ColorFrameProcessor(uint32_t numLinesToExtract, uint32_t startLine);
    void doProcessFrame(uint8_t *decodedFrame, uint32_t width, uint32_t height) override;

  private:
    uint32_t numLinesToExtract;
    uint32_t startLine;
};
#endif