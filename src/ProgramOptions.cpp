#include "ProgramOptions.h"
#include <iostream>

ProgramOptions::ProgramOptions(ArgumentParser parser)
{

    // required
    this->videoStreamFileName =
        parser.getOptionOrDefault<std::string>("input-video-filename", "");
    this->leftHandWhiteKeyColor =
        parser.getOptionOrDefault<std::string>("left-hand-white-key-color", "");
    this->leftHandBlackKeyColor =
        parser.getOptionOrDefault<std::string>("left-hand-black-key-color", "");
    this->rightHandWhiteKeyColor =
        parser.getOptionOrDefault<std::string>("right-hand-white-key-color", "");
    this->rightHandBlackKeyColor =
        parser.getOptionOrDefault<std::string>("right-hand-black-key-color", "");
    this->firstOctaveAt = parser.getOptionOrDefault<int>("first-octave-at", -1);
    this->octavesLength = parser.getOptionOrDefault<double>("octaves-length", -1);
    this->numOfOctaves = parser.getOptionOrDefault<int>("number-of-octaves", -1);
    this->rawFrameLinesToExtract =
        parser.getOptionOrDefault<int>("raw-frame-lines-to-extract", -1);
    this->rawFrameCopyFromLine =
        parser.getOptionOrDefault<int>("raw-frame-copy-from-line", -1);

    // optional
    this->trackMode =
        parser.getOptionOrDefault<std::string>("track-mode", "falling-notes");
    this->numberOfFramesToSkip =
        parser.getOptionOrDefault<int>("number-of-frames-to-skip", 0);
    this->outFileName = parser.getOptionOrDefault<std::string>("out-filename", "out.mid");

    this->firstOctavePositions =
        parser.getOptionOrDefault<std::string>("first-octave-positions", "");
}
bool ProgramOptions::areValid() const
{
    if (this->videoStreamFileName == "") {
        errors.push_back(
            ProgramOptions::getErrorForMissedRequired("input-video-filename"));
    }

    if (this->leftHandWhiteKeyColor == "") {
        errors.push_back(
            ProgramOptions::getErrorForMissedRequired("left-hand-white-key-color"));
    }

    if (this->leftHandBlackKeyColor == "") {
        errors.push_back(
            ProgramOptions::getErrorForMissedRequired("left-hand-black-key-color"));
    }

    if (this->rightHandWhiteKeyColor == "") {
        errors.push_back(
            ProgramOptions::getErrorForMissedRequired("right-hand-white-key-color"));
    }

    if (this->rightHandBlackKeyColor == "") {
        errors.push_back(
            ProgramOptions::getErrorForMissedRequired("right-hand-black-key-color"));
    }

    if (this->firstOctaveAt == -1) {
        errors.push_back(ProgramOptions::getErrorForMissedRequired("first-octave-at"));
    }

    if (this->octavesLength == -1) {
        errors.push_back(ProgramOptions::getErrorForMissedRequired("octaves-length"));
    }

    if (this->numOfOctaves == -1) {
        errors.push_back(ProgramOptions::getErrorForMissedRequired("number-of-octaves"));
    }

    if (this->rawFrameLinesToExtract == -1) {
        errors.push_back(
            ProgramOptions::getErrorForMissedRequired("raw-frame-lines-to-extract"));
    }

    if (this->rawFrameCopyFromLine == -1) {
        errors.push_back(ProgramOptions::getErrorForMissedRequired(
            "raw-frame-copy-from-y-percentage"));
    }

    return errors.empty();
}
void ProgramOptions::reportErrors() const
{
    for (const auto &error : this->errors) {
        std::cout << error << std::endl;
    }
}

std::string ProgramOptions::getErrorForMissedRequired(std::string required)
{
    return "Required argument: --" + required + " is missing.";
}
