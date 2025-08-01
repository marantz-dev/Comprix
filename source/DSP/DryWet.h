
#pragma once
#include "juce_audio_basics/juce_audio_basics.h"
#include <JuceHeader.h>

#define DEFAULT_DRY_WET 1.0f

class DryWet {
  public:
    DryWet(float defaultDryWetRatio = DEFAULT_DRY_WET) : dryWetRatio(defaultDryWetRatio) {}

    ~DryWet() {}

    void prepareToPlay(int maxNumSamples, double sampleRate) {
        drySignal.setSize(2, maxNumSamples);
        drySignal.clear();
        dryWetRatio.reset(sampleRate, 0.05f);
        dryWetRatio.setCurrentAndTargetValue(DEFAULT_DRY_WET);
    }

    void releaseResources() { drySignal.setSize(0, 0); }

    void copyDrySignal(const AudioBuffer<float> &sourceBuffer) {
        const auto numCh = sourceBuffer.getNumChannels();
        const auto numSamples = sourceBuffer.getNumSamples();

        for(int ch = 0; ch < numCh; ++ch)
            drySignal.copyFrom(ch, 0, sourceBuffer, ch, 0, numSamples);
    }

    void mixDrySignal(AudioBuffer<float> &destinationBuffer) {
        const auto numCh = destinationBuffer.getNumChannels();
        const auto numSamples = destinationBuffer.getNumSamples();

        auto dryData = drySignal.getArrayOfWritePointers();
        auto destData = destinationBuffer.getArrayOfWritePointers();
        for(int s = 0; s < numSamples; ++s) {
            auto currentDryWetRatio = dryWetRatio.getNextValue();
            for(int ch = 0; ch < numCh; ++ch) {
                dryData[ch][s] *= 1.0f - currentDryWetRatio;
                destData[ch][s] = destData[ch][s] * currentDryWetRatio + dryData[ch][s];
            }
        }
    }

    void setDWRatio(float newValue) { dryWetRatio.setTargetValue(newValue / 100.0f); }

  private:
    AudioBuffer<float> drySignal;

    SmoothedValue<float, ValueSmoothingTypes::Linear> dryWetRatio;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)
};
