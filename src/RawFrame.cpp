#include "RawFrame.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

RawFrame::RawFrame(uint16_t width, uint16_t height, uint32_t frameNumber) : width(width), height(height), frameNumber(frameNumber), data(nullptr) {}
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
uint16_t RawFrame::getWidth() const
{
    return width;
}
uint16_t RawFrame::getHeight() const
{
    return height;
}
uint16_t RawFrame::getFrameNumber() const
{
    return frameNumber;
}

void RawFrame::save()
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

void RawFrame::copyData(const uint8_t *buffer, size_t startingFrom)
{
    if (this->data)
        return;

    this->data = new uint8_t[this->getFrameSize()];
    std::memcpy(this->data, buffer + startingFrom, this->getFrameSize());
}

void RawFrame::saveBigFormatFrame(const std::vector<std::unique_ptr<RawFrame>> &collection)
{
    if (collection.size() <= 0)
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

