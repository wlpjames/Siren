/*
  ==============================================================================

    reverb.h
    Created: 29 Nov 2019 12:03:46pm
    Author:  William James

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
using namespace dsp;


class reverb    : public Component

{
public:
    
    Slider send;
    juce::dsp::Convolution convolution;

    reverb()
    {
        //volume
        addAndMakeVisible(&send);
        send.setRange(0, 1);
        send.setSliderStyle(send.LinearBarVertical);
    
        
        //get IR file
        auto dir = File::getCurrentWorkingDirectory();
        int numTries = 0;
        while (! dir.getChildFile ("Resources").exists() && numTries++ < 15) {
            dir = dir.getParentDirectory();
        }
        convolution.loadImpulseResponse (dir.getChildFile ("Resources").getChildFile ("guitar_amp.wav"), false, false, 512);
    }

    ~reverb()
    {
    }

    void paint (Graphics& g) override
    {
        return;
    }

    void resized() override
    {
        return;
    }
    
    void process(float* signal, int siglen)
    {
        float * cd[1] ={signal};
        //cd[0] = signal;
        dsp::AudioBlock<float> block (cd, 1, siglen);
        convolution.reset();
        convolution.process(ProcessContextReplacing<float> (block));
    }
    
    void setSamplerate(int sr) {
        juce::dsp::ProcessSpec spec;
        spec.numChannels = 1;
        spec.sampleRate = sr*2;
        spec.maximumBlockSize = 2048;
        convolution.prepare(spec);
    }
private:
    enum
    {
        convolutionIndex               // [2]
    };
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (reverb)
    
};
