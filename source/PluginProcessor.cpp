#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"

//==============================================================================
comprixAudioProcessor::comprixAudioProcessor()
    : AudioProcessor(BusesProperties()
                      .withInput("Input", AudioChannelSet::stereo(), true)
                      .withInput("Sidechain", AudioChannelSet::stereo(), true)
                      .withOutput("Output", AudioChannelSet::stereo(), true)),
      outputVisualiser(1), inputVisualiser(1), gainReductionVisualiser(1),
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
    drywetter.prepareToPlay(samplesPerBlock, sampleRate);
}

void comprixAudioProcessor::releaseResources() {
    compressor.releaseResources();
    auxBuffer.setSize(0, 0);
    drywetter.releaseResources();
    outputVisualiser.clear();
    inputVisualiser.clear();
    gainReductionVisualiser.clear();
}

void comprixAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    auto mainBuffer = getBusBuffer(buffer, true, 0);
    auto sidechainBuffer = getBusBuffer(buffer, true, 1);
    const int numSamples = buffer.getNumSamples();
    const int numChannels = mainBuffer.getNumChannels();

    if(sidechainListen) {
        const int scChannels = sidechainBuffer.getNumChannels();
        const int copyChannels = jmin(scChannels, numChannels);
        for(int ch = 0; ch < copyChannels; ++ch)
            mainBuffer.copyFrom(ch, 0, sidechainBuffer, ch, 0, numSamples);

        if(filterEnabled)
            filter.processBlock(mainBuffer, numSamples);
        inputVisualiser.clear();
        gainReductionVisualiser.clear();
        outputVisualiser.pushBuffer(mainBuffer);

        return;
    }

    auxBuffer.clear();
    const auto &source = useExternalSidechain ? sidechainBuffer : mainBuffer;
    const int sourceChannels = source.getNumChannels();
    for(int ch = 0; ch < sourceChannels; ++ch)
        auxBuffer.addFrom(0, 0, source, ch, 0, numSamples, 1.0f / sourceChannels);

    auxBuffer.applyGain(Decibels::decibelsToGain(sideChainGain));
    inputProbe.set(jmax(auxBuffer.getMagnitude(0, numSamples), inputProbe.get()));

    if(filterEnabled)
        filter.processBlock(auxBuffer, numSamples);

    inputVisualiser.pushBuffer(auxBuffer);

    drywetter.copyDrySignal(mainBuffer);
    compressor.processBlock(mainBuffer, auxBuffer);
    drywetter.mixDrySignal(mainBuffer);

    gainReductionVisualiser.pushBuffer(auxBuffer);
    outputVisualiser.pushBuffer(mainBuffer);
    gainReductionProbe.set(jmax(auxBuffer.getMagnitude(0, numSamples), gainReductionProbe.get()));
    outputProbe.set(jmax(mainBuffer.getMagnitude(0, numSamples), outputProbe.get()));
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
    } else if(paramID == Parameters::nameScopeZoom) {
        int newZoom
         = static_cast<int>(128 + (2048 - 128) * ((100 - newValue) / 100.0f)); // Convert percentage to a factor;
        outputVisualiser.setBufferSize(newZoom);
        inputVisualiser.setBufferSize(newZoom);
        gainReductionVisualiser.setBufferSize(newZoom);
    } else if(paramID == Parameters::nameSidechainGain) {
        sideChainGain = newValue;
    }
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new comprixAudioProcessor(); }
