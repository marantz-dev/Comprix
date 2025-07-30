#include "PluginParameters.h"
#include "PluginProcessor.h"
#include "UI/Sections/Sidechain.h"
#include "PluginEditor.h"
#pragma clang diagnostic ignored "-Wfloat-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
comprixAudioProcessorEditor::comprixAudioProcessorEditor(comprixAudioProcessor &p,
                                                         AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p), audioProcessor(p), valueTreeState(vts), sidechainSection(vts) {
    sidechainGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);

    addAndMakeVisible(sidechainSection);
    // addAndMakeVisible(sidechainSectionBorder);
    // sidechainSectionBorder.setText("Sidechain");
    //
    // addAndMakeVisible(compressorSectionBorder);
    // compressorSectionBorder.setText("Compressor");
    //
    // addAndMakeVisible(meterSectionBorder);
    // meterSectionBorder.setText("Metering");
    //
    // addAndMakeVisible(scopeSectionBorder);
    // scopeSectionBorder.setText("Scope");
    //
    // addAndMakeVisible(scopeControlsBorder);
    // scopeControlsBorder.setText("Scope Controls");
    //
    // addAndMakeVisible(sidechainGainSlider);
    //
    // addAndMakeVisible(externalSidechainButton);
    // externalSidechainButton.setButtonText("Sidechain");
    // externalSidechainButton.setClickingTogglesState(true);
    // externalSidechainButton.changeWidthToFitText();
    //
    // addAndMakeVisible(sidechainListenButton);
    // sidechainListenButton.setButtonText("Listen");
    // sidechainListenButton.setClickingTogglesState(true);

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
    // sidechainSectionBorder.setBounds(sidechainSectionBounds);
    // sidechainSectionBounds.reduce(sidechainSectionBounds.getWidth() * 0.1,
    //                               sidechainSectionBounds.getHeight() * 0.1);
    //
    // auto externalSidechainButtonBounds = sidechainSectionBounds;
    // externalSidechainButtonBounds.removeFromBottom(sidechainSectionBounds.getHeight() * 0.80f);
    // externalSidechainButtonBounds.removeFromRight(externalSidechainButtonBounds.getWidth() * 0.25f
    //                                               + 5);
    // // externalSidechainButton.setBounds(externalSidechainButtonBounds);
    //
    // auto sidechainListenButtonBounds = sidechainSectionBounds;
    // sidechainListenButtonBounds.removeFromBottom(sidechainSectionBounds.getHeight() * 0.80f);
    // sidechainListenButtonBounds.removeFromLeft(externalSidechainButtonBounds.getWidth() * 0.75f
    //                                            + 5);
    // sidechainListenButton.setBounds(externalSidechainButtonBounds);
    //
    // auto compressorSectionBounds = topSectionBounds;
    // compressorSectionBounds.removeFromLeft(topSectionBounds.getWidth() * 0.25f + 7);
    // compressorSectionBounds.removeFromRight(topSectionBounds.getWidth() * 0.25f + 7);
    // compressorSectionBorder.setBounds(compressorSectionBounds);
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
