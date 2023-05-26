#ifndef RAW_FRAME_H
#define RAW_FRAME_H

#include <cstdint>
#include <memory>
#include <vector>

class RawFrame
{

public:
    RawFrame(uint16_t width, uint16_t height, uint32_t frameNumber);
    RawFrame(const RawFrame &other) = delete;
    RawFrame &operator=(const RawFrame &other) = delete;
    ~RawFrame();
    void save();

    uint8_t *getData() const;
    uint16_t getWidth() const;
    uint16_t getHeight() const;
    uint16_t getFrameNumber() const;
    void copyData(const uint8_t *buffer, size_t startingFrom = 0);
    static void saveBigFormatFrame(const std::vector<std::unique_ptr<RawFrame>> &collection);

private:
    uint16_t width;
    uint16_t height;
    uint32_t frameNumber;
    uint8_t *data;
    std::size_t getFrameSize() const;
};

#endif