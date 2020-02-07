/*
  ==============================================================================

    head.h
    Created: 3 Feb 2020 10:16:11pm
    Author:  William James

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class head    : public Component
{
    string Title = "Dub Siren 0.0";
    std::unique_ptr <Drawable> logo;
    Label titleLabel;
    
public:
    head()
    {
        logo = Drawable::createFromImageData (BinaryData::logo_svg, BinaryData::logo_svgSize);
        
        titleLabel.setText("ReggaeSiren", dontSendNotification);
        titleLabel.setFont (Font (46.0f, Font::bold));
        addAndMakeVisible(titleLabel);
    }

    ~head()
    {
    }

    void paint (Graphics& g) override
    {
        g.setColour(Colours::black);
        g.fillAll();
        g.setColour(Colour(168, 65, 34));
        auto area = getLocalBounds();
        auto logoarea = area.removeFromLeft(100);
        g.fillEllipse(Rectangle<float>((float)logoarea.getX(), (float) logoarea.getY(), (float)logoarea.getWidth(), (float) logoarea.getHeight()));
        logo->drawWithin(g, Rectangle<float>((float)logoarea.getX(), (float) logoarea.getY(), (float)logoarea.getWidth(), (float) logoarea.getHeight()), RectanglePlacement::Flags::centred, 1.0f);
    }

    void resized() override
    {
        auto area = getLocalBounds();
        area.removeFromLeft(100);
        titleLabel.setBounds(area.removeFromBottom(80).reduced(10));
        
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (head)
};
