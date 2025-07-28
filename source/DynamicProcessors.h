//
// #pragma once
// #include <JuceHeader.h>
// #include "Detectors.h"
//
// class AnalogCompressor {
//   public:
//     AnalogCompressor() {}
//
//     ~AnalogCompressor() {}
//
//     void prepareToPlay(double sr) {
//         sampleRate = sr;
//         rmsDetector.prepareToPlay(sr);
//         alphaCoeffAttack = computeAlphaCoeff(1.0f);
//         alphaCoeffRelease = computeAlphaCoeff(100.0f);
//         predGain = 1.0f; // Initialize predicted gain
//     }
//
//     void releaseResources() { rmsDetector.releaseResources(); }
//
//     void processBlock(AudioBuffer<float> &mainBuffer, AudioBuffer<float> &sidechainBuffer) {
//         const auto numSamples = mainBuffer.getNumSamples();
//         rmsDetector.processBlock(sidechainBuffer, numSamples);
//         convertDetectorToDecibels(sidechainBuffer, numSamples);
//         computeGain(sidechainBuffer, numSamples);
//         convertDecibelsToLinear(sidechainBuffer, numSamples);
//         smoothGainEnvelope(sidechainBuffer, numSamples);
//         applyGain(mainBuffer, sidechainBuffer);
//     }
//
//     void setThreshold(float newThreshold) { threshold = newThreshold; }
//
//     void setRatio(float newRatio) { ratio = newRatio; }
//
//     void setAttackTime(float newAttackTime) { alphaCoeffAttack =
//     computeAlphaCoeff(newAttackTime); }
//
//     void setReleaseTime(float newReleaseTime) {
//         alphaCoeffRelease = computeAlphaCoeff(newReleaseTime);
//     }
//
//     void setMakeupGain(float newMakeupGain) {
//         makeupGain = Decibels::decibelsToGain(newMakeupGain);
//     }
//
//   private:
//     void smoothGainEnvelope(AudioBuffer<float> &buffer, int numSamples) {
//         auto sidechainData = buffer.getWritePointer(0);
//         for(int smp = 0; smp < numSamples; ++smp) {
//             if(sidechainData[smp] > predGain) {
//                 sidechainData[smp]
//                  = alphaCoeffAttack * predGain + (1.0f - alphaCoeffAttack) * sidechainData[smp];
//             } else {
//                 sidechainData[smp]
//                  = alphaCoeffRelease * predGain + (1.0f - alphaCoeffRelease) *
//                  sidechainData[smp];
//             }
//             predGain = sidechainData[smp];
//         }
//     }
//
//     void computeGain(AudioBuffer<float> &buffer, int numSamples) {
//         auto sidechainData = buffer.getWritePointer(0);
//         FloatVectorOperations::add(sidechainData, -threshold, numSamples);
//         FloatVectorOperations::max(sidechainData, sidechainData, 0.0f, numSamples);
//         FloatVectorOperations::multiply(sidechainData, (1 - (1 / ratio)), numSamples);
//         FloatVectorOperations::multiply(sidechainData, -1.0f, numSamples);
//     }
//
//     void convertDetectorToDecibels(AudioBuffer<float> &buffer, int numSamples) {
//         auto sidechainData = buffer.getWritePointer(0);
//         for(int smp = 0; smp < numSamples; ++smp) {
//             sidechainData[smp] = Decibels::gainToDecibels(sidechainData[smp]);
//         }
//     }
//
//     void convertDecibelsToLinear(AudioBuffer<float> &buffer, int numSamples) {
//         auto sidechainData = buffer.getWritePointer(0);
//         for(int smp = 0; smp < numSamples; ++smp) {
//             sidechainData[smp] = Decibels::decibelsToGain(sidechainData[smp]);
//         }
//     }
//
//     void applyGain(AudioBuffer<float> &mainBuffer, AudioBuffer<float> &sidechainBuffer) {
//         auto mainData = mainBuffer.getArrayOfWritePointers();
//         auto scData = sidechainBuffer.getReadPointer(0);
//
//         auto numSamples = mainBuffer.getNumSamples();
//         auto numCh = mainBuffer.getNumChannels();
//
//         for(int ch = 0; ch < numCh; ++ch) {
//             FloatVectorOperations::multiply(mainData[ch], scData, numSamples);
//         }
//         mainBuffer.applyGain(makeupGain); // Apply makeup gain
//     }
//
//     float computeAlphaCoeff(float timeMs) {
//         float tau = timeMs / 1000.0;
//         return exp(-1.0 / (tau * sampleRate));
//     }
//
//     RRMS rmsDetector;
//     float threshold = -20.0f;
//     float ratio = 2.0f;
//     float alphaCoeffAttack;
//     float alphaCoeffRelease;
//     float makeupGain = 1.0f; // Linear Gain
//     double sampleRate = 44100.0;
//     float predGain = 1.0f;
//
//     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogCompressor)
// };

#pragma once
#include <JuceHeader.h>
#include "Detectors.h"

class AnalogCompressor {
  public:
    AnalogCompressor() {}
    ~AnalogCompressor() {}

    void prepareToPlay(double sr) {
        sampleRate = sr;
        rmsDetector.prepareToPlay(sr);
        alphaCoeffAttack = computeAlphaCoeff(1.0f);
        alphaCoeffRelease = computeAlphaCoeff(100.0f);
        predGain = 1.0f; // Initialize to unity gain (linear)
    }

    void releaseResources() { rmsDetector.releaseResources(); }

    void processBlock(AudioBuffer<float> &mainBuffer, AudioBuffer<float> &sidechainBuffer) {
        const auto numSamples = mainBuffer.getNumSamples();

        // Step 1: RMS Detection
        rmsDetector.processBlock(sidechainBuffer, numSamples);

        // Step 2: Convert RMS values to decibels
        convertDetectorToDecibels(sidechainBuffer, numSamples);

        // Step 3: Compute gain reduction in dB
        computeGain(sidechainBuffer, numSamples);

        // Step 4: Convert dB gain reduction back to linear
        convertDecibelsToLinear(sidechainBuffer, numSamples);

        // Step 5: Smooth the gain envelope
        smoothGainEnvelope(sidechainBuffer, numSamples);

        // Step 6: Apply gain to main buffer
        applyGain(mainBuffer, sidechainBuffer);
    }

    void setThreshold(float newThreshold) { threshold = newThreshold; }
    void setRatio(float newRatio) { ratio = newRatio; }
    void setAttackTime(float newAttackTime) { alphaCoeffAttack = computeAlphaCoeff(newAttackTime); }
    void setReleaseTime(float newReleaseTime) {
        alphaCoeffRelease = computeAlphaCoeff(newReleaseTime);
    }
    void setMakeupGain(float newMakeupGain) {
        makeupGain = Decibels::decibelsToGain(newMakeupGain);
    }

  private:
    void smoothGainEnvelope(AudioBuffer<float> &buffer, int numSamples) {
        auto sidechainData = buffer.getWritePointer(0);
        for(int smp = 0; smp < numSamples; ++smp) {
            // Fixed: Compare current gain with previous gain correctly
            if(sidechainData[smp] < predGain) { // Attack: gain reduction (smaller values)
                sidechainData[smp]
                 = alphaCoeffAttack * predGain + (1.0f - alphaCoeffAttack) * sidechainData[smp];
            } else { // Release: gain increase (larger values)
                sidechainData[smp]
                 = alphaCoeffRelease * predGain + (1.0f - alphaCoeffRelease) * sidechainData[smp];
            }
            predGain = sidechainData[smp];
        }
    }

    void computeGain(AudioBuffer<float> &buffer, int numSamples) {
        auto sidechainData = buffer.getWritePointer(0);

        // Fixed: Proper compression formula
        for(int smp = 0; smp < numSamples; ++smp) {
            float inputLevel = sidechainData[smp];
            float gainReduction = 0.0f;

            if(inputLevel > threshold) {
                // Amount above threshold
                float overshoot = inputLevel - threshold;
                // Apply compression ratio
                gainReduction = overshoot * (1.0f - (1.0f / ratio));
            }

            // Store negative gain reduction (will be converted to linear multiplier)
            sidechainData[smp] = -gainReduction;
        }
    }

    void convertDetectorToDecibels(AudioBuffer<float> &buffer, int numSamples) {
        auto sidechainData = buffer.getWritePointer(0);
        for(int smp = 0; smp < numSamples; ++smp) {
            // Added safety check for log domain conversion
            if(sidechainData[smp] > 0.0f) {
                sidechainData[smp] = Decibels::gainToDecibels(sidechainData[smp]);
            } else {
                sidechainData[smp] = -100.0f; // Very quiet floor
            }
        }
    }

    void convertDecibelsToLinear(AudioBuffer<float> &buffer, int numSamples) {
        auto sidechainData = buffer.getWritePointer(0);
        for(int smp = 0; smp < numSamples; ++smp) {
            sidechainData[smp] = Decibels::decibelsToGain(sidechainData[smp]);
        }
    }

    void applyGain(AudioBuffer<float> &mainBuffer, AudioBuffer<float> &sidechainBuffer) {
        auto mainData = mainBuffer.getArrayOfWritePointers();
        auto scData = sidechainBuffer.getReadPointer(0);
        auto numSamples = mainBuffer.getNumSamples();
        auto numCh = mainBuffer.getNumChannels();

        // Apply compression gain to each channel
        for(int ch = 0; ch < numCh; ++ch) {
            FloatVectorOperations::multiply(mainData[ch], scData, numSamples);
        }

        // Apply makeup gain
        mainBuffer.applyGain(makeupGain);
    }

    float computeAlphaCoeff(float timeMs) {
        float tau = timeMs / 1000.0f;                                    // Fixed: Use float literal
        return std::exp(-1.0f / (tau * static_cast<float>(sampleRate))); // Added std:: and cast
    }

    RRMS rmsDetector;
    float threshold = -20.0f;
    float ratio = 2.0f;
    float alphaCoeffAttack;
    float alphaCoeffRelease;
    float makeupGain = 1.0f; // Linear Gain
    double sampleRate = 44100.0;
    float predGain = 1.0f; // Fixed: Initialize to unity gain

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogCompressor)
};
