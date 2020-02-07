/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/
#include <iostream>
#include <fstream>
 #include "MainComponent.h"
#include "oscComp.h"
#include "lfoComp.h"
#include "delayComp.h"

using namespace std;

//==============================================================================
MainComponent::MainComponent()
{


    //setAudioChannels (0, 2);
    
    // specify the number of input and output channels that we want to open
    auto audioDevice = deviceManager.getCurrentAudioDevice();
    auto numInputChannels  = (audioDevice != nullptr ? audioDevice->getActiveInputChannels() .countNumberOfSetBits() : 0);
    auto numOutputChannels = jmax (audioDevice != nullptr ? audioDevice->getActiveOutputChannels().countNumberOfSetBits() : 2, 2);

    setAudioChannels (numInputChannels, numOutputChannels);
    

	addAndMakeVisible(osc);
	addAndMakeVisible(lfo);
	addAndMakeVisible(delay);
	addAndMakeVisible(Master);
    addAndMakeVisible(sends);
    addAndMakeVisible(header);
    
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
    delay.setInputSampleRate(sampleRate);
    
    //prepare reverb
    reverb.setSamplerate(sampleRate);
    reverb.loadImpulse(BinaryData::tubby_wav, BinaryData::tubby_wavSize);
    
    //prepare amptoner
    //amp.setSamplerate(sampleRate);
    //amp.loadImpulse(BinaryData::cassette_recorder_wav, BinaryData::cassette_recorder_wavSize);
    //amp.setMix(0.75); // try uping it?

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    
    if ( !Process::isForegroundProcess()  ) {
        delay.reset();
        envelope.reset();
        amp.reset();
        reverb.reset();
        Master.reset();
        return;
    }
    
    //bufferToFill.clearActiveBufferRegion();
    float** bufferArr = bufferToFill.buffer->getArrayOfWritePointers();
    int bufferLen = bufferToFill.numSamples;

    envelope.setPlaying(Master.is_playing);
    if ( envelope.currStage != stages::off ) {
        osc.setFreqOffset(lfo.getVal(bufferLen));
        osc.nextFrame(bufferArr[0], bufferLen);
        envelope.proccess(bufferArr[0], bufferLen);
        clipa.process(bufferArr[0], bufferLen);
    }
    //run through a delay
    delaySendVal = sends.delayVal;
    memcpy(delaySend, bufferArr[0], bufferLen * sizeof(float));
    delay.process(delaySend, bufferLen);
    
    reverbSendVal = sends.reverbVal;
    float dryOutputVal = sends.outputVal;
    
    //sum the inputs of the sends
    for (int i = 0; i < bufferLen; i++) {
        reverbSend[i] = bufferArr[0][i] + (delaySend[i] * delaySendVal);
    }
    
    //send output and delay to the reverb send
    reverb.process(reverbSend, bufferLen);
    
    //sum the outputs of the send
    for (int i = 0; i < bufferLen; i++) {
        bufferArr[0][i] = (bufferArr[0][i] * dryOutputVal) + (reverbSend[i] * reverbSendVal) + (delaySend[i] * delaySendVal);
    }
    
    //run through an amp
    //amp.process(bufferArr[0], bufferLen);
    
    //do some more wave shaping
    clipa.process(bufferArr[0], bufferLen);

    //copy all from the chanell to any other chanels
    for (int i = 1; i < bufferToFill.buffer->getNumChannels(); i++) {
        memcpy(bufferArr[i], bufferArr[0], sizeof(float) * bufferLen);
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    // For more details, see the help for AudioProcessor::releaseResources()
    
    delay.reset();
    envelope.reset();
    amp.reset();
    reverb.reset();
    
    
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
	
    //for header
    int headerHeight = 100;
    header.setBounds(area.removeFromTop( headerHeight ));
    

	auto compHeight = getHeight() / 5;

	osc.setBounds(area.removeFromTop( compHeight ));
	lfo.setBounds(area.removeFromTop( compHeight ));
    
    sends.setBounds(area.removeFromRight(area.getWidth() / 2.5));
	delay.setBounds(area.removeFromTop(area.getHeight() / 2.2));
    
    Master.setBounds(area);

}
