
#pragma once

#include <JuceHeader.h>
#include "../../PluginParameters.h"
#include "juce_gui_basics/juce_gui_basics.h"

class CompressorSection : public juce::Component {
  public:
    CompressorSection(AudioProcessorValueTreeState &vts) {
        // Group box
        addAndMakeVisible(sidechainSectionBorder);
        sidechainSectionBorder.setText("Sidechain");
        sidechainSectionBorder.setTextLabelPosition(juce::Justification::centred);

        addAndMakeVisible(attackSlider);
        attackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        attackSlider.setTextValueSuffix(" ms");
        attackSlider.setRange(Parameters::minAttack, Parameters::maxAttack, 0.01);
        attackSlider.setValue(Parameters::defaultAttack);

        addAndMakeVisible(releaseSlider);
        releaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        releaseSlider.setTextValueSuffix(" ms");
        releaseSlider.setRange(Parameters::minRelease, Parameters::maxRelease, 0.01);
        releaseSlider.setValue(Parameters::defaultRelease);

        addAndMakeVisible(thresholdSlider);
        thresholdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        thresholdSlider.setTextValueSuffix(" dB");
        thresholdSlider.setRange(Parameters::minThreshold, Parameters::maxThreshold, 0.1);
        thresholdSlider.setValue(Parameters::defaultThreshold);

        addAndMakeVisible(ratioSlider);
        ratioSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); // Ratio slider
        ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        ratioSlider.setTextValueSuffix(":1");
        ratioSlider.setRange(Parameters::minRatio, Parameters::maxRatio, 0.1);
        ratioSlider.setValue(Parameters::defaultRatio);

        addAndMakeVisible(makeupSlider);
        makeupSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        makeupSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        makeupSlider.setTextValueSuffix(" dB");
        makeupSlider.setRange(Parameters::minMakeup, Parameters::maxMakeup, 0.1);
        makeupSlider.setValue(Parameters::defaultMakeup);

        addAndMakeVisible(kneeSlider);
        kneeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        kneeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        kneeSlider.setTextValueSuffix(" dB");
        kneeSlider.setRange(Parameters::minKnee, Parameters::maxKnee, 0.01);
        kneeSlider.setValue(Parameters::defaultKnee);

        addAndMakeVisible(rmsTimeSlider);
        rmsTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        rmsTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        rmsTimeSlider.setTextValueSuffix(" s");
        rmsTimeSlider.setRange(Parameters::minRmsTime, Parameters::maxRmsTime, 0.01);
        rmsTimeSlider.setValue(Parameters::defaultRmsTime);

        addAndMakeVisible(peakTimeSlider);
        peakTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        peakTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        peakTimeSlider.setTextValueSuffix(" s");
        // peakTimeSlider.setRange(Parameters::minPeakTime, Parameters::maxPeakTime, 0.01);
        // peakTimeSlider.setValue(Parameters::defaultPeakTime);

        addAndMakeVisible(peakDetectorButton);
        peakDetectorButton.setButtonText("Peak");
        peakDetectorButton.setClickingTogglesState(true);
        peakDetectorButton.setToggleState(true, juce::dontSendNotification);
        peakDetectorButton.setTooltip("Use Peak detection for compression");
        peakDetectorButton.setClickingTogglesState(true);

        addAndMakeVisible(rmsDetectorButton);
        rmsDetectorButton.setButtonText("RMS");
        rmsDetectorButton.setClickingTogglesState(true);
        rmsDetectorButton.setToggleState(false, juce::dontSendNotification);
        rmsDetectorButton.setTooltip("Use RMS detection for compression");
        rmsDetectorButton.setClickingTogglesState(true);
    }

    ~CompressorSection() override {}

    void paint(juce::Graphics &g) override {}

    void resized() override {}

  private:
    juce::GroupComponent sidechainSectionBorder;

    Slider attackSlider;
    Slider releaseSlider;
    Slider thresholdSlider;
    Slider ratioSlider;
    Slider makeupSlider;
    Slider kneeSlider;

    Slider rmsTimeSlider;
    Slider peakTimeSlider;

    ToggleButton peakDetectorButton;
    ToggleButton rmsDetectorButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorSection)
};
