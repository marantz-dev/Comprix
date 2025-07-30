
#pragma once

#include <JuceHeader.h>
#include "../../PluginParameters.h"
#include "juce_gui_basics/juce_gui_basics.h"

class CompressorSection : public juce::Component {
  public:
    CompressorSection(AudioProcessorValueTreeState &vts) {
        // Group box
        addAndMakeVisible(compressorSectionBorder);
        compressorSectionBorder.setText("Compressor");
        compressorSectionBorder.setTextLabelPosition(juce::Justification::centred);

        addAndMakeVisible(attackSlider);
        attackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        attackSlider.setTextValueSuffix(" ms");
        attackSlider.setRange(Parameters::minAttack, Parameters::maxAttack, 0.01);
        attackSlider.setValue(Parameters::defaultAttack);
        attackSlider.setDoubleClickReturnValue(true, Parameters::defaultAttack);

        addAndMakeVisible(releaseSlider);
        releaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        releaseSlider.setTextValueSuffix(" ms");
        releaseSlider.setRange(Parameters::minRelease, Parameters::maxRelease, 0.01);
        releaseSlider.setValue(Parameters::defaultRelease);
        releaseSlider.setDoubleClickReturnValue(true, Parameters::defaultRelease);

        addAndMakeVisible(thresholdSlider);
        thresholdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        thresholdSlider.setTextValueSuffix(" dB");
        thresholdSlider.setRange(Parameters::minThreshold, Parameters::maxThreshold, 0.1);
        thresholdSlider.setValue(Parameters::defaultThreshold);
        thresholdSlider.setDoubleClickReturnValue(true, Parameters::defaultThreshold);

        addAndMakeVisible(ratioSlider);
        ratioSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); // Ratio slider
        ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        ratioSlider.setTextValueSuffix(":1");
        ratioSlider.setRange(Parameters::minRatio, Parameters::maxRatio, 0.1);
        ratioSlider.setValue(Parameters::defaultRatio);
        ratioSlider.setDoubleClickReturnValue(true, Parameters::defaultRatio);

        addAndMakeVisible(makeupSlider);
        makeupSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        makeupSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        makeupSlider.setTextValueSuffix(" dB");
        makeupSlider.setRange(Parameters::minMakeup, Parameters::maxMakeup, 0.1);
        makeupSlider.setValue(Parameters::defaultMakeup);
        makeupSlider.setDoubleClickReturnValue(true, Parameters::defaultMakeup);

        addAndMakeVisible(kneeSlider);
        kneeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        kneeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        kneeSlider.setTextValueSuffix(" dB");
        kneeSlider.setRange(Parameters::minKnee, Parameters::maxKnee, 0.01);
        kneeSlider.setValue(Parameters::defaultKnee);
        kneeSlider.setDoubleClickReturnValue(true, Parameters::defaultKnee);

        addAndMakeVisible(rmsTimeSlider);
        rmsTimeSlider.setSliderStyle(juce::Slider::LinearBar);
        rmsTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        rmsTimeSlider.setTextValueSuffix(" s");
        rmsTimeSlider.setRange(Parameters::minRmsTime, Parameters::maxRmsTime, 0.01);
        rmsTimeSlider.setValue(Parameters::defaultRmsTime);
        // rmsTimeSlider.setDoubleClickReturnValue(true, Parameters::defaultRmsTime);

        addAndMakeVisible(peakTimeSlider);
        peakTimeSlider.setSliderStyle(juce::Slider::LinearBar);
        peakTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        peakTimeSlider.setTextValueSuffix(" s");
        // peakTimeSlider.setRange(Parameters::minPeakTime, Parameters::maxPeakTime, 0.01);
        // peakTimeSlider.setValue(Parameters::defaultPeakTime);
        // peakTimeSlider.setDoubleClickReturnValue(true, Parameters::defaultPeakTime);

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

    void paint(juce::Graphics &g) override {
        g.setColour(juce::Colours::white);
        g.drawLine(10.0f, getHeight() / 2.0f, getWidth() - 10.0f, getHeight() / 2.0f, 1.0f);
    }
    void resized() override {
        auto area = getLocalBounds();
        compressorSectionBorder.setBounds(area);

        area.reduce(10, 10); // Padding inside the group box

        // === Split vertically in two halves ===
        auto topRow = area.removeFromTop(area.getHeight() / 2);
        auto bottomRow = area;

        // === TOP ROW: A R T R ===
        auto knobWidth = topRow.getWidth() / 4;
        attackSlider.setBounds(topRow.removeFromLeft(knobWidth).reduced(5));
        releaseSlider.setBounds(topRow.removeFromLeft(knobWidth).reduced(5));
        thresholdSlider.setBounds(topRow.removeFromLeft(knobWidth).reduced(5));
        ratioSlider.setBounds(topRow.reduced(5));

        // === BOTTOM ROW ===
        auto leftBottom = bottomRow.removeFromLeft(bottomRow.getWidth() / 2);
        auto rightBottom = bottomRow;

        // --- LEFT SIDE: Peak & RMS buttons (big) + small sliders ---
        auto buttonHeight = 40;
        peakDetectorButton.setBounds(leftBottom.removeFromTop(buttonHeight).reduced(5));
        rmsDetectorButton.setBounds(leftBottom.removeFromTop(buttonHeight).reduced(5));

        auto timeSliderHeight = (leftBottom.getHeight() - 5) / 2;
        peakTimeSlider.setBounds(leftBottom.removeFromTop(timeSliderHeight).reduced(5));
        rmsTimeSlider.setBounds(leftBottom.reduced(5));

        // --- RIGHT SIDE: KNEE and MAKEUP sliders ---
        auto kneeWidth = rightBottom.getWidth() / 2;
        kneeSlider.setBounds(rightBottom.removeFromLeft(kneeWidth).reduced(5));
        makeupSlider.setBounds(rightBottom.reduced(5));
    }

  private:
    juce::GroupComponent compressorSectionBorder;

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
