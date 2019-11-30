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

//==============================================================================
/*
*/
class delayComp    : public Component, public Slider::Listener, public delay
{
public:
    
    //properties
    Slider decaySlider;
    Label decayLab;

    Slider mixSlider;
    Label mixLab;
    float mixLim = 200;

    Slider lengthSlider;
    Label lengthLab;
    
    delayComp() : delay()
    {
        //decay
        addAndMakeVisible(decaySlider);
        decaySlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        decaySlider.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
        decaySlider.setRange(0, 2.0);
        decaySlider.addListener(this);

        addAndMakeVisible(decayLab);
        decayLab.setText("Decay", dontSendNotification);

        //mix
        addAndMakeVisible(mixSlider);
        mixSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        mixSlider.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
        mixSlider.setRange(0, 1.0);
        mixSlider.addListener(this);

        addAndMakeVisible(mixLab);
        mixLab.setText("Mix", dontSendNotification);

        //length
        addAndMakeVisible(lengthSlider);
        lengthSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        lengthSlider.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
        lengthSlider.setRange(0.1, 1.5);
        lengthSlider.addListener(this);

        addAndMakeVisible(lengthLab);
        lengthLab.setText("Delay Len", dontSendNotification);

    }

    ~delayComp()
    {
    }

    void paint (Graphics& g) override
    {

        g.fillAll (Colours::black);   // clear the background

    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
        auto area = getLocalBounds();
        area.reduce(20, 20);
        
        int compWidth = area.getWidth() / 3;
        
        auto d_area = area.removeFromLeft(compWidth);
        decayLab.setBounds(d_area.removeFromTop(20));
        decaySlider.setBounds(d_area);
        
        auto m_area = area.removeFromLeft(compWidth);
        mixLab.setBounds(m_area.removeFromTop(20));
        mixSlider.setBounds(m_area);
        
        auto l_area = area.removeFromLeft(compWidth);
        lengthLab.setBounds(l_area.removeFromTop(20));
        lengthSlider.setBounds(l_area);

    }
    
    void sliderValueChanged(Slider* slider) override
    {
        if (slider == &decaySlider) {
            setDecay(slider->getValue());
        }
        else if (slider == &mixSlider) {
            setMix(slider->getValue());
        }
        else if (slider == &lengthSlider) {
            setTapeLen(slider->getValue());
        }
        
        return;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (delayComp)
};
