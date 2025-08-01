#pragma once

#include <JuceHeader.h>
#include "PluginParameters.h"
#include "UIutils.h"

using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

class SidechainSection : public juce::Component {
  public:
    SidechainSection(AudioProcessorValueTreeState &vts, AudioProcessor &processor) {
        sidechainSectionBorder.setText("Sidechain");
        sidechainSectionBorder.setTextLabelPosition(Justification::centredTop);
        addAndMakeVisible(sidechainSectionBorder);

        // ################################
        // #                              #
        // #  SETUP BUTTONS AND COMBOBOX  #
        // #                              #
        // ################################

        externalSidechainButton.setButtonText("External");
        externalSidechainButton.setClickingTogglesState(true);
        addAndMakeVisible(externalSidechainButton);

        sidechainListenButton.setButtonText("SOLO");
        sidechainListenButton.setClickingTogglesState(true);
        addAndMakeVisible(sidechainListenButton);

        filterEnabledButton.setButtonText("Filter");
        filterEnabledButton.setClickingTogglesState(true);
        addAndMakeVisible(filterEnabledButton);

        filterTypeComboBox.addItem("Low-pass", 1);
        filterTypeComboBox.addItem("High-pass", 2);
        filterTypeComboBox.addItem("Band-pass", 3);
        filterTypeComboBox.setSelectedId(1);
        filterTypeComboBox.setEditableText(false);
        addAndMakeVisible(filterTypeComboBox);

        // ###################
        // #                 #
        // #  SETUP SLIDERS  #
        // #                 #
        // ###################

        UIutils::setupSlider(sidechainGainSlider, juce::Slider::RotaryHorizontalVerticalDrag,
                             Parameters::minSidechainGain, Parameters::maxSidechainGain,
                             Parameters::defaultSidechainGain, Parameters::stepSizeSidechainGain, " dB",
                             Parameters::skewFactorSidechainGain, sidechainGainLabel, "Gain");
        addAndMakeVisible(sidechainGainSlider);
        addAndMakeVisible(sidechainGainLabel);

        UIutils::setupSlider(filterCutoffSlider, juce::Slider::RotaryHorizontalVerticalDrag,
                             Parameters::minFilterCutoff, Parameters::maxFilterCutoff, Parameters::defaultFilterCutoff,
                             Parameters::stepSizeFilterCutoff, " Hz", Parameters::skewFactorFilterCutoff,
                             filterCutoffLabel, "Cutoff");
        filterCutoffSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        filterCutoffSlider.setPopupDisplayEnabled(true, false, nullptr);
        addAndMakeVisible(filterCutoffSlider);
        addAndMakeVisible(filterCutoffLabel);

        UIutils::setupSlider(filterQualitySlider, juce::Slider::RotaryHorizontalVerticalDrag,
                             Parameters::minFilterQuality, Parameters::maxFilterQuality,
                             Parameters::defaultFilterQuality, Parameters::stepSizeFilterQuality, "",
                             Parameters::skewFactorFilterQuality, filterQualityLabel, "Quality");
        filterQualitySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        filterQualitySlider.setPopupDisplayEnabled(true, false, nullptr);
        addAndMakeVisible(filterQualitySlider);
        addAndMakeVisible(filterQualityLabel);

        // #######################
        // #                     #
        // #  SETUP ATTACHMENTS  #
        // #                     #
        // #######################

        sidechainGainAttachment.reset(new SliderAttachment(vts, Parameters::nameSidechainGain, sidechainGainSlider));

        filterCutoffAttachment.reset(new SliderAttachment(vts, Parameters::nameFilterCutoff, filterCutoffSlider));

        filterQualityAttachment.reset(new SliderAttachment(vts, Parameters::nameFilterQuality, filterQualitySlider));

        externalSidechainAttachment.reset(
         new ButtonAttachment(vts, Parameters::nameSidechainSwitch, externalSidechainButton));

        sidechainListenAttachment.reset(
         new ButtonAttachment(vts, Parameters::nameSidechainListen, sidechainListenButton));

        filterEnabledAttachment.reset(new ButtonAttachment(vts, Parameters::nameFilterSwitch, filterEnabledButton));

        filterTypeAttachment.reset(new ComboBoxAttachment(vts, Parameters::nameFilterType, filterTypeComboBox));
    }

    ~SidechainSection() override {
        sidechainGainAttachment.reset();
        filterCutoffAttachment.reset();
        filterQualityAttachment.reset();
        externalSidechainAttachment.reset();
        sidechainListenAttachment.reset();
        filterEnabledAttachment.reset();
        filterTypeAttachment.reset();
    }

    void paint(juce::Graphics &g) override {
        g.setColour(juce::Colours::white.withAlpha(0.3f));

        g.drawLine(35, sidechainGainSlider.getBottom() + 10, (float)getWidth() - 30.0f,
                   sidechainGainSlider.getBottom() + 8, 1.0f);
    }

    void resized() override {
        auto bounds = getLocalBounds();
        sidechainSectionBorder.setBounds(bounds);

        bounds.reduce(30, 30);

        const int buttonHeight = bounds.getHeight() * 0.12f;
        const int spacing = 10;
        const int sliderSize = std::max(bounds.getHeight() * 0.35f, bounds.getWidth() * 0.35f);

        auto topRow = bounds.removeFromTop(buttonHeight);
        auto buttonWidth = (topRow.getWidth() - spacing) / 2;

        externalSidechainButton.setBounds(topRow.removeFromLeft(buttonWidth));
        sidechainListenButton.setBounds(topRow.removeFromRight(buttonWidth + spacing));

        bounds.removeFromTop(spacing);
        auto gainArea = bounds.removeFromTop(sliderSize + 20);

        sidechainGainSlider.setBounds(gainArea.withSizeKeepingCentre(sliderSize, sliderSize));

        bounds.removeFromTop(spacing);
        auto middleRow = bounds;

        middleRow.setHeight(buttonHeight);
        filterEnabledButton.setBounds(middleRow.removeFromLeft(buttonWidth - 20 - spacing / 2));
        filterTypeComboBox.setBounds(middleRow.removeFromRight(buttonWidth + spacing / 2 + 20));

        bounds.removeFromTop(spacing + buttonHeight + 20);
        auto bottomRow = bounds;
        auto knobWidth = bottomRow.getWidth() / 2;

        filterCutoffSlider.setBounds(
         bottomRow.removeFromLeft(knobWidth).withSizeKeepingCentre(sliderSize, sliderSize).reduced(10));
        filterQualitySlider.setBounds(
         bottomRow.removeFromRight(knobWidth).withSizeKeepingCentre(sliderSize, sliderSize).reduced(10));

        UIutils::attachLabel(sidechainGainLabel, &sidechainGainSlider);
        UIutils::attachLabel(filterCutoffLabel, &filterCutoffSlider, 12);
        UIutils::attachLabel(filterQualityLabel, &filterQualitySlider, 12);
    }

  private:
    juce::GroupComponent sidechainSectionBorder;

    juce::ToggleButton externalSidechainButton;
    juce::ToggleButton sidechainListenButton;
    juce::ToggleButton filterEnabledButton;

    juce::ComboBox filterTypeComboBox;

    juce::Slider sidechainGainSlider;
    juce::Slider filterCutoffSlider;
    juce::Slider filterQualitySlider;

    Label sidechainGainLabel;
    Label filterCutoffLabel;
    Label filterQualityLabel;

    std::unique_ptr<SliderAttachment> sidechainGainAttachment;
    std::unique_ptr<SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<SliderAttachment> filterQualityAttachment;

    std::unique_ptr<ButtonAttachment> externalSidechainAttachment;
    std::unique_ptr<ButtonAttachment> sidechainListenAttachment;
    std::unique_ptr<ButtonAttachment> filterEnabledAttachment;

    std::unique_ptr<ComboBoxAttachment> filterTypeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainSection)
};
