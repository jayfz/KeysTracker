#include "PixelLine.h"
#include <fstream>
#include <iostream>
#include <string>

PixelLine::PixelLine(uint8_t *line, uint32_t bytesToRead)
{
    this->line.reserve(bytesToRead);
    std::copy(line, line + bytesToRead, std::back_inserter(this->line));
}

void PixelLine::saveBigFormatFrame(const std::vector<PixelLine> &lines,
                                   const std::string &outFileName)
{
    if (lines.empty()) {
        return;
    }

    uint32_t width = lines.at(0).getLine().size() / 3;
    std::string fileName = getPathForBigFormatFrame(outFileName);
    std::ofstream outputFrameFile(fileName, std::ofstream::binary);

    outputFrameFile << "P6\n" << width << " " << lines.size() << "\n" << 255 << "\n";

    for (auto it = lines.rbegin(); it != lines.rend(); it++) {
        const auto &pixelLine = *it;
        outputFrameFile.write(reinterpret_cast<const char *>(pixelLine.getLine().data()),
                              pixelLine.getLine().size());
    }
}

const std::vector<uint8_t> &PixelLine::getLine() const { return this->line; }
uint8_t &PixelLine::at(uint64_t index) { return this->line[index]; }

std::string PixelLine::getPathForBigFormatFrame(const std::string &name)
{
    return "./raw-frames/" + name + ".ppm";
}