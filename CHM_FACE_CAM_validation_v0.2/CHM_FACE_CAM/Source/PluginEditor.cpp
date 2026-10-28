#include "PluginEditor.h"
#include <cmath>
CHMFaceCamEditor::CHMFaceCamEditor(CHMFaceCamProcessor& p): AudioProcessorEditor(&p),processor(p) {
 setOpaque(true);
 setResizable(true,true); setResizeLimits(240,180,1600,1100); setSize(760,580);
 auto names=juce::CameraDevice::getAvailableDevices();
 for(int i=0;i<names.size();++i) cameraChoice.addItem(names[i],i+1);
 cameraChoice.onChange=[this]{ openCamera(cameraChoice.getSelectedItemIndex()); };
 addAndMakeVisible(cameraChoice);
 cleanButton.onClick=[this]{setClean(!processor.settings.clean);}; addAndMakeVisible(cleanButton);
 mirrorButton.onClick=[this]{processor.settings.mirror=!processor.settings.mirror; repaint();}; addAndMakeVisible(mirrorButton);
 resetButton.onClick=[this]{auto index=processor.settings.cameraIndex; processor.settings={}; processor.settings.cameraIndex=index; for(auto* s:sliders) s->setValue(s->getProperties().getWithDefault("reset",0),juce::sendNotificationSync); repaint();}; addAndMakeVisible(resetButton);
 auto& s=processor.settings;
 addSlider("Crop left",s.cropLeft,0,0.45,0); addSlider("Crop right",s.cropRight,0,0.45,0);
 addSlider("Crop top",s.cropTop,0,0.45,0); addSlider("Crop bottom",s.cropBottom,0,0.45,0);
 addSlider("Zoom",s.zoom,1,4,1); addSlider("Rotation",s.rotation,-180,180,0);
 addSlider("Brightness",s.brightness,-1,1,0); addSlider("Contrast",s.contrast,0,2,1);
 addSlider("Saturation",s.saturation,0,2,1); addSlider("Warmth",s.warmth,-1,1,0);
 setClean(s.clean);
 if(!names.isEmpty()) cameraChoice.setSelectedItemIndex(juce::jlimit(0,names.size()-1,s.cameraIndex),juce::sendNotificationSync);
}
CHMFaceCamEditor::~CHMFaceCamEditor() { cancelPendingUpdate(); if(camera) camera->removeListener(this); camera.reset(); { const juce::ScopedLock lock(frameLock); latestFrame = {}; } }
void CHMFaceCamEditor::addSlider(const juce::String& title,float& target,double lo,double hi,double reset) {
 auto* l=labels.add(new juce::Label()); l->setText(title,juce::dontSendNotification); l->setColour(juce::Label::textColourId,juce::Colours::lightgrey); addAndMakeVisible(l);
 auto* slider=sliders.add(new juce::Slider()); slider->setRange(lo,hi,0.01); slider->setValue(target); slider->setSliderStyle(juce::Slider::LinearHorizontal); slider->setTextBoxStyle(juce::Slider::TextBoxRight,false,54,20); slider->getProperties().set("reset",reset);
 auto* targetPtr = &target; slider->onValueChange=[this,slider,targetPtr]{*targetPtr=(float)slider->getValue(); repaint(preview);}; addAndMakeVisible(slider);
}
void CHMFaceCamEditor::openCamera(int index) {
 if(camera) camera->removeListener(this); camera.reset(); { const juce::ScopedLock lock(frameLock); latestFrame = {}; }
 auto names=juce::CameraDevice::getAvailableDevices();
 if(index<0 || index>=names.size()) return;
 processor.settings.cameraIndex=index;
 camera.reset(juce::CameraDevice::openDevice(index,320,240,1920,1080));
 if(camera) camera->addListener(this);
 repaint();
}
void CHMFaceCamEditor::imageReceived(const juce::Image& frame) {
 { const juce::ScopedLock lock(frameLock); latestFrame=frame; }
 triggerAsyncUpdate();
}
void CHMFaceCamEditor::handleAsyncUpdate() { repaint(preview); }
void CHMFaceCamEditor::setClean(bool clean) {
 processor.settings.clean=clean;
 cameraChoice.setVisible(!clean); cleanButton.setVisible(!clean); mirrorButton.setVisible(!clean); resetButton.setVisible(!clean);
 for(auto* s:sliders) s->setVisible(!clean); for(auto* l:labels) l->setVisible(!clean);
 resized(); repaint();
}
void CHMFaceCamEditor::resized() {
 const int w=getWidth(), h=getHeight(); const bool clean=processor.settings.clean;
 if(clean) {preview=getLocalBounds(); return;}
 const int panel=juce::jmin(265,juce::jmax(185,w/3));
 preview=juce::Rectangle<int>(0,0,w-panel,h);
 auto x=w-panel+8, pw=panel-16;
 cameraChoice.setBounds(x,8,pw,26); cleanButton.setBounds(x,40,pw,25);
 mirrorButton.setBounds(x,69,pw/2-3,25); resetButton.setBounds(x+pw/2+3,69,pw/2-3,25);
 int y=100; for(int i=0;i<sliders.size();++i) { labels[i]->setBounds(x,y,pw,17); sliders[i]->setBounds(x,y+17,pw,24); y+=45; }
}
void CHMFaceCamEditor::mouseDown(const juce::MouseEvent& e) { if(e.mods.isPopupMenu() || processor.settings.clean) showMenu(); }
void CHMFaceCamEditor::showMenu() {
 juce::PopupMenu m; m.addItem(1,processor.settings.clean?"Show controls":"Clean view"); m.addItem(2,"Mirror",true,processor.settings.mirror);
 m.addSeparator(); m.addItem(3,"Camera off");
 auto names=juce::CameraDevice::getAvailableDevices(); for(int i=0;i<names.size();++i) m.addItem(100+i,names[i],true,processor.settings.cameraIndex==i);
 m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this),[safe=juce::Component::SafePointer<CHMFaceCamEditor>(this)](int result){if(!safe) return; if(result==1) safe->setClean(!safe->processor.settings.clean); else if(result==2){safe->processor.settings.mirror=!safe->processor.settings.mirror;safe->repaint();}else if(result==3){if(safe->camera)safe->camera->removeListener(safe.getComponent());safe->camera.reset();{const juce::ScopedLock lock(safe->frameLock);safe->latestFrame={};}safe->repaint();}else if(result>=100) {safe->openCamera(result-100);safe->cameraChoice.setSelectedItemIndex(result-100,juce::dontSendNotification);} });
}
void CHMFaceCamEditor::paint(juce::Graphics& g) {
 g.fillAll(juce::Colour(0xff101115));
 juce::Image frame; {const juce::ScopedLock lock(frameLock);frame=latestFrame;}
 if(frame.isNull()) {g.setColour(juce::Colours::grey);g.setFont(17);g.drawFittedText("CHM FACE CAM  |  Select a webcam",preview,juce::Justification::centred,2);return;}
 auto& s=processor.settings;
 const int sw=frame.getWidth(),sh=frame.getHeight();
 int left=juce::roundToInt(sw*s.cropLeft),right=juce::roundToInt(sw*s.cropRight),top=juce::roundToInt(sh*s.cropTop),bottom=juce::roundToInt(sh*s.cropBottom);
 auto source=juce::Rectangle<int>(left,top,juce::jmax(1,sw-left-right),juce::jmax(1,sh-top-bottom));
 juce::Image cut=frame.getClippedImage(source);
 // Per-pixel colour grading on a copy: never alters the incoming camera frame.
 if(s.brightness!=0 || s.contrast!=1 || s.saturation!=1 || s.warmth!=0) {
  cut=cut.createCopy(); juce::Image::BitmapData pixels(cut,juce::Image::BitmapData::readWrite);
  for(int y=0;y<cut.getHeight();++y)for(int x=0;x<cut.getWidth();++x){
   auto c=pixels.getPixelColour(x,y); float r=c.getFloatRed(),gr=c.getFloatGreen(),b=c.getFloatBlue();
   float lum=0.2126f*r+0.7152f*gr+0.0722f*b;
   r=lum+(r-lum)*s.saturation;gr=lum+(gr-lum)*s.saturation;b=lum+(b-lum)*s.saturation;
   r=(r-0.5f)*s.contrast+0.5f+s.brightness+s.warmth*0.12f;
   gr=(gr-0.5f)*s.contrast+0.5f+s.brightness;
   b=(b-0.5f)*s.contrast+0.5f+s.brightness-s.warmth*0.12f;
   pixels.setPixelColour(x,y,juce::Colour::fromFloatRGBA(juce::jlimit(0.f,1.f,r),juce::jlimit(0.f,1.f,gr),juce::jlimit(0.f,1.f,b),1));
  }
 }
 juce::Graphics::ScopedSaveState saved(g); g.reduceClipRegion(preview);
 float scale=juce::jmin((float)preview.getWidth()/cut.getWidth(),(float)preview.getHeight()/cut.getHeight())*s.zoom;
 auto cx=(float)preview.getCentreX(),cy=(float)preview.getCentreY();
 auto transform=juce::AffineTransform::translation(-cut.getWidth()*0.5f,-cut.getHeight()*0.5f)
  .scaled(s.mirror?-scale:scale,scale).rotated(juce::degreesToRadians(s.rotation)).translated(cx,cy);
 g.drawImageTransformed(cut,transform,false);
}
