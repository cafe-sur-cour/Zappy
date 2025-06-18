#!/bin/bash

# Start Xvfb
Xvfb :99 -screen 0 1024x768x24 &
export DISPLAY=:99

# Set additional environment variables for headless testing
export SDL_AUDIODRIVER=dummy
export SDL_VIDEODRIVER=dummy
export RAYLIB_AUDIO_FLAG=AUDIO_DEVICE_NULL
export ALSA_CONFIG_PATH=/dev/null
export OPENAL_DRIVER=null
export NO_AUDIO=1

# Wait for Xvfb to start
sleep 1

echo "Virtual display and audio environment set up for testing."
