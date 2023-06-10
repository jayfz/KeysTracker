#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H

#include "ArgumentParser.h"
#include <vector>
#include <string>

struct ProgramOptions
{

    ProgramOptions(ArgumentParser parser);
    bool areValid() const;
    void reportErrors() const;

    std::string videoStreamFileName;
    std::string trackMode;
    std::string leftHandWhiteKeyColor;
    std::string leftHandBlackKeyColor;
    std::string rightHandWhiteKeyColor;
    std::string rightHandBlackKeyColor;
    std::string outFileName;
    int firstOctaveAt;
    double octavesLength;
    int numOfOctaves;
    int rawFrameHeight;
    int rawFrameWidth;
    int rawFrameCopyFromLine;
    int numberOfFramesToSkip;

private:
    static std::string getErrorForMissedRequired(std::string required);
    mutable std::vector<std::string> errors;
};
#endif