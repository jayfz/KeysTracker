#ifndef APPLICATION_H
#define APPLICATION_H

#include "Decoder.h"
#include "FrameProcessor.h"
#include "KeyboardInfo.h"
#include "KeyboardNotesColors.h"
#include "ManagedMidiFile.h"
#include "ProgramOptions.h"
#include "Tracker.h"
#include "TrackingPointStrategy.h"

class Application
{

  public:
    Application(ProgramOptions options);
    Application(const Application &other) = delete;
    Application &operator=(const Application &other) = delete;
    ~Application();

    void run();

  private:
    Tracker *getTracker(std::string trackerOption);

    FrameProcessor *frameProcessor;
    Decoder *decoder;
    ManagedMidiFile midiFile;
    KeyboardNotesColors noteColors;
    KeyboardInfo keyboardInfo;
    Tracker *tracker;
    TrackingPointStrategy *trackingPointStrategy;
    ProgramOptions pOptions;
    bool cacheRun;
};

#endif
