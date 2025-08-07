#pragma once
#include "juce_graphics/juce_graphics.h"
#include <JuceHeader.h>

class GainReductionVisualiser : public AudioVisualiserComponent {
  public:
    GainReductionVisualiser(int numChannels) : AudioVisualiserComponent(numChannels) {}

    void paintChannel(Graphics &g, Rectangle<float> area, const Range<float> *levels, int numLevels,
                      int nextSample) override {
        Path p;
        getChannelAsPath(p, levels, numLevels, nextSample);

        auto transform
         = AffineTransform::fromTargetPoints(0.0f, -1.0f, area.getX(), area.getY(), 0.0f, 1.0f, area.getX(),
                                             area.getBottom(), (float)numLevels, -1.0f, area.getRight(), area.getY());

        p.applyTransform(transform);

        float thickness = 1.5f;
        PathStrokeType stroke(thickness);

        Path offsetPath = p;
        offsetPath.applyTransform(AffineTransform::translation(0.0f, thickness * 0.5f));

        g.strokePath(offsetPath, stroke);
    }
};
