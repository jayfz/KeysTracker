#main executable
add_executable(KeysTracker)

##main executable sources

#removed:
#RawFrame.cpp
#Keyboard.cpp
target_sources(KeysTracker PUBLIC
    Application.cpp
    ArgumentParser.cpp
    ColorFrameProcessor.cpp
    FallingNotesTracker.cpp
    H264Decoder.cpp
    KeyboardInfo.cpp
    KeyboardNotesColors.cpp
    KeyboardTracker.cpp
    main.cpp
    ManagedMidiFile.cpp
    OverlappingNotesStrategy.cpp
    PixelLine.cpp
    ProgramOptions.cpp
    RGBColor.cpp
    SpacedNotesStrategy.cpp
    Tracker.cpp
    PPMImageDecoder.cpp
    RawPPMImageProcessor.cpp
    SimpleNotesStrategy.cpp
)

##main executable include directory  (headers)

target_include_directories(KeysTracker PUBLIC
    ${KeysTracker_SOURCE_DIR}/include
)


##check environment, is it Unix or Windows(msvc)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

#dependency: must use c++ 17
add_library(keys_tracker_compiler_flags INTERFACE)
target_compile_features(keys_tracker_compiler_flags INTERFACE cxx_std_17)

##add compiler flags depending on the environment

#-Wshadow;
target_compile_options(keys_tracker_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-g;-Wall;-Wextra;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)


#dependency: add midifile library
#not add_subdirectory
add_subdirectory(
    ${KeysTracker_SOURCE_DIR}/external/cragsapp/midifile
    ${CMAKE_CURRENT_BINARY_DIR}/midifile
)

#dependency: add shared library avcodec:

find_library(AVCODEC_LIBRARY
NAMES avcodec
REQUIRED
)

#dependency: add shared library avutil:

find_library(AVUTIL_LIBRARY
NAMES avutil libavutil
REQUIRED
)

#dependency: add shared library swscale:

find_library(SWSCALE_LIBRARY
NAMES swscale libswscale
REQUIRED
)


#Linking libraries
## 1. compiler flags(not really a library)
## 2. External github library that we compile and then link
## 3. dynamic libraries that should have been previously installed (i.e sudo apt-get )
## -lavcodec -lavutil -lswscale
target_link_libraries(KeysTracker PUBLIC
    keys_tracker_compiler_flags
    Midifile
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${SWSCALE_LIBRARY}
)