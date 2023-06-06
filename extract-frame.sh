#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 2 ]; then
    echo "Usage: ./extract_frame.sh <input.mp4> <frame_number>"
    exit 1
fi

input_file="$1"
frame_number="$2"

# Execute the ffmpeg command to extract the frame
ffmpeg -i "$input_file" -vf "select=eq(n\,$frame_number)" -vframes 1 frame.jpg
