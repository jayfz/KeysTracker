#ifndef PIXEL_LINE_H
#define PIXEL_LINE_H

#include <cstdint>
#include <memory>
#include <vector>

class PixelLine
{

  public:
    PixelLine(uint8_t *line, uint32_t bytesToRead);
    const std::vector<uint8_t> &getLine() const;
    uint8_t &at(uint64_t index);
    static void saveBigFormatFrame(const std::vector<PixelLine> &lines,
                                   const std::string &outFileName);
    static std::string getPathForBigFormatFrame(const std::string &name);

  private:
    std::vector<uint8_t> line;
};

#endif
