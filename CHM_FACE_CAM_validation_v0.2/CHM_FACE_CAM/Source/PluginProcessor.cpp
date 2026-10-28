#include "PluginProcessor.h"
#include "PluginEditor.h"
CHMFaceCamProcessor::CHMFaceCamProcessor() : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true).withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}
juce::AudioProcessorEditor* CHMFaceCamProcessor::createEditor() { return new CHMFaceCamEditor(*this); }
void CHMFaceCamProcessor::getStateInformation(juce::MemoryBlock& dest) {
 juce::ValueTree v("FaceCam");
 auto& s=settings;
 v.setProperty("left",s.cropLeft,nullptr); v.setProperty("right",s.cropRight,nullptr); v.setProperty("top",s.cropTop,nullptr); v.setProperty("bottom",s.cropBottom,nullptr);
 v.setProperty("zoom",s.zoom,nullptr); v.setProperty("rotation",s.rotation,nullptr); v.setProperty("brightness",s.brightness,nullptr); v.setProperty("contrast",s.contrast,nullptr); v.setProperty("saturation",s.saturation,nullptr); v.setProperty("warmth",s.warmth,nullptr);
 v.setProperty("mirror",s.mirror,nullptr); v.setProperty("clean",s.clean,nullptr); v.setProperty("camera",s.cameraIndex,nullptr);
 juce::MemoryOutputStream out(dest,false); v.writeToStream(out);
}
void CHMFaceCamProcessor::setStateInformation(const void* data,int size) {
 auto v=juce::ValueTree::readFromData(data,(size_t)size); if (!v.isValid()) return;
 auto& s=settings;
 s.cropLeft=(float)v.getProperty("left",0); s.cropRight=(float)v.getProperty("right",0); s.cropTop=(float)v.getProperty("top",0); s.cropBottom=(float)v.getProperty("bottom",0);
 s.zoom=(float)v.getProperty("zoom",1); s.rotation=(float)v.getProperty("rotation",0); s.brightness=(float)v.getProperty("brightness",0); s.contrast=(float)v.getProperty("contrast",1); s.saturation=(float)v.getProperty("saturation",1); s.warmth=(float)v.getProperty("warmth",0);
 s.mirror=(bool)v.getProperty("mirror",false); s.clean=(bool)v.getProperty("clean",false); s.cameraIndex=(int)v.getProperty("camera",0);
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new CHMFaceCamProcessor(); }
