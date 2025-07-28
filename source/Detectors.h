// #pragma once
// #include <JuceHeader.h>
//
// class RRMS {
//   public:
//     RRMS() {}
//     void prepareToPlay(double sr) {
//         // Prepare resources for processing
//         sampleRate = sr;
//         rmsBuffer.setSize(1, static_cast<int>(sr * maxTime));
//         reset();
//     }
//     void releaseResources() {
//         // Release resources after processing
//         rmsBuffer.setSize(0, 0);
//     }
//     void processBlock(AudioBuffer<float> &buffer, int numSamples) {
//         auto bufferData = buffer.getWritePointer(0);
//         auto rmsData = rmsBuffer.getWritePointer(0);
//         FloatVectorOperations::multiply(bufferData, bufferData, numSamples);
//         FloatVectorOperations::multiply(bufferData, 1.0f / windowSize, numSamples);
//         for(int smp = 0; smp < numSamples; ++smp) {
//             rmsAvg = bufferData[smp] - rmsData[rmsIndex];
//             rmsData[rmsIndex] = bufferData[smp];
//             bufferData[smp] = sqrt(rmsAvg);
//             ++rmsIndex %= windowSize;
//         }
//     }
//     void setRMStime(float time) {
//         // Set the RMS time
//     }
//
//     ~RRMS() {}
//
//   private:
//     void reset() {
//         windowSize = jlimit(1, roundToInt(sampleRate * maxTime), roundToInt(sampleRate *
//         rmsTime)); rmsBuffer.setSize(1, windowSize); rmsBuffer.clear(); rmsIndex = 0; rmsAvg =
//         0.0;
//     }
//
//     float rmsTime = 0.0f;
//     double sampleRate = 44100.0f;
//     AudioBuffer<float> rmsBuffer;
//     double rmsAvg = 0.0;
//     float maxTime = 1.0f;
//     int rmsIndex = 0;
//     int windowSize = 0;
//
//     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RRMS)
// };
#pragma once
#include <JuceHeader.h>

class RRMS {
  public:
    RRMS(float defaultTime = 0.05f, float defaultMaxTime = 1.0f)
        : rmsTime(defaultTime), maxTime(defaultMaxTime) {}

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
        FloatVectorOperations::multiply(bufferData, 1.0f / windowSize, numSamples);

        for(int smp = 0; smp < numSamples; ++smp) {
            avg += bufferData[smp] - historyData[historyIndex];
            historyData[historyIndex] = bufferData[smp];
            bufferData[smp] = sqrt(avg);
            ++historyIndex %= windowSize;
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

    float avg = 0;
    AudioBuffer<float> history;
    int historyIndex = 0;
    float maxTime;

    double sampleRate;
    float rmsTime;
    int windowSize;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RRMS)
};
