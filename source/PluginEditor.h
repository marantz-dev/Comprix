#pragma once

#include "PluginProcessor.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include <JuceHeader.h>
#include "UI/Sections/Sidechain.h"

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

    GroupComponent sidechainSectionBorder;
    GroupComponent compressorSectionBorder;
    GroupComponent meterSectionBorder;
    GroupComponent scopeSectionBorder;
    GroupComponent scopeControlsBorder;

    Slider sidechainGainSlider;
    ToggleButton externalSidechainButton;
    ToggleButton sidechainListenButton;
    SidechainSection sidechainSection;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(comprixAudioProcessorEditor)
};
