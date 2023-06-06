#include "RawFrame.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

uint32_t RawFrame::width = 0;
uint32_t RawFrame::height = 0;
uint32_t RawFrame::copyFromYCoordsPercentage = 0;

RawFrame::RawFrame(uint32_t frameNumber, uint32_t originalWidth, uint32_t originalHeight, uint8_t *decodedFrame) : frameNumber(frameNumber)
{
    this->data = new uint8_t[this->getFrameSize()];
    uint32_t line = ((originalHeight * RawFrame::copyFromYCoordsPercentage) / 100);
    size_t copyStartingFrom = (originalWidth * 3) * line;
    std::memcpy(this->data, decodedFrame + copyStartingFrom, this->getFrameSize());
}
RawFrame::~RawFrame()
{
    if (data)
    {
        delete[] data;
        data = nullptr;
    }
}

uint8_t *RawFrame::getData() const
{
    return data;
}
uint32_t RawFrame::getWidth() const
{
    return width;
}
uint32_t RawFrame::getHeight() const
{
    return height;
}
uint16_t RawFrame::getFrameNumber() const
{
    return frameNumber;
}

void RawFrame::save() const
{
    std::string outFileName = "./raw-frames/frame-" + std::to_string(this->frameNumber) + ".ppm";
    std::ofstream outputFrameFile(outFileName, std::ofstream::binary);

    outputFrameFile << "P6\n"
                    << this->getWidth() << " " << this->getHeight() << "\n"
                    << 255 << "\n";

    outputFrameFile.write(reinterpret_cast<char *>(this->data), this->getFrameSize());
}

std::size_t RawFrame::getFrameSize() const
{
    return width * height * 3;
}

void RawFrame::saveBigFormatFrame(const std::vector<std::unique_ptr<RawFrame>> &collection)
{
    if (collection.empty())
    {
        return; // nothing to do
    }

    auto width = collection.at(0)->getWidth();
    auto height = collection.at(0)->getHeight();

    std::string outFileName = "./raw-frames/final-frame.ppm";

    std::ofstream outputFrameFile(outFileName, std::ofstream::binary);

    outputFrameFile << "P6\n"
                    << width << " " << height * collection.size() << "\n"
                    << 255 << "\n";

    for (auto it = collection.rbegin(); it != collection.rend(); it++)
    {
        const auto &rawFrame = *it;
        outputFrameFile.write(reinterpret_cast<char *>(rawFrame->getData()), rawFrame->getFrameSize());
    }

    outputFrameFile.flush();
}
