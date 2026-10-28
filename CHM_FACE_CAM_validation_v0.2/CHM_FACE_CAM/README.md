# CHM FACE CAM — experimental VST3 source

Webcam preview on the master bus with crop (4 edges), zoom, rotation, mirror, brightness, contrast, saturation, warmth and a clean view. No recording, network, or audio DSP. Right-click anywhere in clean view to reopen controls. JUCE 8, C++17.

## Build
Push this folder (including `.github`) to a GitHub repository and run **Actions → Build Windows VST3 → Run workflow**. On success download the build artifact and copy `CHM Face Cam.vst3` to `C:\Program Files\Common Files\VST3\`.

## Caveats
- This is a **first source implementation, not yet build-verified or host-tested**. JUCE/host API details may need adjustment.
- VST3 hosts control floating-window behavior; resizing is requested through JUCE and may be restricted by the host.
- CPU-based pixel processing is not yet optimized. Some webcam drivers may fail to initialize.
- Camera device access is managed by the editor; camera stops when editor closes.
- This is an original implementation, not copied from Habitudes Facecam.
