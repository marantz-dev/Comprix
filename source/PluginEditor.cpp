#include "PluginParameters.h"
#include "PluginProcessor.h"
#include "Sidechain.h"
#include "PluginEditor.h"
#pragma clang diagnostic ignored "-Wfloat-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
comprixAudioProcessorEditor::comprixAudioProcessorEditor(comprixAudioProcessor &p, AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p), audioProcessor(p), valueTreeState(vts), sidechainSection(vts), compressorSection(vts),
      scopeSection(vts, p), meteringSection(vts, p) {
    addAndMakeVisible(sidechainSection);
    sidechainSection.setLookAndFeel(&theme);

    addAndMakeVisible(compressorSection);
    compressorSection.setLookAndFeel(&theme);

    addAndMakeVisible(scopeSection);
    scopeSection.setLookAndFeel(&theme);

    addAndMakeVisible(meteringSection);

    setSize(1000, 600);
    // setResizable(true, true);
    //
    // // Set min and max resize limits
    // setResizeLimits(1000, 700, 1600, 1200); // preserves 3:2 ratio boundaries
    //
    // // Maintain aspect ratio: width / height = 3 / 2
    // getConstrainer()->setFixedAspectRatio(3.0 / 2.0);
}

comprixAudioProcessorEditor::~comprixAudioProcessorEditor() {
    this->setLookAndFeel(nullptr);

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
    g.setFont(30.0f);
    headerBounds.removeFromLeft(40);
    g.drawFittedText("Comprix", headerBounds, juce::Justification::left, 1);
}

void comprixAudioProcessorEditor::resized() {
    auto bounds = getLocalBounds();
    bounds.removeFromTop(bounds.getHeight() * 0.03f); // Reserve space for header
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
