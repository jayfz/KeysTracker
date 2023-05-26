add_library(Midifile STATIC)

set_property(TARGET Midifile PROPERTY VERSION "1.0.0")

target_sources(Midifile PUBLIC
    Binasc.cpp
    MidiEvent.cpp
    MidiEventList.cpp
    MidiFile.cpp
    MidiMessage.cpp
)


# state that anybody linking to us needs to include MIDIFILE_HEADERS
# to find MIDIFILE_HEADERS while we don't.
target_include_directories(Midifile PUBLIC
    ${MIDIFILE_HEADERS}
)

target_link_libraries(Midifile PUBLIC keys_tracker_compiler_flags)
#${CMAKE_CURRENT_SOURCE_DIR}/../include

