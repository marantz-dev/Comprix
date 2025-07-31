#pragma once
#include <JuceHeader.h>

// #################
// #               #
// #  RUNNING RMS  #
// #               #
// #################

class RRMS {
  public:
    RRMS(const float defaultRMSTime = 0.01f, const float defaultMaxRMSTime = 1.0f)
        : rmsTime(defaultRMSTime), maxTime(defaultMaxRMSTime) {}

    ~RRMS() {}

    void prepareToPlay(double sr) {
        sampleRate = sr;
        history.setSize(1, roundToInt(maxTime * sampleRate));
        reset();
    }

    void releaseResources() { history.setSize(0, 0); }

    void processBlock(AudioBuffer<float> &buffer, const int numSamples) {
        auto bufferData = buffer.getWritePointer(0);
        auto historyData = history.getWritePointer(0);

        FloatVectorOperations::multiply(bufferData, bufferData, numSamples);

        for(int smp = 0; smp < numSamples; ++smp) {
            avg += bufferData[smp] - historyData[historyIndex];
            historyData[historyIndex] = bufferData[smp];
            bufferData[smp] = sqrt(avg / windowSize);
            historyIndex = (historyIndex + 1) % windowSize;
        }
    }

    void setRMStime(float newValue) {
        rmsTime = newValue;
        reset();
    }

  private:
    void reset() {
        windowSize = jlimit(1, roundToInt(maxTime * sampleRate), roundToInt(rmsTime * sampleRate));
        history.clear();
        historyIndex = 0;
        avg = 0.0f;
    }

    AudioBuffer<float> history;

    int historyIndex = 0;
    int windowSize;

    float avg = 0;
    float rmsTime;
    float maxTime;

    double sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RRMS)
};

// ###################
// #                 #
// #  PEAK DETECTOR  #
// #                 #
// ###################

class SimplePeakDetector {
  public:
    SimplePeakDetector(const float defaultAttackCoeff = 0.01f, const float defaultReleaseCoeff = 0.99f)
        : attackCoeff(defaultAttackCoeff), releaseCoeff(defaultReleaseCoeff) {}
    ~SimplePeakDetector() {}

    void prepareToPlay(double sr) {
        sampleRate = sr;
        setAttackTime(0.1f);
        setReleaseTime(100.0f);
        reset();
    }

    void releaseResources() {}

    void processBlock(AudioBuffer<float> &peakBuffer, int numSamples) {
        auto bufferData = peakBuffer.getWritePointer(0);
        for(int smp = 0; smp < numSamples; ++smp) {
            float inputAbs = std::abs(bufferData[smp]);

            inputAbs = jlimit(0.0f, 10.0f, inputAbs);

            if(inputAbs > peakValue) {
                peakValue = attackCoeff * peakValue + (1.0f - attackCoeff) * inputAbs;
            } else {
                peakValue = releaseCoeff * peakValue + (1.0f - releaseCoeff) * inputAbs;
            }

            peakValue = std::max(peakValue, 1e-6f);

            bufferData[smp] = jlimit(1e-6f, 2.0f, peakValue);
        }
    }

    void setAttackTime(float timeMs) {
        if(sampleRate > 0) {
            float tau = (timeMs / 1000.0f) * static_cast<float>(sampleRate);
            attackCoeff = tau > 0 ? exp(-1.0f / tau) : 0.0f;
        }
    }

    void setReleaseTime(float timeMs) {
        if(sampleRate > 0) {
            float tau = (timeMs / 1000.0f) * static_cast<float>(sampleRate);
            releaseCoeff = tau > 0 ? exp(-1.0f / tau) : 0.0f;
        }
    }

    void reset() { peakValue = 0.0f; }

  private:
    double sampleRate = 44100.0;
    float peakValue = 0.0f;
    float attackCoeff;
    float releaseCoeff;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimplePeakDetector)
};
