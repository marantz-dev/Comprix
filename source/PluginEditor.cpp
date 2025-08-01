#include "PluginParameters.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "UIutils.h"
#include "juce_audio_processors/juce_audio_processors.h"

comprixAudioProcessorEditor::comprixAudioProcessorEditor(comprixAudioProcessor &p, AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p), audioProcessor(p), valueTreeState(vts), sidechainSection(vts, p),
      compressorSection(vts), scopeSection(vts, p), meteringSection(vts, p) {
    addAndMakeVisible(sidechainSection);
    sidechainSection.setLookAndFeel(&theme);

    addAndMakeVisible(compressorSection);
    compressorSection.setLookAndFeel(&theme);

    addAndMakeVisible(scopeSection);
    scopeSection.setLookAndFeel(&theme);

    addAndMakeVisible(meteringSection);

    UIutils::setupToggleButton(bypassButton, "BYPASS");
    bypassButton.setLookAndFeel(&theme);
    addAndMakeVisible(bypassButton);

    bypassButtonAttachment.reset(
     new AudioProcessorValueTreeState::ButtonAttachment(vts, Parameters::nameBypass, bypassButton));

    setSize(1000, 600);

    // TODO:
    // setResizable(true, true);
    // // Set min and max resize limits
    // setResizeLimits(1000, 700, 1600, 1200); // preserves 3:2 ratio boundaries
    // // Maintain aspect ratio: width / height = 3 / 2
    // getConstrainer()->setFixedAspectRatio(3.0 / 2.0);
}

comprixAudioProcessorEditor::~comprixAudioProcessorEditor() {
    this->setLookAndFeel(nullptr);

    bypassButtonAttachment.reset();

    bypassButton.setLookAndFeel(nullptr);
    sidechainSection.setLookAndFeel(nullptr);
    compressorSection.setLookAndFeel(nullptr);
    scopeSection.setLookAndFeel(nullptr);
    meteringSection.setLookAndFeel(nullptr);
}

void comprixAudioProcessorEditor::paint(juce::Graphics &g) {
    auto bounds = getLocalBounds();
    auto height = bounds.getHeight();

    g.fillAll(juce::Colours::black.brighter(0.1f));
    g.setColour(juce::Colours::white);

    auto headerBounds = bounds.removeFromTop(height * 0.06f);
    headerBounds.removeFromLeft(40);
    g.setFont(30.0f);
    g.drawFittedText("COMPRIX", headerBounds, juce::Justification::left, 1);
}

void comprixAudioProcessorEditor::resized() {
    auto bounds = getLocalBounds();

    const int bypasButtonHeight = 30;
    const int bypasButtonWidth = 100;
    auto buttonBounds = bounds;
    buttonBounds.removeFromTop(5);
    buttonBounds.removeFromLeft(bounds.getWidth() * 0.88);
    bypassButton.setBounds(buttonBounds.withSize(bypasButtonWidth, bypasButtonHeight));

    bounds.removeFromTop(bounds.getHeight() * 0.03f);
    bounds.reduce(20, 20);

    auto topSectionBounds = bounds;
    topSectionBounds.removeFromBottom(bounds.getHeight() * 0.4 + 10);

    auto bottomSectionBounds = bounds;
    bottomSectionBounds.removeFromTop(bounds.getHeight() * 0.6 + 10);

    auto sidechainSectionBounds = topSectionBounds;
    sidechainSectionBounds.removeFromRight(topSectionBounds.getWidth() * 0.75f + 7);
    sidechainSection.setBounds(sidechainSectionBounds);
    sidechainSection.resized();

    auto compressorSectionBounds = topSectionBounds;
    compressorSectionBounds.removeFromLeft(topSectionBounds.getWidth() * 0.25f + 7);
    compressorSectionBounds.removeFromRight(topSectionBounds.getWidth() * 0.25f + 7);
    compressorSection.setBounds(compressorSectionBounds);
    compressorSection.resized();

    auto meteringSectionBounds = topSectionBounds;
    meteringSectionBounds.removeFromLeft(topSectionBounds.getWidth() * 0.75f + 7);
    meteringSection.setBounds(meteringSectionBounds);
    meteringSection.resized();

    scopeSection.setBounds(bottomSectionBounds);
    scopeSection.resized();
}
