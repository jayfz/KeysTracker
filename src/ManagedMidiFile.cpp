#include "ManagedMidiFile.h"
#include <iostream>

ManagedMidiFile::ManagedMidiFile(const std::string &outFileName)
{
    this->midiFile.setTicksPerQuarterNote(120);
    this->midiFile.addTrack();
    this->midiFile.addTrackName(0, 0, "Right hand.");
    this->midiFile.addTrackName(1, 0, "Left hand.");
    this->outFileName = outFileName;
}

void ManagedMidiFile::addRightHandEvent(int atick, int key, int velocity)
{
    this->midiFile.addNoteOn(0, atick, 0, key, velocity);
}

void ManagedMidiFile::addLeftHandEvent(int atick, int key, int velocity)
{
    this->midiFile.addNoteOn(1, atick, 1, key, velocity);
}

void ManagedMidiFile::save() { this->midiFile.write(this->outFileName); }