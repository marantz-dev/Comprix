#pragma once
#include <JuceHeader.h>
#include "DynamicProcessors.h"
#include "Filters.h"
#include "DryWet.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "GainReductionVisualizer.h"

class ComprixAudioProcessor : public juce::AudioProcessor, public AudioProcessorValueTreeState::Listener {
  public:
    ComprixAudioProcessor();
    ~ComprixAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return false; }

    bool producesMidi() const override { return false; }

    bool isMidiEffect() const override { return false; }

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }

    int getCurrentProgram() override { return 0; }

    void setCurrentProgram(int index) override {}

    const juce::String getProgramName(int index) override { return {}; }

    void changeProgramName(int index, const juce::String &newName) override {}

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    void updateProbe(juce::Atomic<float> &probe, const juce::AudioBuffer<float> &buf, int numSamples) {
        probe.set(jmax(buf.getMagnitude(0, numSamples), probe.get()));
    }

    void clearVisualizersForSidechain() {
        gainReductionVisualiser.clear();
        gainReductionProbe.set(1.0f);
        outputVisualiser.clear();
    }

    void applySidechainGain(AudioBuffer<float> &buffer) {
        auto data = buffer.getArrayOfWritePointers();
        for(int smp = 0; smp < buffer.getNumSamples(); ++smp) {
            auto gain = sidechainGain.getNextValue();
            for(int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                data[ch][smp] *= gain;
            }
        }
    }

    void setSidechainGain(bool listen) {}

    AudioVisualiserComponent outputVisualiser;
    AudioVisualiserComponent inputVisualiser;
    GainReductionVisualiser gainReductionVisualiser;

    Atomic<float> inputProbe;
    Atomic<float> outputProbe;
    Atomic<float> gainReductionProbe;

  private:
    void parameterChanged(const String &paramID, float newValue) override;

    AudioProcessorValueTreeState parameters;

    bool useExternalSidechain = false;
    bool sidechainListen = false;
    bool bypass = false;
    SmoothedValue<float, ValueSmoothingTypes::Linear> sidechainGain;

    AnalogCompressor compressor;
    AudioBuffer<float> auxBuffer;
    StereoFilter filter;
    DryWet drywetter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComprixAudioProcessor)
};
