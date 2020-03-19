/*
  ==============================================================================

    delayComp.h
    Created: 21 Nov 2019 11:36:29am
    Author:  billy

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "delay.h"
#include "convolva.h"
#include "knobmanSlider.h"

//==============================================================================
/*
*/
class delayComp    : public Component, public Slider::Listener, public tapeDelay
{
public:
    
    //properties
    Label titleLab;
    
    Image lengthImage = ImageCache::getFromMemory(BinaryData::delay_length100_png, BinaryData::delay_length100_pngSize);
    Image decayImage = ImageCache::getFromMemory(BinaryData::delay_decay100_png, BinaryData::delay_decay100_pngSize);
    
    knobmanSlider decaySlider;
    Label decayLab;

    knobmanSlider lengthSlider;
    Label lengthLab;
    
    
    delayComp() : tapeDelay(), decaySlider(decayImage, 100), lengthSlider(lengthImage, 100)
    {
        
        //title
        titleLab.setText("TAPE DELAY", dontSendNotification);
        titleLab.setJustificationType(Justification::centredBottom);
        titleLab.setFont(Font("sans serif", 14.0, Font::plain));
        addAndMakeVisible(&titleLab);
        
        
        //decay
        addAndMakeVisible(decaySlider);
        decaySlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        decaySlider.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
        decaySlider.setRange(0, 1.5);
        decaySlider.addListener(this);
        decaySlider.setValue(0.3);

        addAndMakeVisible(decayLab);
        decayLab.setText("Decay", dontSendNotification);


        //length
        addAndMakeVisible(lengthSlider);
        lengthSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        lengthSlider.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
        lengthSlider.setRange(0.1, 1.5);
        lengthSlider.setValue(0.6);
        lengthSlider.addListener(this);

        addAndMakeVisible(lengthLab);
        lengthLab.setText("Delay Len", dontSendNotification);

    }

    ~delayComp()
    {
    }

    void paint (Graphics& g) override
    {
        g.setColour(Colour(168, 65, 34));
        int reductSize = 7;
        g.fillRoundedRectangle(reductSize, reductSize, getWidth() - (reductSize * 2), getHeight() - (reductSize * 2), 15.0);
        
        g.setColour(Colours::black);
        reductSize = 11;
        g.drawRoundedRectangle(reductSize, reductSize, getWidth() - (reductSize * 2), getHeight() - (reductSize * 2), 12.0, 2.0);
    }

    void resized() override
    {

        int cutLen;
        auto area = getLocalBounds().reduced(10);

        //commented this to see what would happen
        area.reduce(7, 7);
        
        int compWidth = area.getWidth() / 2;
        
        //title allowed overlay w. knobs
        titleLab.setBounds(getLocalBounds().reduced(15).removeFromTop(20));
        
        auto d_area = area.removeFromLeft(compWidth);
        cutLen = (d_area.getWidth() - d_area.getHeight()) / 2;
        d_area.removeFromLeft(cutLen);
        d_area.removeFromRight(cutLen);
        decaySlider.setBounds(d_area);
        
        auto l_area = area.removeFromLeft(compWidth);
        cutLen = (l_area.getWidth() - l_area.getHeight()) / 2;
        l_area.removeFromLeft(cutLen);
        l_area.removeFromRight(cutLen);
        lengthSlider.setBounds(l_area);

    }
    
    void sliderValueChanged(Slider* slider) override
    {
        if (slider == &decaySlider) {
            setDecay(slider->getValue());
        }
        else if (slider == &lengthSlider) {
            setDelayTime(slider->getValue());
        }
        
        return;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (delayComp)
};
