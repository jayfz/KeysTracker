#include "RGBColor.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

RGBColor::RGBColor(uint8_t red, uint8_t green, uint8_t blue)
    : redChannel(red), greenChannel(green), blueChannel(blue)
{
}

RGBColor RGBColor::calculateAverage(const std::vector<RGBColor> &colors)
{

    // run of the mill average calculator if above does not pan out;
    double redAccumualtor = 0;
    double greenAccumualtor = 0;
    double blueAccumualtor = 0;

    for (auto color : colors) {
        redAccumualtor += color.redChannel;
        greenAccumualtor += color.greenChannel;
        blueAccumualtor += color.blueChannel;
    }

    redAccumualtor = std::round(redAccumualtor / colors.size());
    greenAccumualtor = std::round(greenAccumualtor / colors.size());
    blueAccumualtor = std::round(blueAccumualtor / colors.size());

    return {static_cast<uint8_t>(redAccumualtor), static_cast<uint8_t>(greenAccumualtor),
            static_cast<uint8_t>(blueAccumualtor)};
}
bool RGBColor::areFirstTwoPixelsDifferentFromLastTwoPixels(
    const std::vector<RGBColor> &colors)
{

    // do some special checks for earlier return;
    if (colors.size() >= 4) {
        RGBColor color0 = colors[0];
        RGBColor color1 = colors[1];
        RGBColor colorLast = colors[colors.size() - 1];
        RGBColor colorLastMinusOne = colors[colors.size() - 2];

        RGBColor averageFirst = calculateAverage({color0, color1});
        RGBColor averageLast = calculateAverage({colorLast, colorLastMinusOne});

        bool redFromExtremesAreQuiteDifferent = false;
        bool greenFromExtremesAreQuiteDifferent = false;
        bool blueFromExtremesAreQuiteDifferent = false;

        if (std::abs(averageFirst.redChannel - averageLast.redChannel) >= 40) // 24
            redFromExtremesAreQuiteDifferent = true;

        if (std::abs(averageFirst.greenChannel - averageLast.greenChannel) >= 40) // 24
            greenFromExtremesAreQuiteDifferent = true;

        if (std::abs(averageFirst.blueChannel - averageLast.blueChannel) >= 40) // 24
            blueFromExtremesAreQuiteDifferent = true;

        bool startAndEndAreDifferentColors = redFromExtremesAreQuiteDifferent ||
                                             greenFromExtremesAreQuiteDifferent ||
                                             blueFromExtremesAreQuiteDifferent;

        if (startAndEndAreDifferentColors) {
            return true;
        }
    }

    return false;
}
RGBColor RGBColor::fromString(std::string color)
{

    if (color.size() != 7 || color[0] != 'h') {
        throw std::invalid_argument("Invalid color format: " + color);
    }

    std::string red = color.substr(1, 2);
    std::string green = color.substr(3, 2);
    std::string blue = color.substr(5, 2);

    return {RGBColor::stringHexToInt(red), RGBColor::stringHexToInt(green),
            RGBColor::stringHexToInt(blue)};
}

std::vector<RGBColor> RGBColor::fromBytes(std::vector<uint8_t> colorBytes)
{

    std::vector<RGBColor> result;
    result.reserve(colorBytes.size() / 3);

    for (uint64_t i = 0; i < colorBytes.size(); i += 3) {
        auto red = colorBytes[i];
        auto green = colorBytes[i + 1];
        auto blue = colorBytes[i + 2];

        result.push_back({red, green, blue});
    }

    return result;
}
bool RGBColor::isColorCloseEnoughToReference(RGBColor other, uint8_t fudgeFactor) const
{

    bool redIsSimilar = false;
    bool greenIsSimilar = false;
    bool blueIsSimilar = false;

    if (std::abs(this->redChannel - other.redChannel) < fudgeFactor)
        redIsSimilar = true;
    if (std::abs(this->greenChannel - other.greenChannel) < fudgeFactor)
        greenIsSimilar = true;
    if (std::abs(this->blueChannel - other.blueChannel) < fudgeFactor)
        blueIsSimilar = true;

    return (redIsSimilar && greenIsSimilar && blueIsSimilar);
}

uint8_t RGBColor::stringHexToInt(const std::string &colorChannel)
{
    std::stringstream stream;
    stream << std::hex << colorChannel;

    uint16_t result; // apparently can't use a uint8_t to read since it's treated as a
                     // character...
    stream >> result;

    return static_cast<uint8_t>(result);
}