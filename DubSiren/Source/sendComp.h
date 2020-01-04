/*
  ==============================================================================

    sendComp.h
    Created: 6 Dec 2019 11:37:39am
    Author:  William James

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "knobmanSlider.h"

//==============================================================================
/*
*/
class sendComp    : public Component, public Slider::Listener, public Button::Listener
{
public:
    
    Image sliderImg = ImageCache::getFromMemory(BinaryData::vert_fade_201_png, BinaryData::vert_fade_201_pngSize);
    knobmanSlider volSlider;
    knobmanSlider delaySlider;
    knobmanSlider reverbSlider;
    TextButton output;
    TextButton delay;
    TextButton reverb;
    
    Colour selectedColor = Colours::red;
    
    enum sliderState {
        vol = 0,
        del,
        rev
    };
    sliderState currSlider = vol;
    
    float reverbVal = 0.2;
    float delayVal  = 0.2;
    float outputVal = 0.8;
    
    sendComp() : volSlider(sliderImg, 201), delaySlider(sliderImg, 201), reverbSlider(sliderImg, 201)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        //volume
        addAndMakeVisible(&volSlider);
        volSlider.setSliderStyle(volSlider.RotaryVerticalDrag);
        volSlider.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
        volSlider.setRange(0, 1.0);
        volSlider.addListener(this);
        
        //delay
        addAndMakeVisible(&delaySlider);

        delaySlider.setSliderStyle(volSlider.RotaryVerticalDrag);
        delaySlider.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
        delaySlider.setRange(0, 1);
        delaySlider.addListener(this);
        
        //reverb
        addAndMakeVisible(&reverbSlider);
        reverbSlider.setSliderStyle(volSlider.RotaryVerticalDrag);
        reverbSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        reverbSlider.setRange(0, 1);
        reverbSlider.addListener(this);
        
        
        addAndMakeVisible(&output);

        output.setButtonText("OUT");
        output.setColour(TextButton::buttonColourId, Colour(161, 28, 13));
        output.addListener (this);
        output.colourChanged();
        
        addAndMakeVisible(&delay);
        delay.setColour(TextButton::buttonColourId, Colours::black);
        delay.setButtonText("DEL");
        delay.addListener (this);
        
        addAndMakeVisible(&reverb);
        reverb.setColour(TextButton::buttonColourId, Colours::black);
        reverb.setButtonText("REV");
        reverb.addListener (this);
    }

    ~sendComp()
    {
    }

    void paint (Graphics& g) override
    {
        
        g.setColour(Colour(168, 65, 34));
        int reductSize = 7;
        g.fillRoundedRectangle(reductSize, reductSize, getWidth() - (reductSize * 2), getHeight() - (reductSize * 2), 15.0);
        
        g.setColour(Colours::black);
        reductSize = 12;
        g.drawRoundedRectangle(reductSize, reductSize, getWidth() - (reductSize * 2), getHeight() - (reductSize * 2), 12.0, 3.0);
    
    }

    void resized() override
    {
        
        auto area = getLocalBounds().reduced(20);
        int width = area.getWidth();
        int sliderWidth = width / 2.5;

        //remove all three sliders
        volSlider.setBounds(0,0,0,0);
        delaySlider.setBounds(0,0,0,0);
        reverbSlider.setBounds(0,0,0,0);
        
        switch (currSlider) {
            case vol : volSlider.setBounds(area.removeFromRight(sliderWidth));break;
            case del : delaySlider.setBounds(area.removeFromRight(sliderWidth));break;
            case rev : reverbSlider.setBounds(area.removeFromRight(sliderWidth));break;
        }
        
        //the rest is for buttons
        int buttonHeight = area.getHeight() / 3;
        area = area.removeFromLeft(area.getWidth() - 15);
        output.setBounds(area.removeFromTop(buttonHeight));
        delay.setBounds(area.removeFromTop(buttonHeight));
        reverb.setBounds(area.removeFromTop(buttonHeight));
    }
    
    void sliderValueChanged(Slider* slider) override
    {
        if (slider == &volSlider) {
            outputVal = slider->getValue();
        }
        else if (slider == &reverbSlider) {
            reverbVal = slider->getValue();
        }
        else if (slider == &delaySlider) {
            delayVal = slider->getValue();
        }
    }
    
    void buttonClicked (Button* button) override
    {
        
        //set all blank
        output.setColour(TextButton::buttonColourId, Colours::black);
        reverb.setColour(TextButton::buttonColourId, Colours::black);
        delay.setColour(TextButton::buttonColourId, Colours::black);
        
        Colour MyRed = Colour(161, 28, 13);
        if (button == &output) {
            if (currSlider != vol) {
                currSlider = vol;
                output.setColour(TextButton::buttonColourId, MyRed);
            }
        }
        else if (button == &reverb) {
            if (currSlider == rev) {
                currSlider = vol;
                output.setColour(TextButton::buttonColourId, MyRed);
            }
            else {
                currSlider = rev;
                reverb.setColour(TextButton::buttonColourId, MyRed);
            }
        }
        else if (button == &delay) {
            if (currSlider == del) {
                currSlider = vol;
                output.setColour(TextButton::buttonColourId, MyRed);
            }
            else {
                currSlider = del;
                delay.setColour(TextButton::buttonColourId, MyRed);
            }
        }
        output.colourChanged();
        reverb.colourChanged();
        delay.colourChanged();
        
        resized();
        return;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (sendComp)
};
