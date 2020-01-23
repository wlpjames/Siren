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
#include "knobmanSlider.h"

using namespace std;
//==============================================================================
/*
*/
class oscComp    : public Component,
	public Slider::Listener,
    public waveTable
{
public:

    Label titleLab;
    
	//properties
    //https://www.g200kg.com/en/webknobman/index.html?f=KNB_DR8_xL.knob&n=1531
    Image freqKnobImage = ImageCache::getFromMemory(BinaryData::knob2200_png, BinaryData::knob2200_pngSize);
    
	knobmanSlider freq;
	Label freqLab;
    //https://www.g200kg.com/en/webknobman/index.html?f=KNB_DR8_xL.knob&n=1531
    Image wavtypeKnobImage = ImageCache::getFromMemory(BinaryData::knob24_png, BinaryData::knob24_pngSize);
	knobmanSlider wavtype;
	Label wavLab;

    oscComp() : waveTable(440, 0.3, 44100, 10), freq(freqKnobImage, 200), wavtype(wavtypeKnobImage, 4)
    {
        //set up and build tables in waveTables
        
        createTables(); //this might be called before samplerate!!!
        setGlideRate(1);
        
        addAndMakeVisible(&titleLab);
        titleLab.setText("OSC", dontSendNotification);
        titleLab.setJustificationType(Justification::centredBottom);
        
		addAndMakeVisible(freq);
		freq.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		freq.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
		freq.setRange(50, 1200.0, 1); 
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

        g.setColour(Colour(168, 65, 34));
        int reductSize = 7;
        g.fillRoundedRectangle(reductSize, reductSize, getWidth() - (reductSize * 2), getHeight() - (reductSize * 2), 15.0);
        
        g.setColour(Colours::black);
        reductSize = 12;
        g.drawRoundedRectangle(reductSize, reductSize, getWidth() - (reductSize * 2), getHeight() - (reductSize * 2), 12.0, 3.0);
        

    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

		auto area = getLocalBounds();
		area.reduce(20, 20);
        
        auto titleArea = area.removeFromTop(20);
        titleLab.setBounds(titleArea);
        
		int width = area.getWidth() / 2;

		//area for freq
		auto f_area = area.removeFromLeft(width);
		//freqLab.setBounds(f_area.removeFromTop(30));
        int cutLen = (f_area.getWidth() - f_area.getHeight()) / 2;
        f_area.removeFromLeft(cutLen);
        f_area.removeFromRight(cutLen);
		freq.setBounds(f_area);

		//area for type
		auto t_area = area.removeFromLeft(width);
		//wavLab.setBounds(t_area.removeFromTop(30));
        t_area.removeFromLeft(cutLen);
        t_area.removeFromRight(cutLen);
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
