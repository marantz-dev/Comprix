#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

class comprixAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  comprixAudioProcessorEditor(comprixAudioProcessor &);
  ~comprixAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  comprixAudioProcessor &audioProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(comprixAudioProcessorEditor)
};
