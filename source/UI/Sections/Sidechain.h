#pragma once

#include <JuceHeader.h>
#include "../../PluginParameters.h"

class SidechainSection : public juce::Component {
  public:
    SidechainSection(AudioProcessorValueTreeState &vts) {
        // Group box
        addAndMakeVisible(sidechainSectionBorder);
        sidechainSectionBorder.setText("Sidechain");
        sidechainSectionBorder.setTextLabelPosition(juce::Justification::centred);

        // Toggle buttons
        addAndMakeVisible(externalSidechainButton);
        externalSidechainButton.setButtonText("Sidechain");
        externalSidechainButton.setClickingTogglesState(true);

        addAndMakeVisible(sidechainListenButton);
        sidechainListenButton.setButtonText("Listen");
        sidechainListenButton.setClickingTogglesState(true);

        addAndMakeVisible(filterEnabledButton);
        filterEnabledButton.setButtonText("Filter");
        filterEnabledButton.setClickingTogglesState(true);

        // Filter type combo
        addAndMakeVisible(filterTypeComboBox);
        filterTypeComboBox.addItem("Low-pass", 1);
        filterTypeComboBox.addItem("High-pass", 2);
        filterTypeComboBox.addItem("Band-pass", 3);
        filterTypeComboBox.setSelectedId(1);

        // Sliders
        addAndMakeVisible(sidechainGainSlider);
        sidechainGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        sidechainGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        sidechainGainSlider.setTextValueSuffix(" dB");

        addAndMakeVisible(filterCutoffSlider);
        filterCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        filterCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        filterCutoffSlider.setTextValueSuffix(" Hz");
        filterCutoffSlider.setRange(Parameters::minFilterCutoff, Parameters::maxFilterCutoff, 1.0);

        addAndMakeVisible(filterQualitySlider);
        filterQualitySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        filterQualitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        filterQualitySlider.setTextValueSuffix(" Q");
        filterQualitySlider.setRange(Parameters::minFilterQuality, Parameters::maxFilterQuality,
                                     0.01);

        // Attachments
        // sidechainGainAttachment.reset(
        //  new SliderAttachment(vts, Parameters::nameSidechainSwitch, sidechainGainSlider));
        filterCutoffAttachment.reset(
         new SliderAttachment(vts, Parameters::nameFilterCutoff, filterCutoffSlider));
        filterQualityAttachment.reset(
         new SliderAttachment(vts, Parameters::nameFilterQuality, filterQualitySlider));

        externalSidechainAttachment.reset(
         new ButtonAttachment(vts, Parameters::nameSidechainSwitch, externalSidechainButton));
        sidechainListenAttachment.reset(
         new ButtonAttachment(vts, Parameters::nameSidechainListen, sidechainListenButton));
        filterEnabledAttachment.reset(
         new ButtonAttachment(vts, Parameters::nameFilterSwitch, filterEnabledButton));

        filterTypeAttachment.reset(
         new ComboBoxAttachment(vts, Parameters::nameFilterType, filterTypeComboBox));
    }

    ~SidechainSection() override {}

    void paint(juce::Graphics &g) override {
        g.setColour(juce::Colours::white.withAlpha(0.3f));

        auto bounds = getLocalBounds();
        // Example: draw a line across at Y = 200
        g.drawLine(35, sidechainGainSlider.getBottom() + 8, (float)getWidth() - 30.0f,
                   sidechainGainSlider.getBottom() + 8, 1.0f);
    }

    void resized() override {
        auto bounds = getLocalBounds().reduced(10);
        sidechainSectionBorder.setBounds(bounds);

        bounds.reduce(30, 30); // inner padding

        const int buttonHeight = 30;
        const int spacing = 10;
        const int comboHeight = 25;
        const int sliderSize = 100;
        const int dualSliderSpacing = 20;

        // --- TOP ROW: Sidechain + Monitor
        auto topRow = bounds.removeFromTop(buttonHeight);
        auto buttonWidth = (topRow.getWidth() - spacing) / 2;
        externalSidechainButton.setBounds(topRow.removeFromLeft(buttonWidth));
        sidechainListenButton.setBounds(topRow.removeFromLeft(buttonWidth));

        bounds.removeFromTop(spacing);

        // --- GAIN SLIDER CENTERED
        auto gainArea = bounds.removeFromTop(sliderSize + 20);
        sidechainGainSlider.setBounds(gainArea.withSizeKeepingCentre(sliderSize, sliderSize));
        // gainLabel.setBounds(sidechainGainSlider.getX(), sidechainGainSlider.getBottom(),
        // sliderSize, 20);

        bounds.removeFromTop(spacing);

        // --- FILTER TOGGLE BUTTON
        filterEnabledButton.setBounds(
         bounds.removeFromTop(buttonHeight).withSizeKeepingCentre(100, buttonHeight));
        bounds.removeFromTop(spacing);

        // --- FILTER TYPE COMBOBOX
        filterTypeComboBox.setBounds(
         bounds.removeFromTop(comboHeight).withSizeKeepingCentre(100, comboHeight));
        bounds.removeFromTop(spacing * 2);

        // --- C + Q ROTARY SLIDERS
        auto bottomRow = bounds.removeFromTop(sliderSize + 20);
        auto halfWidth = (bottomRow.getWidth() - dualSliderSpacing) / 2;

        filterCutoffSlider.setBounds(
         bottomRow.removeFromLeft(halfWidth).withSizeKeepingCentre(sliderSize, sliderSize));
        // cutoffLabel.setBounds(filterCutoffSlider.getX(), filterCutoffSlider.getBottom(),
        // sliderSize, 20);

        filterQualitySlider.setBounds(
         bottomRow.removeFromLeft(halfWidth).withSizeKeepingCentre(sliderSize, sliderSize));
        // qLabel.setBounds(filterQualitySlider.getX(), filterQualitySlider.getBottom(), sliderSize,
        // 20);
    }

  private:
    juce::GroupComponent sidechainSectionBorder;

    // Toggles
    juce::ToggleButton externalSidechainButton;
    juce::ToggleButton sidechainListenButton;
    juce::ToggleButton filterEnabledButton;

    // ComboBox
    juce::ComboBox filterTypeComboBox;

    // Sliders
    juce::Slider sidechainGainSlider;
    juce::Slider filterCutoffSlider;
    juce::Slider filterQualitySlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    std::unique_ptr<SliderAttachment> sidechainGainAttachment;
    std::unique_ptr<SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<SliderAttachment> filterQualityAttachment;

    std::unique_ptr<ButtonAttachment> externalSidechainAttachment;
    std::unique_ptr<ButtonAttachment> sidechainListenAttachment;
    std::unique_ptr<ButtonAttachment> filterEnabledAttachment;

    std::unique_ptr<ComboBoxAttachment> filterTypeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainSection)
};
