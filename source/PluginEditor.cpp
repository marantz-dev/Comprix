#include "PluginParameters.h"
#include "PluginProcessor.h"
#include "UI/Sections/Sidechain.h"
#include "PluginEditor.h"
#pragma clang diagnostic ignored "-Wfloat-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
comprixAudioProcessorEditor::comprixAudioProcessorEditor(comprixAudioProcessor &p,
                                                         AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p), audioProcessor(p), valueTreeState(vts), sidechainSection(vts),
      compressorSection(vts) {
    // meterSection(vts), scopeSection(vts), scopeControlsSection(vts) {
    sidechainGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);

    addAndMakeVisible(sidechainSection);
    addAndMakeVisible(compressorSection);

    setSize(1200, 800);
}

comprixAudioProcessorEditor::~comprixAudioProcessorEditor() {}

void comprixAudioProcessorEditor::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour(0xff1a1a1a));

    g.setColour(juce::Colours::white);
    g.setFont(Font(24.0f, juce::Font::bold));

    auto bounds = getLocalBounds();
    auto height = bounds.getHeight();

    auto headerBounds = bounds.removeFromTop(height * 0.1f);
    headerBounds.removeFromLeft(40);
    g.drawFittedText("Comprix", headerBounds, juce::Justification::left, 1);
}

void comprixAudioProcessorEditor::resized() {
    auto bounds = getLocalBounds();
    bounds.removeFromTop(40);
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
    //
    // auto meterSectionBounds = topSectionBounds;
    // meterSectionBounds.removeFromLeft(topSectionBounds.getWidth() * 0.75f + 7);
    // meterSectionBorder.setBounds(meterSectionBounds);
    //
    // auto scopeSectionBounds = bottomSectionBounds;
    // scopeSectionBounds.removeFromRight(bottomSectionBounds.getWidth() * 0.25f + 7);
    // scopeSectionBorder.setBounds(scopeSectionBounds);
    //
    // auto scopeControlsSectionBounds = bottomSectionBounds;
    // scopeControlsSectionBounds.removeFromLeft(bottomSectionBounds.getWidth() * 0.75f + 7);
    // scopeControlsBorder.setBounds(scopeControlsSectionBounds);
}
