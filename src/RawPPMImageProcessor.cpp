#include "RawPPMImageProcessor.h"
#include <algorithm>

void RawPPMImageProcessor::doProcessFrame(uint8_t *decodedFrame, uint32_t width,
                                          uint32_t height)
{

    uint32_t bytesToReadPerLine = width * 3;
    uint32_t bytesReadSoFar = 0;

    for (uint32_t start = 0; start < height; start++) {
        this->lines.emplace_back(decodedFrame + bytesReadSoFar, bytesToReadPerLine);
        bytesReadSoFar += bytesToReadPerLine;
    }

    std::reverse(this->lines.begin(), this->lines.end());
}
