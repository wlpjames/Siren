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
#include "knobmanSlider.h"

//==============================================================================
/*
*/
class lfoComp    : public Component, public Slider::Listener, public waveGen
{
public:
    
    Image SpeedImage = ImageCache::getFromMemory(BinaryData::lfo_speed100_png, BinaryData::lfo_speed100_pngSize);
    Image DepthImage = ImageCache::getFromMemory(BinaryData::lfo_depth100_png, BinaryData::lfo_depth100_pngSize);
    Label titleLab;
    
	//properties
	knobmanSlider speed;
	Label speedLab;

	knobmanSlider depth;
	Label depthLab;

    Image WFKnobImage = ImageCache::getFromMemory(BinaryData::lfo_waveform5_png, BinaryData::lfo_waveform5_pngSize);
	knobmanSlider wavtype;
	Label wavLab;

    lfoComp() : waveGen(3, 1.0, 44100), speed(SpeedImage, 100), depth(DepthImage, 100), wavtype(WFKnobImage, 5)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        //title label
        addAndMakeVisible(&titleLab);
        titleLab.setText("LFO", dontSendNotification);
        titleLab.setJustificationType(Justification::centredBottom);
        
        
        //speed
		addAndMakeVisible(speed);
		speed.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		speed.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
		speed.setRange(.1, 6.0);
        speed.setValue(1);
		speed.setTextValueSuffix(" Hz");
		speed.addListener(this);

		addAndMakeVisible(speedLab);
		speedLab.setText("Speed", dontSendNotification);

		//depth
		addAndMakeVisible(depth);
		depth.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		depth.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
		depth.setRange(0.0, 1.0);
        depth.setValue(0.7);
		depth.setTextValueSuffix(" Hz");
		depth.addListener(this);

		addAndMakeVisible(depthLab);
		depthLab.setText("Depth", dontSendNotification);

		//wavetype
		addAndMakeVisible(wavtype);
		wavtype.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		wavtype.setTextBoxStyle(Slider::NoTextBox, 0, 0, 0);
		wavtype.setRange(0, 4, 1);
		wavtype.addListener(this);

		addAndMakeVisible(wavLab);
		wavLab.setText("Wave Type", dontSendNotification);
    }

    ~lfoComp()
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
        // This method is where you should set the bounds of any child
        // components that your component contains..
        int cutLen;
		auto area = getLocalBounds().reduced(20);

		int width = area.getWidth() / 3;
        
        //title allowed overlay w. knobs
        titleLab.setBounds(getLocalBounds().reduced(20).removeFromTop(15));
        
		//area for speed
		auto s_area = area.removeFromLeft(width);
        
        cutLen = (s_area.getWidth() - s_area.getHeight()) / 2;
        
        s_area.removeFromLeft(cutLen);
        s_area.removeFromRight(cutLen);
		speed.setBounds(s_area);

		//area for depth
		auto d_area = area.removeFromLeft(width);
        cutLen = (d_area.getWidth() - d_area.getHeight()) / 2;
        d_area.removeFromLeft(cutLen);
        d_area.removeFromRight(cutLen);
		depth.setBounds(d_area);
		
		//area for type
		auto t_area = area.removeFromLeft(width);
        cutLen = (t_area.getWidth() - t_area.getHeight()) / 2;
        t_area.removeFromLeft(cutLen);
        t_area.removeFromRight(cutLen);
		wavtype.setBounds(t_area);
    }

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &speed) {
            
            //we want this to get progressivly less active
            float freq = sinh(slider->getValue() / 2.5);
            setFreq(freq);
		}
		else if (slider == &depth) {
            setVol(slider->getValue());
		}
		else if (slider == &wavtype) {
			setType(int(wavtype.getValue()));
		}
	}
    
    static const int maxPeriodLen = 10000;
    float lfoSig[maxPeriodLen];
                                                                                                                                                                                                                
    float getVal(int periodLen)
    /* Returns the first sample from a period, suitable in low frequecies. */
    /* In this case will return a value between 1 and 2 to osc between val and octave above. */
    {
        if (periodLen >= maxPeriodLen) {
            return 0;
        }
        nextFrame(lfoSig, periodLen);
        
        //transform from -1 < x < 1 to 1 < x < 3
        return (lfoSig[0] + 1.0 + vol);
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (lfoComp)
};
