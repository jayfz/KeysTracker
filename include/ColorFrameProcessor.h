#ifndef COLOR_FRAME_PROCESSOR_H
#define COLOR_FRAME_PROCESSOR_H

#include "FrameProcessor.h"

class ColorFrameProcessor : public FrameProcessor
{
  public:
    virtual ~ColorFrameProcessor() = default;
    ColorFrameProcessor(uint32_t numLinesToExtract, uint32_t startLine);
    virtual void doProcessFrame(uint8_t *decodedFrame, uint32_t width,
                                uint32_t height) override;

  private:
    uint32_t numLinesToExtract;
    uint32_t startLine;
};
#endif