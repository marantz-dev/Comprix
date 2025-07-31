#pragma once

#include <JuceHeader.h>

namespace Parameters {
    static const String nameSidechainSwitch = "SCB"; //
    static const String nameSidechainListen = "SCL"; //
    static const String nameAttack = "ATK";          //
    static const String nameRelease = "REL";         //
    static const String nameThreshold = "THR";       //
    static const String nameRatio = "RAT";           //
    static const String nameMakeup = "MU";
    static const String nameDetector = "DET";
    static const String nameKnee = "KNE";
    static const String nameFilterCutoff = "FC";  //
    static const String nameFilterQuality = "FQ"; //
    static const String nameFilterType = "FT";    //
    static const String nameFilterSwitch = "FB";  //
    static const String nameDryWet = "DWR";       //
    static const String nameScopeZoom = "SCT";    //
    static const String nameSidechainGain = "SCG";

    static const float defaultAmount = 1.0f;
    static const float defaultMakeup = 0.0f;
    static const float defaultThreshold = 0.0f;
    static const float defaultRmsTime = 0.01f;
    static const float defaultAttack = 1.0f;
    static const float defaultRelease = 30.0f;
    static const float defaultRatio = 4.0f;
    static const float defaultDetector = 0.0f;        // 0 for RMS, 1 for Peak
    static const float defaultKnee = 6.0f;            // in dB
    static const float defaultFilterCutoff = 1000.0f; // in Hz
    static const float defaultFilterQuality = 1.0f;   // Q
    static const float defaultFilterType = 0.0f;      // 0 for LowPass, 1 for HighPass, 2 for Band
    static const float defaultDryWet = 100.0f;        // in percent
    static const float defaultScopeZoom = 50.0f;      // in %
    static const float defaultSidechainGain = 0.0f;   // in dB

    static const float maxRmsTime = 1.0f;
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
    static const float minDryWet = 0.0f;           // in percent
    static const float maxDryWet = 100.0f;         // in percent
    static const float minSidechainGain = -48.0f;  // in dB
    static const float maxSidechainGain = 24.0f;   // in dB

    // Step size constants for individual parameters
    static const float stepSizeAttack = 0.01f;
    static const float stepSizeRelease = 0.01f;
    static const float stepSizeThreshold = 0.1f;
    static const float stepSizeRatio = 0.01f;
    static const float stepSizeKnee = 0.01f;
    static const float stepSizeMakeup = 0.1f;
    static const float stepSizeFilterCutoff = 1.0f;
    static const float stepSizeFilterQuality = 0.01f;
    static const float stepSizeDryWet = 0.1f;
    static const float stepSizeScopeZoom = 0.1f;     // in percent
    static const float stepSizeSidechainGain = 0.1f; // in dB

    // Skew factor constants for individual parameters
    static const float skewFactorAttack = 0.2f;
    static const float skewFactorRelease = 0.2f;
    static const float skewFactorThreshold = 1.6f;
    static const float skewFactorRatio = 0.5f;
    static const float skewFactorKnee = 0.5f;
    static const float skewFactorMakeup = 1.0f;
    static const float skewFactorFilterCutoff = 0.5f;
    static const float skewFactorFilterQuality = 0.5f;
    static const float skewFactorDryWet = 1.0f;
    static const float skewFactorScopeZoom = 1.0f;     // in percent
    static const float skewFactorSidechainGain = 1.0f; // in dB

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;

        int id = 1;

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameAttack, id++), "Attack (ms)",
         NormalisableRange<float>(minAttack, maxAttack, stepSizeAttack, skewFactorAttack), defaultAttack));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameRelease, id++), "Release (ms)",
         NormalisableRange<float>(minRelease, maxRelease, stepSizeRelease, skewFactorRelease), defaultRelease));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameThreshold, id++), "Threshold (dB)",
         NormalisableRange<float>(minThreshold, maxThreshold, stepSizeThreshold, skewFactorThreshold),
         defaultThreshold));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameRatio, id++), "Ratio",
         NormalisableRange<float>(minRatio, maxRatio, stepSizeRatio, skewFactorRatio), defaultRatio));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameKnee, id++), "Knee Width (dB)",
         NormalisableRange<float>(minKnee, maxKnee, stepSizeKnee, skewFactorKnee), defaultKnee));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameMakeup, id++), "Makeup (dB)",
         NormalisableRange<float>(minMakeup, maxMakeup, stepSizeMakeup, skewFactorMakeup), defaultMakeup));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameFilterCutoff, id++), "Filter Cutoff (Hz)",
         NormalisableRange<float>(minFilterCutoff, maxFilterCutoff, stepSizeFilterCutoff, skewFactorFilterCutoff),
         defaultFilterCutoff));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameFilterQuality, id++), "Filter Quality (Q)",
         NormalisableRange<float>(minFilterQuality, maxFilterQuality, stepSizeFilterQuality, skewFactorFilterQuality),
         defaultFilterQuality));

        params.push_back(std::make_unique<AudioParameterChoice>(ParameterID(nameFilterType, id++), "Filter Type",
                                                                StringArray{"LowPass", "HighPass", "BandPass"},
                                                                defaultFilterType));

        params.push_back(
         std::make_unique<AudioParameterBool>(ParameterID(nameFilterSwitch, id++), "Filter ON/OFF", false));

        params.push_back(std::make_unique<AudioParameterBool>(ParameterID("SCB", id++), "Sidechain ON/OF", false));

        params.push_back(std::make_unique<AudioParameterBool>(ParameterID("SCL", id++), "Sidechain Listen", false));

        params.push_back(std::make_unique<AudioParameterChoice>(ParameterID(nameDetector, id++), "Detector",
                                                                StringArray{"RMS", "Peak"}, defaultDetector));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameDryWet, id++), "Dry/Wet (%)",
         NormalisableRange<float>(minDryWet, maxDryWet, stepSizeDryWet, skewFactorDryWet), defaultDryWet));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameScopeZoom, id++), "Scope Zoom (%)",
         NormalisableRange<float>(0.1f, 100.0f, stepSizeScopeZoom, skewFactorScopeZoom), defaultScopeZoom));

        params.push_back(std::make_unique<AudioParameterFloat>(
         ParameterID(nameSidechainGain, id++), "Sidechain Gain (dB)",
         NormalisableRange<float>(minSidechainGain, maxSidechainGain, stepSizeSidechainGain, skewFactorSidechainGain),
         defaultSidechainGain));

        return {params.begin(), params.end()};
    }

    static void
    addListeners(AudioProcessorValueTreeState &valueTreeState, AudioProcessorValueTreeState::Listener *listener) {
        std::unique_ptr<XmlElement> xml(valueTreeState.copyState().createXml());

        for(auto *element : xml->getChildWithTagNameIterator("PARAM")) {
            const String &id = element->getStringAttribute("id");
            valueTreeState.addParameterListener(id, listener);
        }
    }
} // namespace Parameters
