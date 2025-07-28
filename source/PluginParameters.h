#pragma once

#include <JuceHeader.h>

namespace Parameters {
    static const String nameSidechainSwitch = "SCS";
    static const String nameAttack = "ATK";
    static const String nameRelease = "REL";
    static const String nameThreshold = "THR";
    static const String nameRatio = "RAT";
    static const String nameMakeup = "MU";

    static const float defaultAmount = 1.0f;
    static const float defaultMakeup = 0.0f;
    static const float defaultThreshold = -20.0f;
    static const float defaultRmsTime = 0.05f;
    static const float defaultAttack = 1.0f;
    static const float defaultRelease = 0.1f;
    static const float defaultRatio = 2.0f;

    static const float maxRmsTime = 0.5f;
    static const float minRmsTime = 0.01f;
    static const float minAttack = 0.0f;
    static const float maxAttack = 15.0f;
    static const float minRelease = 0.0f;
    static const float maxRelease = 1.0f;
    static const float minMakeup = -6.0f;
    static const float maxMakeup = 24.0f;
    static const float minThreshold = -60.0f;
    static const float maxThreshold = 0.0f;
    static const float minRatio = 1.0f;
    static const float maxRatio = 20.0f;

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;

        int id = 1;

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameAttack, id++), "Attack (ms)",
         NormalisableRange<float>(minAttack, maxAttack, 0.01f, 0.5), defaultAttack));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameRelease, id++), "Release (ms)",
         NormalisableRange<float>(minRelease, maxRelease, 0.01f, 0.5), defaultRelease));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameThreshold, id++), "Threshold (dB)",
         NormalisableRange<float>(minThreshold, maxThreshold, 0.1f, 0.5), defaultThreshold));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameRatio, id++), "Ratio",
         NormalisableRange<float>(minRatio, maxRatio, 0.1f, 0.5), defaultRatio));
        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameMakeup, id++), "Makeup (dB)",
         NormalisableRange<float>(minMakeup, maxMakeup, 0.1f, 0.5), defaultMakeup));

        params.push_back(
         std::make_unique<AudioParameterBool>(ParameterID("SCB", id++), "Sidechain ON/OF", false));

        return {params.begin(), params.end()};
    }

    static void addListeners(AudioProcessorValueTreeState &valueTreeState,
                             AudioProcessorValueTreeState::Listener *listener) {
        std::unique_ptr<XmlElement> xml(valueTreeState.copyState().createXml());

        for(auto *element : xml->getChildWithTagNameIterator("PARAM")) {
            const String &id = element->getStringAttribute("id");
            valueTreeState.addParameterListener(id, listener);
        }
    }
} // namespace Parameters
