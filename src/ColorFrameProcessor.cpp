#include "ColorFrameProcessor.h"

ColorFrameProcessor::ColorFrameProcessor(uint32_t numLinesToExtract, uint32_t startLine)
    : numLinesToExtract(numLinesToExtract), startLine(startLine)
{

    this->lines.reserve(20000);
}
void ColorFrameProcessor::doProcessFrame(uint8_t *decodedFrame, uint32_t width,
                                         uint32_t height)
{

    if (this->startLine + numLinesToExtract > height) {
        return; // can not read safely
    }

    for (uint32_t line = this->numLinesToExtract; line > 0; line -= 1) {
        uint32_t bytesToReadFromLine = width * 3;
        uint32_t lineOffset = bytesToReadFromLine * (this->startLine + line);
        uint8_t *pointerToLine = decodedFrame + lineOffset;
        this->lines.emplace_back(pointerToLine, bytesToReadFromLine);
    }
}
