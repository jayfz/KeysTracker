#include "Application.h"
#include "ColorFrameProcessor.h"
#include "FallingNotesTracker.h"
#include "H264Decoder.h"
#include "KeyboardTracker.h"
#include "OverlappingNotesStrategy.h"
#include "PPMImageDecoder.h"
#include "RawPPMImageProcessor.h"
#include "SpacedNotesStrategy.h"

#include <filesystem>

bool doesCachedDecodedImageExist(const std::string &filename);
std::string getFileStem(const std::string &filename);

Application::Application(ProgramOptions options)
    : midiFile(options.outFileName),

      noteColors(options.leftHandWhiteKeyColor, options.leftHandBlackKeyColor,
                 options.rightHandWhiteKeyColor, options.rightHandBlackKeyColor),

      keyboardInfo(options.octavesLength, options.numOfOctaves, options.firstOctaveAt,
                   noteColors),
      tracker(this->getTracker(options.trackMode)),
      videoStreamFileName(options.videoStreamFileName)
{

    if (!doesCachedDecodedImageExist(options.videoStreamFileName)) {

        this->frameProcessor = new ColorFrameProcessor(options.rawFrameLinesToExtract,
                                                       options.rawFrameCopyFromLine);

        this->decoder = new H264Decoder(this->frameProcessor, options.videoStreamFileName,
                                        options.numberOfFramesToSkip);
    } else {

        this->frameProcessor = new RawPPMImageProcessor();
        this->decoder = new PPMImageDecoder(this->frameProcessor, "");
    }
}
Application::~Application()
{
    delete this->frameProcessor;
    delete this->tracker;
    delete this->trackingPointStrategy;
}
void Application::run()
{

    if (!decoder->wasInitializedCorrectly()) {
        return;
    }

    decoder->decode();
    PixelLine::saveBigFormatFrame(this->frameProcessor->getLines(),
                                  getFileStem(this->videoStreamFileName));
    std::vector<MidiKeysEvent> events =
        tracker->generateMidiEvents(this->frameProcessor->getLines());

    for (const auto &event : events) {
        if (event.isLeftEvent)
            this->midiFile.addLeftHandEvent(event.tick, event.note + 24, event.velocity);
        else
            this->midiFile.addRightHandEvent(event.tick, event.note + 24, event.velocity);
    }

    this->midiFile.save();
}

Tracker *Application::getTracker(std::string trackerOption)
{

    if (trackerOption == "falling-notes") {
        this->trackingPointStrategy = new OverlappingNotesStrategy();
        return new FallingNotesTracker(this->keyboardInfo, this->trackingPointStrategy);
    }

    if (trackerOption == "keys") {
        this->trackingPointStrategy = new SpacedNotesStrategy();
        return new KeyboardTracker(this->keyboardInfo, this->trackingPointStrategy);
    }

    this->trackingPointStrategy = new OverlappingNotesStrategy();
    return new FallingNotesTracker(this->keyboardInfo, this->trackingPointStrategy);
}

bool doesCachedDecodedImageExist(const std::string &filename)
{
    std::string fileStem = getFileStem(filename);
    std::filesystem::path path(PixelLine::getPathForBigFormatFrame(fileStem));
    return std::filesystem::exists(path);
}

std::string getFileStem(const std::string &filename)
{
    std::filesystem::path path(filename);
    return std::string(path.stem());
}
