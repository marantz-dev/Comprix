
#pragma once
#include <JuceHeader.h>
#include "Detectors.h"

enum detectorType { RMS, Peak };
class AnalogCompressor {
  public:
    AnalogCompressor() {}

    ~AnalogCompressor() {}

    void prepareToPlay(double sr) {
        sampleRate = sr;
        rmsDetector.prepareToPlay(sr);
        peakDetector.prepareToPlay(sr);

        alphaCoeffAttack.reset(sr, 0.05);
        alphaCoeffRelease.reset(sr, 0.05);
        kneeWidth.reset(sr, 0.05);
        makeupGain.reset(sr, 0.05);

        alphaCoeffAttack.setCurrentAndTargetValue(computeAlphaCoeff(1.0f));
        alphaCoeffRelease.setCurrentAndTargetValue(computeAlphaCoeff(100.0f));
        kneeWidth.setCurrentAndTargetValue(6.0f);
        makeupGain.setCurrentAndTargetValue(1.0f);
    }

    void releaseResources() { rmsDetector.releaseResources(); }

    void processBlock(AudioBuffer<float> &mainBuffer, AudioBuffer<float> &sidechainBuffer) {
        const auto numSamples = mainBuffer.getNumSamples();

        switch(currentDetector) {
        case RMS: rmsDetector.processBlock(sidechainBuffer, numSamples); break;
        case Peak: peakDetector.processBlock(sidechainBuffer, numSamples); break;
        }

        convertDetectorToDecibels(sidechainBuffer, numSamples);
        computeGain(sidechainBuffer, numSamples);
        convertDecibelsToLinear(sidechainBuffer, numSamples);
        smoothGainEnvelope(sidechainBuffer, numSamples);
        applyGainReduction(mainBuffer, sidechainBuffer);
    }

    void setThreshold(float newThreshold) { threshold = newThreshold; }

    void setRatio(float newRatio) { ratio = newRatio; }

    void setAttackTime(float newAttackTime) {
        alphaCoeffAttack.setCurrentAndTargetValue(computeAlphaCoeff(newAttackTime));
    }

    void setReleaseTime(float newReleaseTime) {
        alphaCoeffRelease.setCurrentAndTargetValue(computeAlphaCoeff(newReleaseTime));
    }

    void setMakeupGain(float newMakeupGain) { makeupGain.setTargetValue(Decibels::decibelsToGain(newMakeupGain)); }

    void setDetectorType(detectorType newDetector) { currentDetector = newDetector; }

    void setKneeWidth(float newKneeWidth) { kneeWidth = newKneeWidth; }

  private:
    void smoothGainEnvelope(AudioBuffer<float> &buffer, int numSamples) {
        auto sidechainData = buffer.getWritePointer(0);

        for(int smp = 0; smp < numSamples; ++smp) {
            auto alphaAtk = alphaCoeffAttack.getNextValue();
            auto alphaRel = alphaCoeffRelease.getNextValue();

            if(sidechainData[smp] > predGain)
                sidechainData[smp] = alphaAtk * predGain + (1.0f - alphaAtk) * sidechainData[smp];
            else
                sidechainData[smp] = alphaRel * predGain + (1.0f - alphaRel) * sidechainData[smp];

            predGain = sidechainData[smp];
        }
    }

    void computeGain(AudioBuffer<float> &buffer, int numSamples) {
        auto sidechainData = buffer.getWritePointer(0);

        float compressionFactor = 1.0f - (1.0f / ratio);

        for(int smp = 0; smp < numSamples; ++smp) {
            float kwidth = kneeWidth.getNextValue();
            float kneeStart = threshold - kwidth / 2.0f;
            float kneeEnd = threshold + kwidth / 2.0f;
            float inputLevel = sidechainData[smp];
            float gainReduction = 0.0f;

            if(inputLevel <= kneeStart) {
                gainReduction = 0.0f;
            } else if(inputLevel >= kneeEnd) {
                gainReduction = (inputLevel - threshold) * compressionFactor;
            } else {
                float kneePosition = (inputLevel - kneeStart) / kwidth;
                float kneeGain = kneePosition * kneePosition;
                float overshoot = inputLevel - threshold;
                gainReduction = kneeGain * overshoot * compressionFactor;
            }

            sidechainData[smp] = -gainReduction;
        }
    }

    void applyGainReduction(AudioBuffer<float> &mainBuffer, AudioBuffer<float> &sidechainBuffer) {
        auto mainData = mainBuffer.getArrayOfWritePointers();
        auto scData = sidechainBuffer.getReadPointer(0);

        auto numSamples = mainBuffer.getNumSamples();
        auto numCh = mainBuffer.getNumChannels();

        for(int ch = 0; ch < numCh; ++ch) {
            FloatVectorOperations::multiply(mainData[ch], scData, numSamples);
        }

        for(int smp = 0; smp < numSamples; ++smp) {
            auto mkGain = makeupGain.getNextValue();
            for(int ch = 0; ch < numCh; ++ch) {
                mainData[ch][smp] *= mkGain;
            }
        }
    }

    void convertDetectorToDecibels(AudioBuffer<float> &buffer, int numSamples) {
        auto sidechainData = buffer.getWritePointer(0);

        for(int smp = 0; smp < numSamples; ++smp) {
            sidechainData[smp] = Decibels::gainToDecibels(sidechainData[smp]);
        }
    }

    void convertDecibelsToLinear(AudioBuffer<float> &buffer, int numSamples) {
        auto sidechainData = buffer.getWritePointer(0);

        for(int smp = 0; smp < numSamples; ++smp) {
            sidechainData[smp] = Decibels::decibelsToGain(sidechainData[smp]);
        }
    }

    float computeAlphaCoeff(float timeMs) {
        float tau = timeMs / 1000.0f;
        return exp(-1.0f / (tau * static_cast<float>(sampleRate)));
    }

    RRMS rmsDetector;
    SimplePeakDetector peakDetector;
    detectorType currentDetector = RMS;

    float threshold = 0.0f;
    float ratio = 2.0f;
    float predGain = 1.0f;

    double sampleRate = 44100.0;

    SmoothedValue<float, ValueSmoothingTypes::Linear> alphaCoeffAttack, alphaCoeffRelease, kneeWidth, makeupGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogCompressor)
};
