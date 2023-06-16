#ifndef IMAGE_DECODER_H
#define IMAGE_DECODER_H

#include "Decoder.h"
#include <cstdint>

class PPMImageDecoder : public Decoder
{

  public:
    PPMImageDecoder(FrameProcessor *processor, const std::string &fileName);
    ~PPMImageDecoder();

    bool wasInitializedCorrectly() override;
    void decode() override;

  private:
    std::pair<uint32_t, uint32_t> getWidthAndHeight(std::string line);
    uint32_t getDimension(std::istringstream &stream);
};

#endif