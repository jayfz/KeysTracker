#ifndef APPLICATION_H
#define APPLICATION_H

#include "FallingNotesTracker.h"
#include "FrameProcessor.h"
#include "H264Decoder.h"
#include "KeyboardInfo.h"
#include "KeyboardNotesColors.h"
#include "KeyboardTracker.h"
#include "ManagedMidiFile.h"
#include "OverlappingNotesStrategy.h"
#include "ProgramOptions.h"
#include "SpacedNotesStrategy.h"

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
    H264Decoder decoder;
    ManagedMidiFile midiFile;
    KeyboardNotesColors noteColors;
    KeyboardInfo keyboardInfo;
    Tracker *tracker;
    TrackingPointStrategy *trackingPointStrategy;
};

#endif
