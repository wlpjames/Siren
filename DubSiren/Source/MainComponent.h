/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "oscComp.h"
#include "lfoComp.h"
#include "delayComp.h"
#include "master.h"
#include "convolva.h"
#include "sendComp.h"
#include "env.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
	delayComp delay;
	oscComp osc;
	lfoComp lfo;
	master Master;
    convolva reverb;
    convolva amp;
    sendComp sends;
    env envelope = env(0.05, 0.1);
    softClippa clipa;
    
    //background
    Image backgroundImage;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
