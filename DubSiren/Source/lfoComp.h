/*
  ==============================================================================

    lfoComp.h
    Created: 21 Nov 2019 11:36:48am
    Author:  billy

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "osc.h"

//==============================================================================
/*
*/
class lfoComp    : public Component, public Slider::Listener, public waveGen
{
public:

	//properties
	Slider speed;
	Label speedLab;

	Slider depth;
	Label depthLab;
    float depthLim = 200;

	Slider wavtype;
	Label wavLab;

    lfoComp() : waveGen(3, 1.0, 44100)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

        //speed
		addAndMakeVisible(speed);
		speed.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		speed.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
		speed.setRange(1, 10.0);
		speed.setTextValueSuffix(" Hz");
		speed.addListener(this);

		addAndMakeVisible(speedLab);
		speedLab.setText("Speed", dontSendNotification);

		//depth
		addAndMakeVisible(depth);
		depth.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		depth.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
		depth.setRange(0, 1.0);
		depth.setTextValueSuffix(" Hz");
		depth.addListener(this);

		addAndMakeVisible(depthLab);
		depthLab.setText("Depth", dontSendNotification);

		//wavetype
		addAndMakeVisible(wavtype);
		wavtype.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		wavtype.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
		wavtype.setRange(0, 5, 1);
		wavtype.addListener(this);

		addAndMakeVisible(wavLab);
		wavLab.setText("Wave Type", dontSendNotification);
    }

    ~lfoComp()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (Colours::black);   // clear the background

    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

		auto area = getLocalBounds();

		int width = getWidth() / 3;

		//area for speed
		auto s_area = area.removeFromLeft(width);
		speedLab.setBounds(s_area.removeFromTop(30));
		speed.setBounds(s_area);

		//area for depth
		auto d_area = area.removeFromLeft(width);
		depthLab.setBounds(d_area.removeFromTop(30));
		depth.setBounds(d_area);
		
		//area for type
		auto t_area = area.removeFromLeft(width);
		wavLab.setBounds(t_area.removeFromTop(30));
		wavtype.setBounds(t_area);
    }

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &speed) {
            setFreq(slider->getValue());
		}
		else if (slider == &depth) {
            setVol(slider->getValue() * depthLim);
		}
		else if (slider == &wavtype) {
			setType(int(wavtype.getValue()));
		}
	}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (lfoComp)
};
