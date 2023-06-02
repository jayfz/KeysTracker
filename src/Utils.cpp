#include <vector>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "Utils.h"

/*
 * Converts an RGB color value to HSL. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes r, g, and b are contained in the set [0, 255] and
 * returns HSL in the set [0, 1].
 */

// double minFunc(double a, double b) { return (a < b) ? a : b; }
// double maxFunc(double a, double b) { return (a > b) ? a : b; }
HSL rgb2hsl(double red, double green, double blue)
{

    HSL result;

    double r = red / 255.0;
    double g = green / 255.0;
    double b = blue / 255.0;

    double cmax = std::max({r, g, b});
    double cmin = std::min({r, g, b});
    double delta = cmax - cmin;

    double h, s, l;

    if (delta == 0)
    {
        h = 0;
    }
    else if (cmax == r)
    {
        h = fmod((g - b) / delta, 6);
    }
    else if (cmax == g)
    {
        h = (b - r) / delta + 2;
    }
    else
    { // cmax == b
        h = (r - g) / delta + 4;
    }

    h = round(h * 60);
    if (h < 0)
    {
        h += 360;
    }

    l = (cmax + cmin) / 2;

    if (delta == 0)
    {
        s = 0;
    }
    else
    {
        s = delta / (1 - fabs(2 * l - 1));
    }

    // return {h, s, l};
    result.hue = static_cast<int>(std::round(h));
    result.saturation = static_cast<int>(std::round(s * 100));
    result.lightness = static_cast<int>(std::round(l * 100));

    return result;
}

bool areFirstTwoPixelsDifferentFromLastTwoPixels(const std::vector<RGB> &colors)
{

    // do some special checks for earlier return;
    if (colors.size() >= 4)
    {
        RGB color0 = colors[0];
        RGB color1 = colors[1];
        RGB colorLast = colors[colors.size() - 1];
        RGB colorLastMinusOne = colors[colors.size() - 2];

        RGB averageFirst = calculateAverageRGB({color0, color1});
        RGB averageLast = calculateAverageRGB({colorLast, colorLastMinusOne});

        bool redFromExtremesAreQuiteDifferent = false;
        bool greenFromExtremesAreQuiteDifferent = false;
        bool blueFromExtremesAreQuiteDifferent = false;

        if (std::abs(averageFirst.red - averageLast.red) >= 40) // 24
            redFromExtremesAreQuiteDifferent = true;

        if (std::abs(averageFirst.green - averageLast.green) >= 40) // 24
            greenFromExtremesAreQuiteDifferent = true;

        if (std::abs(averageFirst.blue - averageLast.blue) >= 40) // 24
            blueFromExtremesAreQuiteDifferent = true;

        bool startAndEndAreDifferentColors = redFromExtremesAreQuiteDifferent || greenFromExtremesAreQuiteDifferent || blueFromExtremesAreQuiteDifferent;

        if (startAndEndAreDifferentColors)
        {
            return true;
        }
    }

    return false;
}

RGB calculateAverageRGB(const std::vector<RGB> &colors)
{

    // run of the mill average calculator if above does not pan out;
    double redAccumualtor = 0;
    double greenAccumualtor = 0;
    double blueAccumualtor = 0;

    for (auto color : colors)
    {
        redAccumualtor += color.red;
        greenAccumualtor += color.green;
        blueAccumualtor += color.blue;
    }

    redAccumualtor = std::round(redAccumualtor / colors.size());
    greenAccumualtor = std::round(greenAccumualtor / colors.size());
    blueAccumualtor = std::round(blueAccumualtor / colors.size());

    return {static_cast<uint8_t>(redAccumualtor),
            static_cast<uint8_t>(greenAccumualtor),
            static_cast<uint8_t>(blueAccumualtor)};
}
bool isColorCloseEnoughToReferenceRGB(RGB reference, RGB other, bool beStrict)
{

    bool redIsSimilar = false;
    bool greenIsSimilar = false;
    bool blueIsSimilar = false;

    uint8_t fudgeFactor = beStrict ? 24 : 50;

    if (std::abs(reference.red - other.red) < fudgeFactor)
        redIsSimilar = true;
    if (std::abs(reference.green - other.green) < fudgeFactor)
        greenIsSimilar = true;
    if (std::abs(reference.blue - other.blue) < fudgeFactor)
        blueIsSimilar = true;

    return (redIsSimilar && greenIsSimilar && blueIsSimilar);
}

uint8_t stringHexToInt(const std::string &colorChannel)
{
    std::stringstream stream;
    stream << std::hex << colorChannel;

    uint16_t result; // apparently can't use a uint8_t to read since it's treated as a character...
    stream >> result;

    return static_cast<uint8_t>(result);
}

RGB stringToRGB(char *color)
{
    std::string colorString(color);

    if (colorString.size() != 7 || colorString[0] != 'h')
    {
        throw std::invalid_argument("Invalid color format: " + colorString);
    }

    std::string red = colorString.substr(1, 2);
    std::string green = colorString.substr(3, 2);
    std::string blue = colorString.substr(5, 2);

    return {
        stringHexToInt(red),
        stringHexToInt(green),
        stringHexToInt(blue),
    };
}