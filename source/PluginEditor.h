#pragma once

#include <JuceHeader.h>
#include <memory>
#include "PluginProcessor.h"
#include "Metering.h"
#include "Sidechain.h"
#include "Compression.h"
#include "Scope.h"
#include "Theme.h"
#include "juce_gui_basics/juce_gui_basics.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class ComprixAudioProcessorEditor : public juce::AudioProcessorEditor {
  public:
    ComprixAudioProcessorEditor(ComprixAudioProcessor &, AudioProcessorValueTreeState &);
    ~ComprixAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    ComprixAudioProcessor &audioProcessor;
    AudioProcessorValueTreeState &valueTreeState;

    SidechainSection sidechainSection;
    CompressorSection compressorSection;
    ScopeSection scopeSection;
    MeteringSection meteringSection;

    Theme theme;

    ToggleButton bypassButton;
    using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<ButtonAttachment> bypassButtonAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComprixAudioProcessorEditor)
};
