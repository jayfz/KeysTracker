#ifndef RAW_FRAME_H
#define RAW_FRAME_H

#include <cstdint>
#include <memory>
#include <vector>

class RawFrame
{

public:
    RawFrame(uint32_t frameNumber);
    RawFrame(const RawFrame &other) = delete;
    RawFrame &operator=(const RawFrame &other) = delete;
    ~RawFrame();

    uint8_t *getData() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    uint16_t getFrameNumber() const;

    void save() const;
    void copyData(const uint8_t *buffer);
    static void saveBigFormatFrame(const std::vector<std::unique_ptr<RawFrame>> &collection);

    static uint32_t width;
    static uint32_t height;

private:
    uint32_t frameNumber;
    uint8_t *data = nullptr;
    std::size_t getFrameSize() const;
};

#endif