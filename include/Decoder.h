#ifndef DECODER_H
#define DECODER_H

#include "FrameProcessor.h"
#include <fstream>
class Decoder
{

  public:
    Decoder(FrameProcessor *frameProcessor, const std::string &fileName)
        : frameProcessor(frameProcessor), inputFile(fileName, std::ios_base::binary)
    {
    }

    virtual ~Decoder() = default;

    virtual bool wasInitializedCorrectly() = 0;
    virtual void decode() = 0;

  protected:
    FrameProcessor *frameProcessor = nullptr;
    std::ifstream inputFile;
};

#endif