#include "Application.h"

Application::Application(ProgramOptions options)
    :

      frameProcessor(new ColorFrameProcessor(options.rawFrameLinesToExtract,
                                             options.rawFrameCopyFromLine)),

      decoder(frameProcessor, options.videoStreamFileName, 5),

      midiFile(options.outFileName),

      noteColors(options.leftHandWhiteKeyColor, options.leftHandBlackKeyColor,
                 options.rightHandWhiteKeyColor, options.rightHandBlackKeyColor),

      keyboardInfo(options.octavesLength, options.numOfOctaves, options.firstOctaveAt,
                   noteColors)

{

    this->tracker = getTracker(options.trackMode);
}
Application::~Application()
{
    delete this->frameProcessor;
    delete this->tracker;
    delete this->trackingPointStrategy;
}
void Application::run()
{

    if (decoder.wasInitializedCorrectly()) {

        decoder.decode();

        std::vector<MidiKeysEvent> events =
            tracker->generateMidiEvents(this->frameProcessor->getLines());

        for (const auto &event : events) {
            if (event.isLeftEvent)
                this->midiFile.addLeftHandEvent(event.tick, event.note + 0,
                                                event.velocity);
            else
                this->midiFile.addRightHandEvent(event.tick, event.note + 0,
                                                 event.velocity);
        }

        this->midiFile.save();
    }
}

Tracker *Application::getTracker(std::string trackerOption)
{

    if (trackerOption == "falling-notes") {
        this->trackingPointStrategy = new SpacedNotesStrategy();
        return new FallingNotesTracker(this->keyboardInfo, this->trackingPointStrategy);
    }

    if (trackerOption == "keys") {
        this->trackingPointStrategy = new OverlappingNotesStrategy();
        return new KeyboardTracker(this->keyboardInfo, this->trackingPointStrategy);
    }

    this->trackingPointStrategy = new SpacedNotesStrategy();
    return new FallingNotesTracker(this->keyboardInfo, this->trackingPointStrategy);
}
