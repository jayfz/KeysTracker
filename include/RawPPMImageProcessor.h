#ifndef RAW_PPM_IMAGE_PROCESSOR_H
#define RAW_PPM_IMAGE_PROCESSOR_H

#include "FrameProcessor.h"

class RawPPMImageProcessor : public FrameProcessor
{
  public:
    virtual void doProcessFrame(uint8_t *decodedFrame, uint32_t width,
                                uint32_t height) override;
};

#endif