#pragma once
#include <JuceHeader.h>

class DBXDial : public juce::LookAndFeel_V4 {
  public:
    void
    drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                     float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override {
        auto bounds = juce::Rectangle<float>(x, y, width, height);
        auto centre = bounds.getCentre();
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 8.0f;

        // Subtle, modernized vintage colors
        auto knobColor = juce::Colour(0xffe8e8e8);     // Very light gray
        auto knobShadow = juce::Colour(0xffc0c0c0);    // Subtle shadow
        auto knobHighlight = juce::Colour(0xfff5f5f5); // Soft highlight
        auto markColor = juce::Colour(0xff666666);     // Medium gray marks
        auto pointerColor = juce::Colour(0xff888888);  // Muted dark gray pointer

        // ###############
        // #             #
        // #  TICKMARKS  #
        // #             #
        // ###############

        auto tickRadius = radius - 3.0f; // Close to edge
        auto numTicks = 11;              // Back to 11 ticks like before
        for(int i = 0; i < numTicks; ++i) {
            auto tickAngle
             = rotaryStartAngle + (float)i / (numTicks - 1) * (rotaryEndAngle - rotaryStartAngle);
            auto tickLength = (i % 2 == 0) ? 6.0f : 3.0f; // Major/minor ticks
            auto tickThickness = (i % 2 == 0) ? 1.2f : 0.8f;

            auto innerX = centre.x
                          + std::cos(tickAngle - juce::MathConstants<float>::halfPi)
                             * (tickRadius - tickLength);
            auto innerY = centre.y
                          + std::sin(tickAngle - juce::MathConstants<float>::halfPi)
                             * (tickRadius - tickLength);
            auto outerX
             = centre.x + std::cos(tickAngle - juce::MathConstants<float>::halfPi) * tickRadius;
            auto outerY
             = centre.y + std::sin(tickAngle - juce::MathConstants<float>::halfPi) * tickRadius;

            g.setColour(markColor.withAlpha(0.4f));
            g.drawLine(innerX, innerY, outerX, outerY, tickThickness);
        }

        // Main knob with very subtle gradient
        auto knobRadius = radius - 15.0f;

        // Subtle gradient for depth
        juce::ColourGradient knobGradient(
         knobHighlight, centre.x - knobRadius * 0.4f, centre.y - knobRadius * 0.4f,
         knobColor.darker(0.1f), centre.x + knobRadius * 0.6f, centre.y + knobRadius * 0.6f, true);
        g.setGradientFill(knobGradient);
        g.fillEllipse(centre.x - knobRadius, centre.y - knobRadius, knobRadius * 2.0f,
                      knobRadius * 2.0f);

        // Very subtle shadow edge
        g.setColour(knobShadow.withAlpha(0.3f));
        g.drawEllipse(centre.x - knobRadius, centre.y - knobRadius, knobRadius * 2.0f,
                      knobRadius * 2.0f, 1.0f);

        // Needle pointer going to the edge, thicker
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto pointerLength = knobRadius * 0.95f; // Almost to the edge of knob
        auto pointerThickness = 2.5f;            // Thicker needle

        auto pointerX
         = centre.x + std::cos(angle - juce::MathConstants<float>::halfPi) * pointerLength;
        auto pointerY
         = centre.y + std::sin(angle - juce::MathConstants<float>::halfPi) * pointerLength;

        g.setColour(pointerColor);
        g.drawLine(centre.x, centre.y, pointerX, pointerY, pointerThickness);

        // Small center dot
        auto centerDotRadius = 1.5f;
        g.setColour(pointerColor.withAlpha(0.6f));
        g.fillEllipse(centre.x - centerDotRadius, centre.y - centerDotRadius,
                      centerDotRadius * 2.0f, centerDotRadius * 2.0f);
    }

    juce::MouseCursor getMouseCursorFor(juce::Component &component) override {
        if(auto *slider = dynamic_cast<juce::Slider *>(&component)) {
            if(slider->isMouseButtonDown())
                return juce::MouseCursor::NoCursor;
            else
                return juce::MouseCursor::PointingHandCursor;
        }
        return juce::LookAndFeel_V4::getMouseCursorFor(component);
    }
};
