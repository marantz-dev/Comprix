
#pragma once
#include <JuceHeader.h>

#define DEFAULT_DRY_WET 1.0f

class DryWet {
  public:
    DryWet(float defaultDryWetRatio = DEFAULT_DRY_WET) : dryWetRatio(defaultDryWetRatio) {}

    ~DryWet() {}

    void prepareToPlay(int maxNumSamples) {
        drySignal.setSize(2, maxNumSamples);
        drySignal.clear();

        updateState();
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

        destinationBuffer.applyGain(wetGain);

        for(int ch = 0; ch < numCh; ++ch) {
            drySignal.applyGain(ch, 0, numSamples, dryGain);
            destinationBuffer.addFrom(ch, 0, drySignal, ch, 0, numSamples);
        }
    }

    void setDWRatio(float newValue) {
        dryWetRatio = newValue / 100;
        updateState();
    }

  private:
    void updateState() {
        dryGain = sqrt(1 - dryWetRatio);
        wetGain = sqrt(dryWetRatio);
    }

    float dryWetRatio = DEFAULT_DRY_WET;
    float dryGain = 0.0f;
    float wetGain = 0.0f;

    AudioBuffer<float> drySignal;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)
};
