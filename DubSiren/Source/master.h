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

	ImageButton playStop;
    
    
    bool is_playing = false;
    Image buttonImages = ImageCache::getFromMemory(BinaryData::switch_press_4_png, BinaryData::switch_press_4_pngSize);
    
    master()
    {

        
		addAndMakeVisible(playStop);
		playStop.setColour(0, Colours::black);
		playStop.setColour(1, Colours::black);
		playStop.setButtonText("Play/Stop");
        
        playStop.setImages(false, true, true,
                           buttonImages.getClippedImage(Rectangle<int>(0, 0, buttonImages.getWidth(), buttonImages.getHeight() / 10)), 1.0, Colour(NULL),
                           buttonImages.getClippedImage(Rectangle<int>(0, 0, buttonImages.getWidth(), buttonImages.getHeight() / 10)), 1.0, Colour(NULL),
                           buttonImages.getClippedImage(Rectangle<int>(0, (buttonImages.getHeight() / 10) * 9, buttonImages.getWidth(), buttonImages.getHeight() / 10)),
                           1.0, Colour(NULL));
        
        playStop.addListener(this);

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
        g.setColour(Colour(168, 65, 34));
        int reductSize = 7;
        g.fillRoundedRectangle(reductSize, reductSize, getWidth() - (reductSize * 2), getHeight() - (reductSize * 2), 15.0);
        
        g.setColour(Colours::black);
        reductSize = 12;
        g.drawRoundedRectangle(reductSize, reductSize, getWidth() - (reductSize * 2), getHeight() - (reductSize * 2), 12.0, 3.0);
    }

    void resized() override
    {
        
		auto area = getLocalBounds();
		playStop.setBounds(area.reduced(15));

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (master)
};
