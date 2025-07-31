#pragma once

#include <JuceHeader.h>

namespace UIutils {

    inline void setupSlider(juce::Slider &slider, const juce::Slider::SliderStyle &style, float min, float max,
                            float defaultValue, float stepSize, const juce::String &suffix, float skewFactor,
                            juce::Label &label, const juce::String &labelText) {
        slider.setSliderStyle(style);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        slider.setRange(min, max, stepSize);
        slider.setValue(defaultValue);
        slider.setDoubleClickReturnValue(true, defaultValue);
        slider.setTextValueSuffix(suffix);
        slider.setSkewFactor(skewFactor);

        label.setText(labelText, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.attachToComponent(&slider, false);
    }

    inline void attachLabel(juce::Label &label, juce::Slider *slider, int distance = 5) {
        label.attachToComponent(slider, false);
        label.setBounds(label.getX(), label.getY() + distance, label.getWidth(), label.getHeight());
    }

    inline void setupToggleButton(juce::ToggleButton &button, const juce::String &text) {
        button.setButtonText(text);
        button.setClickingTogglesState(true);
        button.setToggleState(false, juce::dontSendNotification);
    }
    //     addAndMakeVisible(peakDetectorButton);
    // peakDetectorButton.setButtonText("Peak");
    // peakDetectorButton.setClickingTogglesState(true);
    // peakDetectorButton.setToggleState(true, juce::dontSendNotification);
    // peakDetectorButton.setTooltip("Use Peak detection for compression");
    // peakDetectorButton.setClickingTogglesState(true);

}
