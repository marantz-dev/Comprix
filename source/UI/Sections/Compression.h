
#pragma once

#include <JuceHeader.h>
#include "PluginParameters.h"
#include "UIutils.h"

class CompressorSection : public juce::Component {
  public:
    CompressorSection(AudioProcessorValueTreeState &apvts) : vts(apvts) {
        // Group box
        addAndMakeVisible(compressorSectionBorder);
        compressorSectionBorder.setText("Compressor");
        compressorSectionBorder.setTextLabelPosition(juce::Justification::centred);

        // ###################
        // #                 #
        // #  SETUP SLIDERS  #
        // #                 #
        // ###################

        UIutils::setupSlider(attackSlider, juce::Slider::RotaryHorizontalVerticalDrag, Parameters::minAttack,
                             Parameters::maxAttack, Parameters::defaultAttack, Parameters::stepSizeAttack, " ms",
                             Parameters::skewFactorAttack, attackLabel, "Attack");
        addAndMakeVisible(attackSlider);
        addAndMakeVisible(attackLabel);

        UIutils::setupSlider(releaseSlider, juce::Slider::RotaryHorizontalVerticalDrag, Parameters::minRelease,
                             Parameters::maxRelease, Parameters::defaultRelease, Parameters::stepSizeRelease, " ms",
                             Parameters::skewFactorRelease, releaseLabel, "Release");
        addAndMakeVisible(releaseSlider);
        addAndMakeVisible(releaseLabel);

        UIutils::setupSlider(thresholdSlider, juce::Slider::RotaryHorizontalVerticalDrag, Parameters::minThreshold,
                             Parameters::maxThreshold, Parameters::defaultThreshold, Parameters::stepSizeThreshold,
                             " dB", Parameters::skewFactorThreshold, thresholdLabel, "Threshold");
        addAndMakeVisible(thresholdSlider);
        addAndMakeVisible(thresholdLabel);

        UIutils::setupSlider(ratioSlider, juce::Slider::RotaryHorizontalVerticalDrag, Parameters::minRatio,
                             Parameters::maxRatio, Parameters::defaultRatio, Parameters::stepSizeRatio, ":1",
                             Parameters::skewFactorRatio, ratioLabel, "Ratio");
        addAndMakeVisible(ratioSlider);
        addAndMakeVisible(ratioLabel);

        UIutils::setupSlider(makeupSlider, juce::Slider::RotaryHorizontalVerticalDrag, Parameters::minMakeup,
                             Parameters::maxMakeup, Parameters::defaultMakeup, Parameters::stepSizeMakeup, " dB",
                             Parameters::skewFactorMakeup, makeupLabel, "Makeup");
        addAndMakeVisible(makeupSlider);
        addAndMakeVisible(makeupLabel);

        UIutils::setupSlider(dryWetSlider, juce::Slider::RotaryHorizontalVerticalDrag, 0.0f, Parameters::maxDryWet,
                             Parameters::defaultDryWet, Parameters::stepSizeDryWet, " %", Parameters::skewFactorDryWet,
                             dryWetLabel, "Dry/Wet");
        addAndMakeVisible(dryWetSlider);
        addAndMakeVisible(dryWetLabel);

        UIutils::setupSlider(kneeSlider, juce::Slider::LinearHorizontal, Parameters::minKnee, Parameters::maxKnee,
                             Parameters::defaultKnee, Parameters::stepSizeKnee, " dB", Parameters::skewFactorKnee,
                             kneeLabel, "Knee Width");
        kneeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
        kneeSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
        addAndMakeVisible(kneeSlider);
        addAndMakeVisible(kneeLabel);

        // ##################
        // #                #
        // #  SETUP BUTTON  #
        // #                #
        // ##################

        // peakDetectorButton.setButtonText("Peak");
        // peakDetectorButton.setClickingTogglesState(true);
        // peakDetectorButton.setToggleState(true, juce::dontSendNotification);
        // peakDetectorButton.setTooltip("Use Peak detection for compression");
        // peakDetectorButton.setClickingTogglesState(true);
        UIutils::setupToggleButton(peakDetectorButton, "Peak");
        peakDetectorButton.setToggleState(true, juce::dontSendNotification);
        addAndMakeVisible(peakDetectorButton);

        UIutils::setupToggleButton(rmsDetectorButton, "RMS");
        addAndMakeVisible(rmsDetectorButton);

        // ########################
        // #                      #
        // #  SETUP ATTACHEMENTS  #
        // #                      #
        // ########################

        attackAttachment.reset(new SliderAttachment(vts, Parameters::nameAttack, attackSlider));
        releaseAttachment.reset(new SliderAttachment(vts, Parameters::nameRelease, releaseSlider));
        thresholdAttachment.reset(new SliderAttachment(vts, Parameters::nameThreshold, thresholdSlider));
        ratioAttachment.reset(new SliderAttachment(vts, Parameters::nameRatio, ratioSlider));
        makeupAttachment.reset(new SliderAttachment(vts, Parameters::nameMakeup, makeupSlider));
        kneeAttachment.reset(new SliderAttachment(vts, Parameters::nameKnee, kneeSlider));
        dryWetAttachment.reset(new SliderAttachment(vts, Parameters::nameDryWet, dryWetSlider));

        rmsDetectorButton.onClick = [this]() {
            vts.getParameter(Parameters::nameDetector)->setValueNotifyingHost(0.0f);
            updateDetectorButtons();
        };
        peakDetectorButton.onClick = [this]() {
            vts.getParameter(Parameters::nameDetector)->setValueNotifyingHost(1.0f);
            updateDetectorButtons();
        };
    }

    ~CompressorSection() override {
        attackAttachment.reset();
        releaseAttachment.reset();
        thresholdAttachment.reset();
        ratioAttachment.reset();
        makeupAttachment.reset();
        kneeAttachment.reset();
        dryWetAttachment.reset();
        peakDetectorAttachment.reset();
        rmsDetectorAttachment.reset();
    }

    void paint(juce::Graphics &g) override {
        g.setColour(juce::Colours::white);
        g.drawLine(10.0f, getHeight() / 2.0f, getWidth() - 10.0f, getHeight() / 2.0f, 1.0f);
    }
    void resized() override {
        auto bounds = getLocalBounds();
        compressorSectionBorder.setBounds(bounds);

        bounds.reduce(30, 30);

        auto topRow = bounds.removeFromTop(bounds.getHeight() / 2);
        auto bottomRow = bounds;

        auto knobWidth = topRow.getWidth() / 4;
        attackSlider.setBounds(topRow.removeFromLeft(knobWidth).reduced(5));
        releaseSlider.setBounds(topRow.removeFromLeft(knobWidth).reduced(5));
        thresholdSlider.setBounds(topRow.removeFromLeft(knobWidth).reduced(5));
        ratioSlider.setBounds(topRow.reduced(5));

        auto leftBottom = bottomRow.removeFromLeft(bottomRow.getWidth() / 2);
        auto rightBottom = bottomRow;

        auto buttonWidth = leftBottom.getWidth() / 2;
        auto buttonHeight = 40;
        int buttonSpacing = 10;

        leftBottom.removeFromTop(buttonSpacing);

        auto buttonBounds = leftBottom;
        rmsDetectorButton.setBounds(buttonBounds.removeFromLeft(buttonWidth).withHeight(buttonHeight).reduced(5));
        peakDetectorButton.setBounds(buttonBounds.withHeight(buttonHeight).reduced(5));

        leftBottom.removeFromTop(buttonHeight + buttonSpacing);
        kneeSlider.setBounds(leftBottom.reduced(5));

        auto kneeWidth = rightBottom.getWidth() / 2;
        rightBottom.removeFromTop(buttonSpacing);
        makeupSlider.setBounds(rightBottom.removeFromLeft(kneeWidth).reduced(5));
        dryWetSlider.setBounds(rightBottom.reduced(5));

        UIutils::attachLabel(attackLabel, &attackSlider);
        UIutils::attachLabel(releaseLabel, &releaseSlider);
        UIutils::attachLabel(thresholdLabel, &thresholdSlider);
        UIutils::attachLabel(ratioLabel, &ratioSlider);
        UIutils::attachLabel(makeupLabel, &makeupSlider, 12);
        UIutils::attachLabel(dryWetLabel, &dryWetSlider, 12);
        UIutils::attachLabel(kneeLabel, &kneeSlider);
    }

  private:
    void updateDetectorButtons() {
        auto *param = dynamic_cast<AudioParameterChoice *>(vts.getParameter(Parameters::nameDetector));
        int value = param->getIndex();
        rmsDetectorButton.setToggleState(value == 0, juce::dontSendNotification);
        peakDetectorButton.setToggleState(value == 1, juce::dontSendNotification);
    }

    juce::GroupComponent compressorSectionBorder;
    AudioProcessorValueTreeState &vts;

    Slider attackSlider;
    Slider releaseSlider;
    Slider thresholdSlider;
    Slider ratioSlider;
    Slider makeupSlider;
    Slider kneeSlider;
    Slider dryWetSlider;

    Label attackLabel;
    Label releaseLabel;
    Label thresholdLabel;
    Label ratioLabel;
    Label makeupLabel;
    Label kneeLabel;
    Label dryWetLabel;

    ToggleButton peakDetectorButton;
    ToggleButton rmsDetectorButton;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    std::unique_ptr<SliderAttachment> thresholdAttachment;
    std::unique_ptr<SliderAttachment> ratioAttachment;
    std::unique_ptr<SliderAttachment> makeupAttachment;
    std::unique_ptr<SliderAttachment> kneeAttachment;
    std::unique_ptr<SliderAttachment> dryWetAttachment;
    std::unique_ptr<ButtonAttachment> peakDetectorAttachment;
    std::unique_ptr<ButtonAttachment> rmsDetectorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorSection)
};
