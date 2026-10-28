# CHM FACE CAM — validation status

## Verified locally
- ZIP extraction and source inventory
- CMake source paths and GitHub workflow structure reviewed
- Camera frame reset protected by the frame lock
- GitHub artifact upload targets the VST3 output directory

## NOT YET VERIFIED
- JUCE C++ compilation (JUCE is not installed; outbound GitHub DNS is unavailable in this environment)
- Camera permissions and video performance on Windows
- Actual VST3 loading in Cubase 14 or FL Studio
- Host-specific resizing and Clean View behavior

## Known functional gaps
- Color processing is performed in paint() and may cause high CPU load
- The small settings window clips some controls (scrollable panel needed)
- No independent floating window beyond the host's plugin window
- Filter presets, temperature/tint/gamma not implemented yet
- Webcam may not be shared with another program if its driver disallows it

Do not label this build production-ready until CI and host testing pass.
