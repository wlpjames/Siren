/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/
 #include "MainComponent.h"
#include "oscComp.h"
#include "lfoComp.h"
#include "delayComp.h"
using namespace std;

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (300, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (1, 1);
    }

	//volume
	addAndMakeVisible(&vol);
	vol.setRange(0, 1);          // [1]
	vol.setTextValueSuffix(" Db");     // [2]
	vol.setSliderStyle(vol.LinearBarVertical);
	//vol.addListener(this);

	addAndMakeVisible(osc);
	addAndMakeVisible(lfo);
	addAndMakeVisible(delay);
	addAndMakeVisible(Master);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    osc.setSampleRate(sampleRate);
    rev.setSamplerate(sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    float** bufferArr = bufferToFill.buffer->getArrayOfWritePointers();
    int bufferLen = bufferToFill.numSamples;
    
    //do the lfo thing
    float temp[bufferLen];
    lfo.nextFrame(&temp[0], bufferLen);
    osc.setFreqOffset(temp[0]);
    
    if (Master.is_playing) {
        //get osc signal
        osc.nextFrame(bufferArr[0], bufferLen);
    }
    
    //run through a delay
    //delay.process(bufferArr[0], bufferLen);
    
    //will run through reverb
    rev.process(bufferArr[0], bufferLen);
    
    //then will run through Analogish IR
    
    for (int i = 1; i < bufferToFill.buffer->getNumChannels(); i++) {
        memcpy(bufferArr[i], bufferArr[0], sizeof(float) * bufferLen);
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::grey);

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	auto area = getLocalBounds();

	int headerHeight = 40;

	auto compHeight = getHeight() / 4;

	//for header 
	area.removeFromTop(headerHeight);
	

	osc.setBounds(area.removeFromTop(compHeight));
	lfo.setBounds(area.removeFromTop(compHeight));

	int masterHeight = 70;
	Master.setBounds(area.removeFromBottom(masterHeight));
	
	//the rest is for the lower box area 
	vol.setBounds(area.removeFromRight(50));
	delay.setBounds(area);


}
