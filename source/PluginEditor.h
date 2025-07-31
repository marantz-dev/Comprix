#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Meters.h"
#include "Sidechain.h"
#include "Compression.h"
#include "Scope.h"
#include "Theme.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class comprixAudioProcessorEditor : public juce::AudioProcessorEditor {
  public:
    comprixAudioProcessorEditor(comprixAudioProcessor &, AudioProcessorValueTreeState &);
    ~comprixAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    comprixAudioProcessor &audioProcessor;
    AudioProcessorValueTreeState &valueTreeState;

    SidechainSection sidechainSection;
    CompressorSection compressorSection;
    ScopeSection scopeSection;
    MeteringSection meteringSection;

    Theme theme;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(comprixAudioProcessorEditor)
};
