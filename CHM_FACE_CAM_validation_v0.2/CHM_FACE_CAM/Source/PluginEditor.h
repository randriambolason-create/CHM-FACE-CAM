#pragma once
#include "PluginProcessor.h"
class CHMFaceCamEditor final : public juce::AudioProcessorEditor, private juce::CameraDevice::Listener, private juce::AsyncUpdater
{
public:
 explicit CHMFaceCamEditor(CHMFaceCamProcessor&);
 ~CHMFaceCamEditor() override;
 void paint(juce::Graphics&) override;
 void resized() override;
 void mouseDown(const juce::MouseEvent&) override;
private:
 void imageReceived(const juce::Image&) override;
 void handleAsyncUpdate() override;
 void openCamera(int index);
 void setClean(bool);
 void showMenu();
 void addSlider(const juce::String&, float&, double, double, double);
 CHMFaceCamProcessor& processor;
 std::unique_ptr<juce::CameraDevice> camera;
 juce::CriticalSection frameLock;
 juce::Image latestFrame;
 juce::ComboBox cameraChoice;
 juce::TextButton cleanButton{"CLEAN VIEW"}, mirrorButton{"MIRROR"}, resetButton{"RESET"};
 juce::OwnedArray<juce::Slider> sliders;
 juce::OwnedArray<juce::Label> labels;
 juce::Rectangle<int> preview;
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CHMFaceCamEditor)
};
