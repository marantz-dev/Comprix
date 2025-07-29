#pragma once

#include <JuceHeader.h>

namespace Parameters {
    static const String nameSidechainSwitch = "SCB";
    static const String nameSidechainListen = "SCL";
    static const String nameAttack = "ATK";
    static const String nameRelease = "REL";
    static const String nameThreshold = "THR";
    static const String nameRatio = "RAT";
    static const String nameMakeup = "MU";
    static const String nameDetector = "DET";
    static const String nameKnee = "KNE";
    static const String nameFilterCutoff = "FC";
    static const String nameFilterQuality = "FQ";

    static const float defaultAmount = 1.0f;
    static const float defaultMakeup = 0.0f;
    static const float defaultThreshold = 0.0f;
    static const float defaultRmsTime = 0.05f;
    static const float defaultAttack = 1.0f;
    static const float defaultRelease = 30.0f;
    static const float defaultRatio = 4.0f;
    static const float defaultDetector = 0.0f;        // 0 for RMS, 1 for Peak
    static const float defaultKnee = 6.0f;            // in dB
    static const float defaultFilterCutoff = 1000.0f; // in Hz
    static const float defaultFilterQuality = 1.0f;   // Q

    static const float maxRmsTime = 0.5f;
    static const float minRmsTime = 0.01f;
    static const float minAttack = 0.01f;
    static const float maxAttack = 1000.0f;
    static const float minRelease = 1.0f;
    static const float maxRelease = 3000.0f;
    static const float minMakeup = -36.0f;
    static const float maxMakeup = 36.0f;
    static const float minThreshold = -60.0f;
    static const float maxThreshold = 6.0f;
    static const float minRatio = 1.0f;
    static const float maxRatio = 20.0f;
    static const float minKnee = 0.01f;
    static const float maxKnee = 36.0f;
    static const float minFilterCutoff = 20.0f;    // in Hz
    static const float maxFilterCutoff = 20000.0f; // in Hz
    static const float minFilterQuality = 0.1f;    // Q factor
    static const float maxFilterQuality = 10.0f;   // Q factor

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;

        int id = 1;

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameAttack, id++), "Attack (ms)",
         NormalisableRange<float>(minAttack, maxAttack, 0.01f, 0.2f), defaultAttack));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameRelease, id++), "Release (ms)",
         NormalisableRange<float>(minRelease, maxRelease, 0.01f, 0.2f), defaultRelease));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameThreshold, id++), "Threshold (dB)",
         NormalisableRange<float>(minThreshold, maxThreshold, 0.1f, 1.6f), defaultThreshold));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameRatio, id++), "Ratio",
         NormalisableRange<float>(minRatio, maxRatio, 0.1f, 0.5f), defaultRatio));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameKnee, id++), "Knee Width (dB)",
         NormalisableRange<float>(minKnee, maxKnee, 0.01f, 0.5f), defaultKnee));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameMakeup, id++), "Makeup (dB)",
         NormalisableRange<float>(minMakeup, maxMakeup, 0.1f), defaultMakeup));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameFilterCutoff, id++), "Filter Cutoff (Hz)",
         NormalisableRange<float>(minFilterCutoff, maxFilterCutoff, 1.0f, 0.5f),
         defaultFilterCutoff));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameFilterQuality, id++), "Filter Quality (Q)",
         NormalisableRange<float>(minFilterQuality, maxFilterQuality, 0.01f, 0.5f),
         defaultFilterQuality));

        params.push_back(
         std::make_unique<AudioParameterBool>(ParameterID("SCB", id++), "Sidechain ON/OF", false));

        params.push_back(
         std::make_unique<AudioParameterBool>(ParameterID("SCL", id++), "Sidechain Listen", false));

        params.push_back(std::make_unique<AudioParameterChoice>(
         ParameterID(nameDetector, id++), "Detector", StringArray{"RMS", "Peak"}, defaultDetector));

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
