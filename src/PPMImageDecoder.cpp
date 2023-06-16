#include "PPMImageDecoder.h"
#include <fstream>
#include <iostream>
#include <sstream>

PPMImageDecoder::PPMImageDecoder(FrameProcessor *processor, const std::string &fileName)
    : Decoder(processor, fileName)
{
}
PPMImageDecoder::~PPMImageDecoder() {}

bool PPMImageDecoder::wasInitializedCorrectly() { return !inputFile.is_open(); }
void PPMImageDecoder::decode()
{

    std::string line;
    std::getline(this->inputFile, line);

    if (line != "P6")
        return;

    std::getline(this->inputFile, line);
    auto [width, height] = this->getWidthAndHeight(line);

    std::getline(this->inputFile, line);

    if (line != "255")
        return;

    uint64_t imageSizeInBytes = width * height * 3;
    uint8_t *buffer = new uint8_t[imageSizeInBytes];

    this->inputFile.read(reinterpret_cast<char *>(buffer), imageSizeInBytes);
    this->frameProcessor->doProcessFrame(buffer, width, height);

    delete[] buffer;
}

std::pair<uint32_t, uint32_t> PPMImageDecoder::getWidthAndHeight(std::string line)
{

    std::istringstream dimensions(line);
    uint32_t width = this->getDimension(dimensions);
    uint32_t height = this->getDimension(dimensions);
    return std::make_pair(width, height);
}

uint32_t PPMImageDecoder::getDimension(std::istringstream &stream)
{
    std::string dimension;
    std::getline(stream, dimension, ' ');
    return std::stoul(dimension);
}
