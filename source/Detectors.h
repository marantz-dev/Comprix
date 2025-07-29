#pragma once
#include <JuceHeader.h>
#include <cstdlib>

class RRMS {
  public:
    RRMS() {}
    void prepareToPlay(double sr) {
        sampleRate = sr;
        rmsBuffer.setSize(1, static_cast<int>(sr * maxTime));
        reset();
    }
    void releaseResources() { rmsBuffer.setSize(0, 0); }
    void processBlock(AudioBuffer<float> &buffer, int numSamples) {
        auto bufferData = buffer.getWritePointer(0);
        auto rmsData = rmsBuffer.getWritePointer(0);
        FloatVectorOperations::multiply(bufferData, bufferData, numSamples);
        FloatVectorOperations::multiply(bufferData, 1.0f / windowSize, numSamples);
        for(int smp = 0; smp < numSamples; ++smp) {
            rmsAvg = bufferData[smp] - rmsData[rmsIndex];
            rmsData[rmsIndex] = bufferData[smp];
            bufferData[smp] = sqrt(rmsAvg);
            ++rmsIndex %= windowSize;
        }
    }
    void setRMStime(float time) {}

    ~RRMS() {}

  private:
    void reset() {
        windowSize = jlimit(1, roundToInt(sampleRate * maxTime), roundToInt(sampleRate * rmsTime));
        rmsBuffer.setSize(1, windowSize);
        rmsBuffer.clear();
        rmsIndex = 0;
        rmsAvg = 0.0;
    }

    float rmsTime = 0.0f;
    double sampleRate = 44100.0f;
    AudioBuffer<float> rmsBuffer;
    double rmsAvg = 0.0;
    float maxTime = 1.0f;
    int rmsIndex = 0;
    int windowSize = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RRMS)
};

class SimplePeakDetector {
  public:
    SimplePeakDetector() {}
    void prepareToPlay(double sr) {
        sampleRate = sr;
        reset();
    }
    void releaseResources() {}
    void processBlock(AudioBuffer<float> &peakBuffer, int numSamples) {
        auto bufferData = peakBuffer.getWritePointer(0);
        for(int smp = 0; smp < numSamples; ++smp) {
            peakValue = alphaCoeff * peakValue;
            if(abs(bufferData[smp]) > peakValue) {
                bufferData[smp] = abs(bufferData[smp]);
            } else {
                bufferData[smp] = peakValue;
            }
        }
    }
    void reset() { peakValue = 0.0f; }

  private:
    const float alphaCoeff = 0.2f;
    double sampleRate = 44100.0f;
    float peakValue = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimplePeakDetector)
};
