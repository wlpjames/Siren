/*
  ==============================================================================

    knobmanSlider.h
    Created: 12 Dec 2019 10:10:02am
    Author:  William James

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class knobmanSlider    : public juce::Slider
{
public:
    knobmanSlider(juce::Image stripToShow, int NumFrames)
        : filmStrip(stripToShow)
    {
        // careful with empty images (zero division) and/or non-square images.
        // also, strips with one image wont work as well.
        // strips are vertical stacked images of the same size.
        numFrames = NumFrames;
        sideLength = filmStrip.getWidth();
        hieghtLen = filmStrip.getHeight() / numFrames;
    }

    ~knobmanSlider()
    {
    }
    
    void paint(juce::Graphics & g) override
    {
        std::size_t imageNumber = static_cast<std::size_t>(0.5 + (getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (numFrames - 1));
        g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(), 0, imageNumber * hieghtLen, sideLength, hieghtLen);
    }
    
    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    private:
    std::size_t numFrames, sideLength, hieghtLen;
    juce::Image filmStrip;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (knobmanSlider)
};
