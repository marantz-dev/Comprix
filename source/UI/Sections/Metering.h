

#pragma once
#include "PluginProcessor.h"
#include "Meters.h"
#include <JuceHeader.h>

class MeteringSection : public juce::Component {
  public:
    MeteringSection(AudioProcessorValueTreeState &vts, ComprixAudioProcessor &p)
        : audioProcessor(p), valueTreeState(vts) {
        // Section Border
        addAndMakeVisible(meteringSectionBorder);
        meteringSectionBorder.setTextLabelPosition(juce::Justification::centredTop);
        meteringSectionBorder.setText("Meters");

        // ########################################
        // #                                      #
        // #  SETUP METERS AND CONNECT TO PROBES  #
        // #                                      #
        // ########################################

        addAndMakeVisible(inputMeter);
        inputMeter.connectTo(p.inputProbe);

        addAndMakeVisible(outputMeter);
        outputMeter.connectTo(p.outputProbe);

        addAndMakeVisible(gainReductionMeter);
        gainReductionMeter.connectTo(p.gainReductionProbe);

        inputLabel.setText("IN", juce::dontSendNotification);
        inputLabel.setJustificationType(juce::Justification::centred);

        outputLabel.setText("OUT", juce::dontSendNotification);
        outputLabel.setJustificationType(juce::Justification::centred);

        gainReductionLabel.setText("GR", juce::dontSendNotification);
        gainReductionLabel.setJustificationType(juce::Justification::centred);
    }

    void resized() override {
        auto bounds = getLocalBounds();
        meteringSectionBorder.setBounds(bounds);
        bounds.reduce(30, 30);

        auto meterHeight = bounds.getHeight() - 30;
        auto meterWidth = bounds.getWidth() / 5;
        auto meterSpacing = 30;

        auto inputArea = bounds;
        inputArea.removeFromRight(2 * meterWidth + 2 * meterSpacing);
        inputMeter.setBounds(inputArea.withSizeKeepingCentre(meterWidth, meterHeight));

        auto sidechainArea = bounds;
        sidechainArea.removeFromLeft(meterWidth + meterSpacing);
        sidechainArea.removeFromRight(meterWidth + meterSpacing);
        gainReductionMeter.setBounds(sidechainArea.withSizeKeepingCentre(meterWidth, meterHeight));

        auto outputArea = bounds;
        outputArea.removeFromLeft(2 * meterWidth + 2 * meterSpacing);
        outputMeter.setBounds(outputArea.withSizeKeepingCentre(meterWidth, meterHeight));

        gainReductionLabel.attachToComponent(&gainReductionMeter, false);
        outputLabel.attachToComponent(&outputMeter, false);
        inputLabel.attachToComponent(&inputMeter, false);
    }

  private:
    ComprixAudioProcessor &audioProcessor;
    AudioProcessorValueTreeState &valueTreeState;

    GroupComponent meteringSectionBorder;

    VolumeMeter inputMeter;
    VolumeMeter outputMeter;
    GainReductionMeter gainReductionMeter;

    Label inputLabel;
    Label outputLabel;
    Label gainReductionLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeteringSection)
};
