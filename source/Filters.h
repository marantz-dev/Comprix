#pragma once
#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#define MAX_NUM_CHANNELS 2
class StereoFilter {
  public:
    StereoFilter(double defaultFrequency = 440.0,
                 double defaultQuality
                 = 1 / MathConstants<double>::sqrt2 /* int deafultType, ecc.... */)
        : frequency(defaultFrequency), quality(defaultQuality) {
        for(int i = 0; i < MAX_NUM_CHANNELS; ++i) {
            iirFilters.add(new dsp::IIR::Filter<float>());
        }
    }
    ~StereoFilter() {}

    void prepareToPlay(double sr) {
        sampleRate = sr;
        reset();
        updateCoefficients();
    }

    void releaseResources() {}

    void processBlock(AudioBuffer<float> &buffer, const int numSamples) {
        dsp::AudioBlock<float> block(buffer.getArrayOfWritePointers(), buffer.getNumChannels(),
                                     buffer.getNumSamples());

        for(int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            dsp::AudioBlock<float> chBlock = block.getSingleChannelBlock(ch);
            dsp::ProcessContextReplacing<float> context(chBlock);
            iirFilters.getUnchecked(ch)->process(context);
        }
    }

    void setFrequency(double newValue) {
        frequency = jmin(newValue, sampleRate * 0.499);
        updateCoefficients();
    }

    void setQuality(float newValue) {
        quality = newValue;
        updateCoefficients();
    }

    void reset() {
        for(int i = 0; i < MAX_NUM_CHANNELS; ++i) {
            iirFilters.getUnchecked(i)->reset();
        }
    }

  private:
    void updateCoefficients() {
        auto iirCoefficients
         = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, frequency, quality);
        for(int i = 0; i < MAX_NUM_CHANNELS; ++i) {
            iirFilters.getUnchecked(i)->coefficients = iirCoefficients;
        }
    }

    double frequency;
    double quality;
    double sampleRate = 48000;

    // dsp::IIR::Filter<float> iirFilter;

    OwnedArray<dsp::IIR::Filter<float>> iirFilters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoFilter)
};
