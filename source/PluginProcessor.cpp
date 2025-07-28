#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"
#include <cstdio>

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
}

void comprixAudioProcessor::releaseResources() {
    compressor.releaseResources();
    auxBuffer.setSize(0, 0); // Clear the aux buffer
}

void comprixAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                         juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    auto mainBuffer = getBusBuffer(buffer, true, 0);
    auto sidechainBuffer = getBusBuffer(buffer, true, 1);
    auto numSamples = buffer.getNumSamples();
    auto numChannels = mainBuffer.getNumChannels();

    // TODO: Fix sidechain handling
    AudioBuffer<float> &scSource = mainBuffer;
    auxBuffer.clear();

    for(int ch = 0; ch < numChannels; ++ch)
        auxBuffer.addFrom(0, 0, scSource, ch, 0, numSamples, 1.0 / numChannels);

    compressor.processBlock(mainBuffer, auxBuffer);
}

bool comprixAudioProcessor::hasEditor() const {
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *comprixAudioProcessor::createEditor() {
    // return new comprixAudioProcessorEditor(*this);
    return nullptr;
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
    } else if(paramID == "SCB") {
        // Handle sidechain toggle
    }
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new comprixAudioProcessor(); }
