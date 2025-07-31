
#pragma once
#include "PluginProcessor.h"
#include <JuceHeader.h>
#include <memory>
#include "PluginParameters.h"
#include "juce_graphics/juce_graphics.h"

class ScopeSection : public juce::Component {
  public:
    ScopeSection(AudioProcessorValueTreeState &vts, comprixAudioProcessor &p) : audioProcessor(p), valueTreeState(vts) {
        addAndMakeVisible(scopeSectionBorder);
        scopeSectionBorder.setTextLabelPosition(juce::Justification::centredTop);
        scopeSectionBorder.setText("Scope");

        setupVisualiser(audioProcessor.inputVisualiser, juce::Colours::transparentBlack, Colours::grey.darker());

        setupVisualiser(audioProcessor.outputVisualiser, juce::Colours::transparentBlack, juce::Colours::white);

        setupVisualiser(audioProcessor.sidechainVisualiser, juce::Colours::transparentBlack, Colours::red);

        // Section Border
        addAndMakeVisible(scopeControlsSectionBorder);
        scopeControlsSectionBorder.setText("SCOPE CONTROLS");
        scopeControlsSectionBorder.setTextLabelPosition(juce::Justification::centredTop);

        // Buttons
        setupButton(inButton, "IN");
        setupButton(grButton, "GR");
        setupButton(outButton, "OUT");
        outButton.setToggleState(true, juce::dontSendNotification);
        grButton.setToggleState(true, juce::dontSendNotification);
        inButton.setToggleState(true, juce::dontSendNotification);

        // Slider
        scopeZoomSlider.setSliderStyle(juce::Slider::LinearHorizontal);
        scopeZoomSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        scopeZoomSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
        scopeZoomSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
        scopeZoomSlider.setRange(0.0f, 100.0f, 0.1);
        addAndMakeVisible(scopeZoomSlider);

        scopeZoomSliderAttachment.reset(
         new SliderAttachment(valueTreeState, Parameters::nameScopeZoom, scopeZoomSlider));

        scopeZoomLabel.setText("ZOOM", juce::dontSendNotification);
        scopeZoomLabel.setJustificationType(juce::Justification::centred);
        scopeZoomLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(scopeZoomLabel);

        inButton.onClick = [this]() { audioProcessor.inputVisualiser.setVisible(inButton.getToggleState()); };
        grButton.onClick = [this]() { audioProcessor.sidechainVisualiser.setVisible(grButton.getToggleState()); };
        outButton.onClick = [this]() { audioProcessor.outputVisualiser.setVisible(outButton.getToggleState()); };
    }

    ~ScopeSection() override {}

    void paint(juce::Graphics &g) override {}

    void resized() override {
        auto bounds = getLocalBounds();

        auto scopeControlsSectionBounds = bounds;
        scopeControlsSectionBounds.removeFromLeft(bounds.getWidth() * 0.75f + 7);
        scopeControlsSectionBorder.setBounds(scopeControlsSectionBounds);
        scopeControlsSectionBounds.reduce(30, 30);

        auto buttonHeight = 25;
        auto buttonWidth = 80;
        auto buttonSpacing = 10;
        auto sliderHeight = 30;

        inButton.setBounds(
         scopeControlsSectionBounds.removeFromTop(buttonHeight).withSizeKeepingCentre(buttonWidth, buttonHeight));
        scopeControlsSectionBounds.removeFromTop(buttonSpacing);
        grButton.setBounds(
         scopeControlsSectionBounds.removeFromTop(buttonHeight).withSizeKeepingCentre(buttonWidth, buttonHeight));
        scopeControlsSectionBounds.removeFromTop(buttonSpacing);
        outButton.setBounds(
         scopeControlsSectionBounds.removeFromTop(buttonHeight).withSizeKeepingCentre(buttonWidth, buttonHeight));

        scopeControlsSectionBounds.removeFromTop(15); // spacing before TIME
        scopeZoomLabel.setBounds(scopeControlsSectionBounds.removeFromTop(20));
        scopeZoomSlider.setBounds(scopeControlsSectionBounds.removeFromTop(sliderHeight));

        auto scopeSectionBounds = bounds;
        scopeSectionBounds.removeFromRight(bounds.getWidth() * 0.25f + 7);
        scopeSectionBorder.setBounds(scopeSectionBounds);
        scopeSectionBounds.reduce(0, 30);
        scopeSectionBounds.removeFromLeft(3);
        scopeSectionBounds.removeFromRight(3);

        audioProcessor.inputVisualiser.setBounds(scopeSectionBounds);
        audioProcessor.outputVisualiser.setBounds(scopeSectionBounds);
        audioProcessor.sidechainVisualiser.setBounds(scopeSectionBounds);
    }

  private:
    void setupVisualiser(AudioVisualiserComponent &visualiser, juce::Colour backgroundColour,
                         juce::Colour foregroundColour) {
        addAndMakeVisible(visualiser);
        visualiser.setColours(backgroundColour, foregroundColour);
        visualiser.setRepaintRate(60);
        visualiser.setBufferSize(1024);
        visualiser.setOpaque(false);
    }
    comprixAudioProcessor &audioProcessor;
    AudioProcessorValueTreeState &valueTreeState;
    juce::GroupComponent scopeSectionBorder;

    juce::GroupComponent scopeControlsSectionBorder;

    // Buttons
    juce::ToggleButton inButton;
    juce::ToggleButton grButton;
    juce::ToggleButton outButton;

    // Slider
    juce::Slider scopeZoomSlider;
    juce::Label scopeZoomLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> scopeZoomSliderAttachment;

    void setupButton(juce::ToggleButton &button, const juce::String &text) {
        addAndMakeVisible(button);
        button.setButtonText(text);
        button.setClickingTogglesState(true);
        button.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScopeSection)
};
