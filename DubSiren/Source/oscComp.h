/*
  ==============================================================================

    oscComp.h
    Created: 21 Nov 2019 11:36:16am
    Author:  billy

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "waveTable.h"

using namespace std;
//==============================================================================
/*
*/
class oscComp    : public Component,
	public Slider::Listener,
    public waveTable
{
public:

	//properties
	Slider freq;
	Label freqLab;

	Slider wavtype;
	Label wavLab;

    oscComp() : waveTable(440, 0.3, 44100, 100)
    {
        //set up and build tables in waveTables
        
        createTables(); //this might be called before samplerate!!!
        setGlideRate(1);
        
		addAndMakeVisible(freq);
		freq.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		freq.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
		freq.setRange(50, 5000.0); 
		freq.setTextValueSuffix(" Hz");  
		freq.addListener(this);

		addAndMakeVisible(freqLab);
		freqLab.setText("Frequency", dontSendNotification);


		addAndMakeVisible(wavtype);
		wavtype.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		wavtype.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
		wavtype.setRange(0, 3, 1);
		wavtype.addListener(this);

		addAndMakeVisible(wavLab);
		wavLab.setText("Type", dontSendNotification);
    }

    ~oscComp()
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
		
		int width = getWidth() / 2;

		//area for freq
		auto f_area = area.removeFromLeft(width);
		freqLab.setBounds(f_area.removeFromTop(30));
		freq.setBounds(f_area);

		//area for freq
		auto t_area = area.removeFromLeft(width);
		wavLab.setBounds(t_area.removeFromTop(30));
		wavtype.setBounds(t_area);
    }

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &freq) {
            setFreq(slider->getValue());
		}
		else if (slider == &wavtype) {
            setType( int( wavtype.getValue() ));
		}
	}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (oscComp)
};
