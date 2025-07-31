#pragma once
#include "juce_graphics/juce_graphics.h"
#include <JuceHeader.h>

class Theme : public juce::LookAndFeel_V4 {
  public:
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override {
        auto bounds = juce::Rectangle<float>(x, y, width, height);
        auto centre = bounds.getCentre();
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 8.0f;

        // Subtle, modernized vintage colors
        auto knobColor = juce::Colour(0xffe8e8e8);       // Very light gray
        auto knobShadow = juce::Colour(0xffc0c0c0);      // Subtle shadow
        auto knobHighlight = juce::Colour(0xfff5f5f5);   // Soft highlight
        auto markColor = juce::Colours::grey.brighter(); // Medium gray marks
        auto pointerColor = juce::Colour(0xff888888);    // Muted dark gray pointer

        // ###############
        // #             #
        // #  TICKMARKS  #
        // #             #
        // ###############

        auto tickRadius = radius - 3.0f; // Close to edge
        auto numTicks = 11;              // Back to 11 ticks like before
        for(int i = 0; i < numTicks; ++i) {
            auto tickAngle = rotaryStartAngle + (float)i / (numTicks - 1) * (rotaryEndAngle - rotaryStartAngle);
            auto tickLength = (i % 2 == 0) ? 6.0f : 3.0f; // Major/minor ticks
            auto tickThickness = (i % 2 == 0) ? 1.2f : 0.8f;

            auto innerX
             = centre.x + std::cos(tickAngle - juce::MathConstants<float>::halfPi) * (tickRadius - tickLength);
            auto innerY
             = centre.y + std::sin(tickAngle - juce::MathConstants<float>::halfPi) * (tickRadius - tickLength);
            auto outerX = centre.x + std::cos(tickAngle - juce::MathConstants<float>::halfPi) * tickRadius;
            auto outerY = centre.y + std::sin(tickAngle - juce::MathConstants<float>::halfPi) * tickRadius;

            g.setColour(markColor.withAlpha(0.4f));
            g.drawLine(innerX, innerY, outerX, outerY, tickThickness);
        }

        // Main knob with very subtle gradient
        auto knobRadius = radius - 15.0f;

        // Subtle gradient for depth
        juce::ColourGradient knobGradient(knobHighlight, centre.x - knobRadius * 0.4f, centre.y - knobRadius * 0.4f,
                                          knobColor.darker(0.1f), centre.x + knobRadius * 0.6f,
                                          centre.y + knobRadius * 0.6f, true);
        g.setGradientFill(knobGradient);
        g.fillEllipse(centre.x - knobRadius, centre.y - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);

        // Very subtle shadow edge
        g.setColour(knobShadow.withAlpha(0.3f));
        g.drawEllipse(centre.x - knobRadius, centre.y - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f, 1.0f);

        // Needle pointer going to the edge, thicker
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto pointerLength = knobRadius * 0.95f; // Almost to the edge of knob
        auto pointerThickness = 2.5f;            // Thicker needle

        auto pointerX = centre.x + std::cos(angle - juce::MathConstants<float>::halfPi) * pointerLength;
        auto pointerY = centre.y + std::sin(angle - juce::MathConstants<float>::halfPi) * pointerLength;

        g.setColour(pointerColor);
        g.drawLine(centre.x, centre.y, pointerX, pointerY, pointerThickness);

        // Small center dot
        auto centerDotRadius = 1.5f;
        g.setColour(pointerColor.withAlpha(0.6f));
        g.fillEllipse(centre.x - centerDotRadius, centre.y - centerDotRadius, centerDotRadius * 2.0f,
                      centerDotRadius * 2.0f);
    }

    // void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float
    // minSliderPos,
    //                       float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider &slider) override {
    //     auto bounds = juce::Rectangle<float>(x, y, width, height);
    //     auto isHorizontal = style == juce::Slider::LinearHorizontal;
    //
    //     // Vintage console color palette - greyscale
    //     auto panelColor = juce::Colour(0xff2a2a2a);    // Dark grey panel
    //     auto trackColor = juce::Colour(0xff1a1a1a);    // Dark worn track
    //     auto trackRimColor = juce::Colour(0xff666666); // Medium grey rim
    //     auto fillColor = juce::Colour(0xff505050);     // Medium grey fill
    //     auto thumbColor = juce::Colour(0xff888888);    // Light grey thumb
    //     auto thumbRimColor = juce::Colour(0xff555555); // Darker grey rim
    //     auto wearColor = juce::Colour(0xff181818);     // Wear marks
    //
    //     // Calculate dimensions for vintage proportions
    //     auto trackThickness = isHorizontal ? height * 0.4f : width * 0.4f;
    //     auto thumbSize = isHorizontal ? trackThickness * 0.9f : trackThickness * 0.9f;
    //
    //     // Create track bounds
    //     juce::Rectangle<float> trackBounds;
    //     if(isHorizontal) {
    //         trackBounds = bounds.withSizeKeepingCentre(width - thumbSize, trackThickness);
    //     } else {
    //         trackBounds = bounds.withSizeKeepingCentre(trackThickness, height - thumbSize);
    //     }
    //
    //     // Draw panel background with wood grain effect
    //     g.setColour(panelColor);
    //     g.fillRoundedRectangle(bounds.reduced(2), 3.0f);
    //
    //     // Add subtle panel texture
    //     for(int i = 0; i < 5; i++) {
    //         g.setColour(panelColor.darker(0.1f + i * 0.02f));
    //         float grainY = bounds.getY() + (i * bounds.getHeight() / 4.0f);
    //         g.drawLine(bounds.getX(), grainY, bounds.getRight(), grainY, 0.5f);
    //     }
    //
    //     auto cornerRadius = trackThickness * 0.15f; // Less rounded for vintage look
    //
    //     // Draw track bezel (raised rim effect)
    //     g.setColour(trackRimColor.brighter(0.3f));
    //     auto outerTrack = trackBounds.expanded(3.0f);
    //     g.fillRoundedRectangle(outerTrack, cornerRadius + 2.0f);
    //
    //     // Track shadow (deep inset)
    //     g.setColour(juce::Colour(0xff000000).withAlpha(0.6f));
    //     auto shadowTrack = trackBounds.translated(1, 2);
    //     g.fillRoundedRectangle(shadowTrack, cornerRadius);
    //
    //     // Main track (recessed)
    //     g.setColour(trackColor);
    //     g.fillRoundedRectangle(trackBounds, cornerRadius);
    //
    //     // Track wear pattern
    //     g.setColour(wearColor);
    //     auto wearTrack = trackBounds.reduced(1.5f);
    //     g.fillRoundedRectangle(wearTrack, cornerRadius - 1.0f);
    //
    //     // Add scratches and wear marks on track
    //     g.setColour(trackColor.brighter(0.1f));
    //     for(int i = 0; i < 3; i++) {
    //         float scratchX = trackBounds.getX() + (i * trackBounds.getWidth() / 2.5f);
    //         g.drawLine(scratchX, trackBounds.getY() + 2, scratchX + trackBounds.getWidth() * 0.1f,
    //                    trackBounds.getBottom() - 2, 0.3f);
    //     }
    //
    //     // Calculate fill area - using constrained thumb position
    //     juce::Rectangle<float> fillBounds;
    //     if(isHorizontal) {
    //         auto fillWidth = thumbCentre.x - trackBounds.getX();
    //         fillBounds = trackBounds.withWidth(fillWidth);
    //     } else {
    //         auto fillHeight = trackBounds.getBottom() - thumbCentre.y;
    //         fillBounds = juce::Rectangle<float>(trackBounds.getX(), thumbCentre.y, trackBounds.getWidth(),
    //         fillHeight);
    //     }
    //
    //     // Draw vintage-style fill without outer glow
    //     if(!fillBounds.isEmpty()) {
    //         // Main fill with greyscale gradient
    //         juce::ColourGradient fillGradient;
    //         if(isHorizontal) {
    //             fillGradient = juce::ColourGradient(fillColor.brighter(0.3f), fillBounds.getX(),
    //                                                 fillBounds.getY() + fillBounds.getHeight() * 0.2f,
    //                                                 fillColor.darker(0.2f), fillBounds.getX(),
    //                                                 fillBounds.getBottom() - fillBounds.getHeight() * 0.2f, false);
    //         } else {
    //             fillGradient
    //              = juce::ColourGradient(fillColor.brighter(0.3f), fillBounds.getX() + fillBounds.getWidth() * 0.2f,
    //                                     fillBounds.getY(), fillColor.darker(0.2f),
    //                                     fillBounds.getRight() - fillBounds.getWidth() * 0.2f, fillBounds.getY(),
    //                                     false);
    //         }
    //
    //         g.setGradientFill(fillGradient);
    //         g.fillRoundedRectangle(fillBounds, cornerRadius);
    //
    //         // Subtle highlight stripe
    //         g.setColour(fillColor.brighter(0.4f).withAlpha(0.6f));
    //         auto highlightBounds = fillBounds.reduced(1.0f).withHeight(fillBounds.getHeight() * 0.25f);
    //         g.fillRoundedRectangle(highlightBounds, cornerRadius - 0.5f);
    //     }
    //
    //     // Calculate thumb position - constrained to track bounds
    //     juce::Point<float> thumbCentre;
    //     auto thumbRadius = thumbSize * 0.5f;
    //
    //     if(isHorizontal) {
    //         // Constrain horizontal movement to keep thumb within track
    //         float minX = trackBounds.getX() + thumbRadius;
    //         float maxX = trackBounds.getRight() - thumbRadius;
    //         float constrainedX = juce::jlimit(minX, maxX, sliderPos);
    //         thumbCentre = juce::Point<float>(constrainedX, bounds.getCentreY());
    //     } else {
    //         // Constrain vertical movement to keep thumb within track
    //         float minY = trackBounds.getY() + thumbRadius;
    //         float maxY = trackBounds.getBottom() - thumbRadius;
    //         float constrainedY = juce::jlimit(minY, maxY, sliderPos);
    //         thumbCentre = juce::Point<float>(bounds.getCentreX(), constrainedY);
    //     }
    //
    //     // Draw vintage knob/thumb - slightly bigger
    //
    //     // Knob bezel (outer ring) - smaller and contained
    //     g.setColour(thumbRimColor.darker(0.4f));
    //     g.fillEllipse(thumbCentre.x - thumbRadius - 1, thumbCentre.y - thumbRadius - 1, (thumbRadius + 1) * 2,
    //                   (thumbRadius + 1) * 2);
    //
    //     // Knob shadow - smaller
    //     g.setColour(juce::Colour(0xff000000).withAlpha(0.5f));
    //     g.fillEllipse(thumbCentre.x - thumbRadius + 1, thumbCentre.y - thumbRadius + 1.5f, thumbRadius * 2,
    //                   thumbRadius * 2);
    //
    //     // Main knob body with vintage brass gradient
    //     juce::ColourGradient knobGradient(thumbColor.brighter(0.3f), thumbCentre.x - thumbRadius * 0.4f,
    //                                       thumbCentre.y - thumbRadius * 0.4f, thumbColor.darker(0.4f),
    //                                       thumbCentre.x + thumbRadius * 0.6f, thumbCentre.y + thumbRadius * 0.6f,
    //                                       true);
    //     g.setGradientFill(knobGradient);
    //     g.fillEllipse(thumbCentre.x - thumbRadius, thumbCentre.y - thumbRadius, thumbRadius * 2, thumbRadius * 2);
    //
    //     // Knob rim highlight
    //     g.setColour(thumbRimColor.brighter(0.5f));
    //     g.drawEllipse(thumbCentre.x - thumbRadius, thumbCentre.y - thumbRadius, thumbRadius * 2, thumbRadius *
    //     2, 1.5f);
    //
    //     // Inner knob detail ring
    //     g.setColour(thumbRimColor.darker(0.2f));
    //     auto innerRingRadius = thumbRadius * 0.75f;
    //     g.drawEllipse(thumbCentre.x - innerRingRadius, thumbCentre.y - innerRingRadius, innerRingRadius * 2,
    //                   innerRingRadius * 2, 0.8f);
    //
    //     // Vintage knob center dot/indicator
    //     g.setColour(thumbRimColor.darker(0.6f));
    //     auto dotRadius = thumbRadius * 0.2f;
    //     g.fillEllipse(thumbCentre.x - dotRadius, thumbCentre.y - dotRadius, dotRadius * 2, dotRadius * 2);
    //
    //     // Knob wear marks and patina - smaller
    //     g.setColour(thumbColor.darker(0.3f).withAlpha(0.4f));
    //     for(int i = 0; i < 4; i++) {
    //         float angle = i * juce::MathConstants<float>::halfPi;
    //         float markX = thumbCentre.x + std::cos(angle) * thumbRadius * 0.7f;
    //         float markY = thumbCentre.y + std::sin(angle) * thumbRadius * 0.7f;
    //         g.fillEllipse(markX - 0.8f, markY - 0.8f, 1.6f, 1.6f);
    //     }
    //
    //     // Final highlight on top of knob - smaller
    //     g.setColour(thumbColor.brighter(0.8f).withAlpha(0.6f));
    //     auto highlightRadius = thumbRadius * 0.25f;
    //     g.fillEllipse(thumbCentre.x - highlightRadius - thumbRadius * 0.15f,
    //                   thumbCentre.y - highlightRadius - thumbRadius * 0.15f, highlightRadius * 2, highlightRadius *
    //                   2);
    // }
    void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override {
        auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
        auto isToggleOn = button.getToggleState();
        auto isButtonDown = shouldDrawButtonAsDown;
        auto isHovered = shouldDrawButtonAsHighlighted;

        // Modern flat colors similar to your image
        auto backgroundColor = juce::Colour(0xff3a3a3a); // Background gray
        auto onColor = juce::Colour(0xffff9500);         // Orange when active (like "Peak")
        auto offColor = juce::Colour(0xff464646);        // Light gray when off
        auto hoverColor = juce::Colour(0xff525252);      // Slightly lighter on hover
        auto textOffColor = juce::Colour(0xffcccccc);    // Light gray text when off
        auto textOnColor = juce::Colour(0xff000000);     // Black text when on (for contrast with orange)

        // Color selection based on state
        juce::Colour buttonColor;
        if(isToggleOn) {
            buttonColor = isButtonDown ? onColor.darker(0.15f) : onColor;
        } else {
            if(isButtonDown) {
                buttonColor = offColor.darker(0.1f);
            } else if(isHovered) {
                buttonColor = hoverColor;
            } else {
                buttonColor = offColor;
            }
        }

        auto cornerRadius = 2.0f; // Very subtle corners like in your image

        // Main button - clean and flat
        g.setColour(buttonColor);
        g.fillRoundedRectangle(bounds, cornerRadius);

        // Very subtle border for definition
        g.setColour(backgroundColor.darker(0.2f).withAlpha(0.3f));
        g.drawRoundedRectangle(bounds, cornerRadius, 0.5f);

        // Draw button text
        if(button.getButtonText().isNotEmpty()) {
            g.setColour(isToggleOn ? textOnColor : textOffColor);
            g.setFont(juce::FontOptions(bounds.getHeight() * 0.5f, juce::Font::plain));
            g.drawFittedText(button.getButtonText(), bounds.toNearestInt(), juce::Justification::centred, 1);
        }
    }

    void drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
        auto isButtonDown = shouldDrawButtonAsDown;
        auto isHovered = shouldDrawButtonAsHighlighted;

        // Clean modern colors matching your image
        auto buttonColor = juce::Colour(0xff464646);  // Base gray color
        auto hoverColor = juce::Colour(0xff525252);   // Slightly lighter on hover
        auto pressedColor = juce::Colour(0xff3a3a3a); // Darker when pressed

        // Select color based on state
        juce::Colour currentColor;
        if(isButtonDown) {
            currentColor = pressedColor;
        } else if(isHovered) {
            currentColor = hoverColor;
        } else {
            currentColor = buttonColor;
        }

        auto cornerRadius = 2.0f; // Minimal corners like your image

        // Main button - clean and flat
        g.setColour(currentColor);
        g.fillRoundedRectangle(bounds, cornerRadius);

        // Very subtle border for definition
        g.setColour(juce::Colour(0xff2a2a2a).withAlpha(0.3f));
        g.drawRoundedRectangle(bounds, cornerRadius, 0.5f);
    }

    void drawButtonText(juce::Graphics &g, juce::TextButton &button, bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override {
        auto font = getTextButtonFont(button, button.getHeight());
        g.setFont(font);

        // Ableton Live text styling - clean and minimal
        auto textColor = juce::Colour(0xffcccccc); // Light gray, similar to Ableton's text

        // Subtle state changes like Ableton
        if(shouldDrawButtonAsDown) {
            textColor = textColor.darker(0.2f);
        } else if(shouldDrawButtonAsHighlighted) {
            textColor = textColor.brighter(0.1f);
        }

        auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
        auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;
        auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
        auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
        auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        auto textWidth = button.getWidth() - leftIndent - rightIndent;

        if(textWidth > 0) {
            // Clean text without shadow - Ableton style
            g.setColour(textColor);
            g.drawFittedText(button.getButtonText(), leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                             juce::Justification::centred, 2);
        }
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
