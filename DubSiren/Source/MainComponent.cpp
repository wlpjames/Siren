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
    
    // specify the number of input and output channels that we want to open
    auto audioDevice = deviceManager.getCurrentAudioDevice();
    auto numInputChannels  = (audioDevice != nullptr ? audioDevice->getActiveInputChannels() .countNumberOfSetBits() : 0);
    auto numOutputChannels = jmax (audioDevice != nullptr ? audioDevice->getActiveOutputChannels().countNumberOfSetBits() : 2, 2);

    setAudioChannels (numInputChannels, numOutputChannels);
    /*
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
        setAudioChannels (2, 2);
    }
     */

    
	addAndMakeVisible(osc);
	addAndMakeVisible(lfo);
	addAndMakeVisible(delay);
	addAndMakeVisible(Master);
    addAndMakeVisible(sends);
    
    
    setSize (300, 600);
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
    envelope.prepare(sampleRate);
    lfo.setSampleRate(sampleRate);
    delay.setSampleRate(sampleRate);
    
    //prepare reverb
    reverb.setSamplerate(sampleRate);
    reverb.loadImpulse(BinaryData::tubby_wav, BinaryData::tubby_wavSize);
    
    //prepare amptoner
    amp.setSamplerate(sampleRate);
    amp.loadImpulse(BinaryData::guitar_amp_wav, BinaryData::guitar_amp_wavSize);
    amp.setMix(0.5);
    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    float** bufferArr = bufferToFill.buffer->getArrayOfWritePointers();
    int bufferLen = bufferToFill.numSamples;
    
    //zero the array
    for (int i = 0; i <bufferLen; i++)
    {
        bufferArr[0][i] = 0.0f;
    }
    
    //do the lfo thing
    osc.setFreqOffset(lfo.getVal(bufferLen));
    
    //do the env thing
    envelope.setPlaying(Master.is_playing);
    
    //get osc signal
    osc.nextFrame(bufferArr[0], bufferLen);
    envelope.proccess(bufferArr[0], bufferLen);
    
    //run through a delay
    float delaySendVal = sends.delayVal;
    float delaySend[1000];
    memcpy(delaySend, bufferArr[0], bufferLen * sizeof(float));
    delay.process(delaySend, bufferLen);
    
    float reverbSendVal = sends.reverbVal;
    float reverbSend[1000];
    memcpy(reverbSend, bufferArr[0], bufferLen * sizeof(float));
    reverb.process(reverbSend, bufferLen);
    
    float dryOutputVal = sends.outputVal;
    

    for (int i = 0; i < bufferLen; i++) {
        bufferArr[0][i] = (bufferArr[0][i] * dryOutputVal) + (delaySend[i] * delaySendVal) + (reverbSend[i] * reverbSendVal);
    }
    
    //run through an amp
    amp.process(bufferArr[0], bufferLen);
    
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
    g.setColour(Colours::black);
    g.fillAll();
}

void MainComponent::resized()
{

	auto area = getLocalBounds().reduced(5);
    area.removeFromBottom(25);
	int headerHeight = 70;

	auto compHeight = getHeight() / 5;

	//for header 
	area.removeFromTop( headerHeight );

	osc.setBounds(area.removeFromTop( compHeight ));
	lfo.setBounds(area.removeFromTop( compHeight ));
    
    sends.setBounds(area.removeFromRight(area.getWidth() / 2.5));
	delay.setBounds(area.removeFromTop(area.getHeight() / 2.2));
    
    Master.setBounds(area);

}
