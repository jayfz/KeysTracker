#ifndef RGB_COLOR
#define RGB_COLOR

#include <cstdint>
#include <vector>
#include <string>

class RGBColor
{
public:
    RGBColor(uint8_t red, uint8_t green, uint8_t blue);
    static RGBColor calculateAverage(const std::vector<RGBColor> &colors);
    static bool areFirstTwoPixelsDifferentFromLastTwoPixels(const std::vector<RGBColor> &colors);
    static RGBColor fromString(std::string color);

    bool isColorCloseEnoughToReference(RGBColor other, bool beStrict) const;

private:
    uint8_t redChannel;
    uint8_t greenChannel;
    uint8_t blueChannel;

    static uint8_t stringHexToInt(const std::string &colorChannel);
};

#endif