
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

        // updateState();
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

        // destinationBuffer.applyGain(wetGain);

        // for(int ch = 0; ch < numCh; ++ch) {
        //     drySignal.applyGain(ch, 0, numSamples, dryGain);
        //     destinationBuffer.addFrom(ch, 0, drySignal, ch, 0, numSamples);
        // }
        auto dryData = drySignal.getArrayOfWritePointers();
        auto destData = destinationBuffer.getArrayOfWritePointers();
        for(int s = 0; s < numSamples; ++s) {
            dryGain = sqrt(1.0f - dryWetRatio.getNextValue());
            wetGain = sqrt(dryWetRatio.getNextValue());
            for(int ch = 0; ch < numCh; ++ch) {
                dryData[ch][s] *= dryGain;
                destData[ch][s] = destData[ch][s] * wetGain + dryData[ch][s];
            }
        }
    }

    void setDWRatio(float newValue) {
        // dryWetRatio = newValue / 100;
        dryWetRatio.setTargetValue(newValue / 100.0f);
    }

  private:
    // void updateState() {
    //     dryGain = sqrt(1 - dryWetRatio);
    //     wetGain = sqrt(dryWetRatio);
    // }

    // float dryWetRatio = DEFAULT_DRY_WET;
    float dryGain = 0.0f;
    float wetGain = 0.0f;

    AudioBuffer<float> drySignal;

    SmoothedValue<float, ValueSmoothingTypes::Linear> dryWetRatio;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)
};
