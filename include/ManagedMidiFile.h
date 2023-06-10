#ifndef MANAGED_MIDI_FILE
#define MANAGED_MIDI_FILE

#include <MidiFile.h>

class ManagedMidiFile
{

public:
    ManagedMidiFile(const std::string &outFileName);
    ManagedMidiFile(const ManagedMidiFile &other) = delete;
    ManagedMidiFile &operator=(const ManagedMidiFile &other) = delete;

    void addRightHandEvent(int atick, int key, int velocity = 0);
    void addLeftHandEvent(int atick, int key, int velocity = 0);
    void save();

private:
    smf::MidiFile midiFile;
    std::string outFileName;
};

#endif