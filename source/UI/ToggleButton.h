#pragma once
#include <JuceHeader.h>

class ButtonToggle : public juce::LookAndFeel_V4 {
  public:
    void
    drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                     bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        auto bounds = button.getLocalBounds().toFloat();
        auto isToggleOn = button.getToggleState();
        auto isButtonDown = shouldDrawButtonAsDown;
        auto isHovered = shouldDrawButtonAsHighlighted;

        // Ableton-style colors
        auto offColor = juce::Colour(0xff404040);     // Dark gray when off
        auto onColor = juce::Colour(0xff0084ff);      // Ableton blue when on
        auto borderColor = juce::Colour(0xff606060);  // Subtle border
        auto textOffColor = juce::Colour(0xffb0b0b0); // Light gray text when off
        auto textOnColor = juce::Colour(0xffffffff);  // White text when on

        // Adjust colors for interaction states
        if(isButtonDown) {
            onColor = onColor.darker(0.3f);
            offColor = offColor.darker(0.2f);
        } else if(isHovered) {
            onColor = onColor.brighter(0.1f);
            offColor = offColor.brighter(0.1f);
        }

        // Draw button background
        auto cornerRadius = 20.0f;
        g.setColour(isToggleOn ? onColor : offColor);
        g.fillRoundedRectangle(bounds, cornerRadius);

        // Draw subtle border
        g.setColour(borderColor.withAlpha(0.3f));
        g.drawRoundedRectangle(bounds, cornerRadius, 1.0f);

        // Add subtle inner highlight when on
        if(isToggleOn) {
            auto highlightBounds = bounds.reduced(1.0f);
            g.setColour(juce::Colours::white.withAlpha(0.1f));
            g.fillRoundedRectangle(highlightBounds, cornerRadius - 1.0f);
        }

        // Draw button text
        if(button.getButtonText().isNotEmpty()) {
            g.setColour(isToggleOn ? textOnColor : textOffColor);
            g.setFont(juce::FontOptions(12.0f, juce::Font::plain));
            g.drawFittedText(button.getButtonText(), bounds.toNearestInt(),
                             juce::Justification::centred, 1);
        }
    }
};
