# KeysTracker
A C++ learning exercise. Keystracker takes a H264 video stream of a synthesia-like Piano tutorial and handful of command-line parameters, tries to decode the frames and understand when and which notes are being played. It then outputs a midi file with the song.

While efforts for cross-platform compatibility were made, at the moment it supports only enviroments that have a install of these core dependencies:

1. avcodec
2. libavutil
3. libswscale

Tested only on Ubuntu/Debian.
