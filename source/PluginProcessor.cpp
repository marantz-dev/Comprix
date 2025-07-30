#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"

//==============================================================================
comprixAudioProcessor::comprixAudioProcessor()
    : AudioProcessor(BusesProperties()
                      .withInput("Input", AudioChannelSet::stereo(), true)
                      .withInput("Sidechain", AudioChannelSet::stereo(), true)
                      .withOutput("Output", AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "ComprixParams", Parameters::createParameterLayout()), compressor()

{
    Parameters::addListeners(parameters, this);
}

comprixAudioProcessor::~comprixAudioProcessor() {}

//==============================================================================
void comprixAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    compressor.prepareToPlay(sampleRate);
    auxBuffer.setSize(1, samplesPerBlock);
    auxBuffer.clear();
    filter.prepareToPlay(sampleRate);
    drywetter.prepareToPlay(sampleRate, samplesPerBlock);
}

void comprixAudioProcessor::releaseResources() {
    compressor.releaseResources();
    auxBuffer.setSize(0, 0); // Clear the aux buffer
    drywetter.releaseResources();
}

void comprixAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                         juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto mainBuffer = getBusBuffer(buffer, true, 0);
    auto sidechainBuffer = getBusBuffer(buffer, true, 1);
    auto numSamples = buffer.getNumSamples();
    auto numChannels = mainBuffer.getNumChannels();

    AudioBuffer<float> &mainSource = mainBuffer;
    AudioBuffer<float> &externalSource = sidechainBuffer;

    auxBuffer.clear();
    if(useExternalSidechain) {
        int sourceChannels = externalSource.getNumChannels();
        for(int ch = 0; ch < sourceChannels; ++ch) {
            auxBuffer.addFrom(0, 0, externalSource, ch, 0, numSamples, 1.0f / sourceChannels);
        }

    } else {
        int sourceChannels = mainSource.getNumChannels();
        for(int ch = 0; ch < sourceChannels; ++ch) {
            auxBuffer.addFrom(0, 0, mainSource, ch, 0, numSamples, 1.0f / sourceChannels);
        }
    }

    if(filterEnabled) {
        filter.processBlock(auxBuffer, numSamples);
    }

    if(sidechainListen) {
        for(int ch = 0; ch < numChannels; ++ch) {
            mainBuffer.copyFrom(ch, 0, auxBuffer, 0, 0, numSamples);
        }
    } else {
        drywetter.copyDrySignal(mainBuffer);
        compressor.processBlock(mainBuffer, auxBuffer);
        drywetter.mixDrySignal(mainBuffer);
    }
}

bool comprixAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *comprixAudioProcessor::createEditor() {
    return new comprixAudioProcessorEditor(*this, parameters);
}

void comprixAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void comprixAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if(xmlState.get() != nullptr)
        if(xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}
bool comprixAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
    if(layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
       && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if(layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    if(layouts.inputBuses[1] != juce::AudioChannelSet::mono()
       && layouts.inputBuses[1] != juce::AudioChannelSet::stereo()
       && layouts.inputBuses[1] != juce::AudioChannelSet::disabled())
        return false;

    return true;
}

void comprixAudioProcessor::parameterChanged(const String &paramID, float newValue) {
    if(paramID == Parameters::nameAttack) {
        compressor.setAttackTime(newValue);
    } else if(paramID == Parameters::nameRelease) {
        compressor.setReleaseTime(newValue);
    } else if(paramID == Parameters::nameThreshold) {
        compressor.setThreshold(newValue);
    } else if(paramID == Parameters::nameRatio) {
        compressor.setRatio(newValue);
    } else if(paramID == Parameters::nameMakeup) {
        compressor.setMakeupGain(newValue);
    } else if(paramID == Parameters::nameSidechainSwitch) {
        useExternalSidechain = newValue > 0.5f;
    } else if(paramID == Parameters::nameSidechainListen) {
        sidechainListen = newValue > 0.5f;
    } else if(paramID == Parameters::nameDetector) {
        int detectorIndex = static_cast<int>(newValue);

        switch(detectorIndex) {
        case 0: compressor.setDetectorType(RMS); break;
        case 1: compressor.setDetectorType(Peak); break;
        // Future detector types:
        // case 2:
        //     compressor.setDetectorType(Envelope);
        //     break;
        default:
            compressor.setDetectorType(RMS); // Fallback to RMS
            break;
        }
    } else if(paramID == Parameters::nameKnee) {
        compressor.setKneeWidth(newValue);
    } else if(paramID == Parameters::nameFilterCutoff) {
        filter.setFrequency(newValue);
    } else if(paramID == Parameters::nameFilterQuality) {
        filter.setQuality(newValue);
    } else if(paramID == Parameters::nameFilterType) {
        int filterTypeIndex = static_cast<int>(newValue);
        switch(filterTypeIndex) {
        case 0: filter.setFilterType(LowPass); break;
        case 1: filter.setFilterType(HighPass); break;
        case 2: filter.setFilterType(BandPass); break;
        default: filter.setFilterType(LowPass); // Fallback to Low Pass
        }
    } else if(paramID == Parameters::nameFilterSwitch) {
        filterEnabled = newValue > 0.5f;
    } else if(paramID == Parameters::nameDryWet) {
        drywetter.setDWRatio(newValue);
    }
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new comprixAudioProcessor(); }
