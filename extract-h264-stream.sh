#!/bin/bash
input_file="$1"

ffmpeg -i "$input_file" -c:v copy -bsf:v h264_mp4toannexb -an output.h264
