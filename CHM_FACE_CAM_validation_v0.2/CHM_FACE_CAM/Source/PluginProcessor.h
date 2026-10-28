#pragma once
#include <JuceHeader.h>
class CHMFaceCamProcessor final : public juce::AudioProcessor
{
public:
 CHMFaceCamProcessor();
 const juce::String getName() const override { return "CHM Face Cam"; }
 void prepareToPlay(double, int) override {}
 void releaseResources() override {}
 bool isBusesLayoutSupported(const BusesLayout& l) const override { return l.getMainInputChannelSet() == l.getMainOutputChannelSet() && !l.getMainInputChannelSet().isDisabled(); }
 void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override {}
 void processBlock(juce::AudioBuffer<double>&, juce::MidiBuffer&) override {}
 bool supportsDoublePrecisionProcessing() const override { return true; }
 juce::AudioProcessorEditor* createEditor() override;
 bool hasEditor() const override { return true; }
 double getTailLengthSeconds() const override { return 0; }
 bool acceptsMidi() const override { return false; }
 bool producesMidi() const override { return false; }
 int getNumPrograms() override { return 1; }
 int getCurrentProgram() override { return 0; }
 void setCurrentProgram(int) override {}
 const juce::String getProgramName(int) override { return {}; }
 void changeProgramName(int, const juce::String&) override {}
 void getStateInformation(juce::MemoryBlock&) override;
 void setStateInformation(const void*, int) override;
 struct Settings { float cropLeft=0, cropRight=0, cropTop=0, cropBottom=0, zoom=1, rotation=0, brightness=0, contrast=1, saturation=1, warmth=0; bool mirror=false, clean=false; int cameraIndex=0; } settings;
private:
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CHMFaceCamProcessor)
};
