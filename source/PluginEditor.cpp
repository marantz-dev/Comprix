#include "PluginParameters.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

comprixAudioProcessorEditor::comprixAudioProcessorEditor(comprixAudioProcessor &p,
                                                         AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p), audioProcessor(p), valueTreeState(vts) {
    // ##################
    // #                #
    // #  EXAMPLE DIAL  #
    // #                #
    // ##################

    // Setup gain dial
    addAndMakeVisible(gainDial);
    gainDial.setLookAndFeel(&prettyLookAndFeel);
    gainDial.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    gainDial.setRange(Parameters::minMakeup, Parameters::maxMakeup, 0.1);
    gainDial.setValue(Parameters::defaultMakeup);
    gainDial.setTextValueSuffix(" dB");

    // Setup labels
    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    gainAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameMakeup, gainDial));

    // ####################
    // #                  #
    // #  EXAMPLE BUTTON  #
    // #                  #
    // ####################

    setSize(600, 400);
}

comprixAudioProcessorEditor::~comprixAudioProcessorEditor() {
    // IMPORTANT: Reset look and feel before destruction
    gainDial.setLookAndFeel(nullptr);
}

void comprixAudioProcessorEditor::paint(juce::Graphics &g) {
    // Dark background to match the dial style
    g.fillAll(juce::Colour(0xff1a1a1a));

    // Optional: Add some text or branding
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(24.0f, juce::Font::bold));
    g.drawFittedText("Comprix", getLocalBounds().removeFromTop(40), juce::Justification::centred,
                     1);
}

void comprixAudioProcessorEditor::resized() {
    auto bounds = getLocalBounds();

    // Reserve space for title
    bounds.removeFromTop(50);

    // Dial section
    auto dialSection = bounds.removeFromTop(180);
    auto dialWidth = 120;
    auto dialSpacing = (dialSection.getWidth() - (3 * dialWidth)) / 4;

    // Position dials horizontally
    auto gainBounds
     = dialSection.removeFromLeft(dialSpacing + dialWidth).removeFromRight(dialWidth);

    // Labels above dials
    gainLabel.setBounds(gainBounds.removeFromTop(25));

    // Dials
    gainDial.setBounds(gainBounds);

    // Visualizers in remaining space
    auto visualizerBounds = bounds.reduced(20);
    // audioProcessor.viewer.setBounds(visualizerBounds);
    // audioProcessor.viewer2.setBounds(visualizerBounds);
}
