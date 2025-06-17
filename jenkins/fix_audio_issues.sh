#!/bin/bash

# Apply patches to RayLib to disable audio in tests
mkdir -p /tmp/patches

cat > /tmp/patches/disable_audio.patch << 'EOL'
diff --git a/src/raudio.c b/src/raudio.c
index 12345678..87654321 100644
--- a/src/raudio.c
+++ b/src/raudio.c
@@ -31,6 +31,8 @@
 #include "raudio.h"

 #if defined(RAUDIO_STANDALONE)
+    #define AUDIO_DEVICE_FORMAT    0
+    #define AUDIO_DEVICE_CHANNELS  0
     #include <stdarg.h>         // Required for: va_list, va_start(), va_end()
     #include <stdio.h>          // Required for: FILE, fopen(), fclose(), fread()

@@ -84,6 +86,10 @@ static void *AudioBuffer = NULL;
 // Audio system initialization and de-initialization
 //------------------------------------------------------------------------------------

+// Dummy implementation for audio initialization
+bool InitAudioDevice(void) { return false; }
+void CloseAudioDevice(void) {}
+
 // Check if audio device is ready
 bool IsAudioDeviceReady(void)
 {
@@ -94,6 +100,8 @@ bool IsAudioDeviceReady(void)
 // Set master volume (listener)
 void SetMasterVolume(float volume)
 {
+    if (volume) {}  // Avoid unused parameter warning
+    return;
 }

 //------------------------------------------------------------------------------------
EOL

# Create a script that will be run before tests to set up the environment
cat > /tmp/patches/apply_patches.sh << 'EOL'
#!/bin/bash

# Apply patch if RayLib exists in /usr/local/include
if [ -d "/usr/local/include/raylib" ]; then
  cd /usr/local/include/raylib
  patch -p1 < /tmp/patches/disable_audio.patch || echo "Patch already applied or failed"
fi

# Create dummy audio devices
mkdir -p /tmp/audio
touch /tmp/audio/dummy_device

# Set additional environment variables
export AUDIODEV=null
export DISPLAY=:99
export SDL_AUDIODRIVER=dummy
export SDL_VIDEODRIVER=dummy
export RAYLIB_AUDIO_FLAG=AUDIO_DEVICE_NULL
export ALSA_CONFIG_PATH=/dev/null
export OPENAL_DRIVER=null
export NO_AUDIO=1
export LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:$LD_LIBRARY_PATH

# Start Xvfb if not already running
if ! pgrep -x Xvfb > /dev/null; then
  Xvfb :99 -screen 0 1024x768x24 &
  sleep 1
fi

echo "Environment prepared for GUI tests"
EOL

chmod +x /tmp/patches/apply_patches.sh

# Run the script to apply patches and set up environment
/tmp/patches/apply_patches.sh
