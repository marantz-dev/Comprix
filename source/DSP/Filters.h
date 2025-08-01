#pragma once
#include <JuceHeader.h>

#define MAX_NUM_CHANNELS 2
enum FilterType { LowPass, HighPass, BandPass };

class StereoFilter {
  public:
    StereoFilter(const double defaultFrequency = 440.0, const double defaultQuality = 1 / MathConstants<double>::sqrt2,
                 const FilterType defaultFilterType = LowPass)
        : frequency(defaultFrequency), quality(defaultQuality), filterType(defaultFilterType) {
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
        if(!isActive) {
            return;
        }
        dsp::AudioBlock<float> block(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());

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

    void setFilterType(FilterType newType) {
        filterType = newType;
        updateCoefficients();
    }

    void reset() {
        for(int i = 0; i < MAX_NUM_CHANNELS; ++i) {
            iirFilters.getUnchecked(i)->reset();
        }
    }
    void setBypass(const bool newValue) { isActive = newValue; }

  private:
    void updateCoefficients() {
        dsp::IIR::Coefficients<float>::Ptr iirCoefficients;

        switch(filterType) {
        case LowPass:
            iirCoefficients = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, frequency, quality);
            break;
        case HighPass:
            iirCoefficients = dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, frequency, quality);
            break;
        case BandPass:
            iirCoefficients = dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, frequency, quality);
            break;
        default: iirCoefficients = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, frequency, quality); break;
        }

        for(int i = 0; i < MAX_NUM_CHANNELS; ++i) {
            iirFilters.getUnchecked(i)->coefficients = iirCoefficients;
        }
    }

    double frequency;
    double quality;
    double sampleRate = 48000;

    bool isActive = false;

    FilterType filterType;

    OwnedArray<dsp::IIR::Filter<float>> iirFilters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoFilter)
};
