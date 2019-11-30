/*
  ==============================================================================

    master.h
    Created: 21 Nov 2019 12:39:45pm
    Author:  billy

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class master    : public Component,
                  public Button::Listener
{
public:

	TextButton playStop;
	TextButton bleep;
    
    bool is_playing = false;

    master()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

		addAndMakeVisible(playStop);
		playStop.setColour(0, Colours::black);
		playStop.setColour(1, Colours::black);
		playStop.setButtonText("Play/Stop");
        playStop.addListener (this);

		addAndMakeVisible(bleep);
		bleep.setColour(0, Colours::red);
		bleep.setButtonText("Bleep");

    }

    ~master()
    {
        
    }
    
    bool get_is_playing()
    {
        return is_playing;
    }
    
    void buttonClicked (Button* button) override
    {
        if (button == &playStop) {
            is_playing = !is_playing;
            return;
        }
        return;
    }
    
    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (Colours::black);   // clear the background

        g.setColour (Colours::darkblue);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

		auto area = getLocalBounds();
		int width = getWidth();
		playStop.setBounds(area.removeFromRight(width / 3));
		bleep.setBounds(area.removeFromLeft(width / 3));

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (master)
};
