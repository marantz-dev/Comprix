#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>
#include <memory>
#include "UI/Dial.h"
#include "juce_audio_processors/juce_audio_processors.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class comprixAudioProcessorEditor : public juce::AudioProcessorEditor {
  public:
    comprixAudioProcessorEditor(comprixAudioProcessor &, AudioProcessorValueTreeState &);
    ~comprixAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    void setupSlider(Slider &slider, int x, int y, int w, int h);

    comprixAudioProcessor &audioProcessor;
    AudioProcessorValueTreeState &valueTreeState;
    // Custom look and feel
    DBXDial prettyLookAndFeel;

    // Your dials (add as many as you need)
    Slider gainDial;

    // Labels for the dials
    Label gainLabel;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(comprixAudioProcessorEditor)
};
