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


class convolva    : public Component

{
public:
    
    Slider send;
    juce::dsp::Convolution convolution;
    bool is_prepared = false;
    bool is_loaded = false;
    float mix;
    float* tempMix;
    const int MAXBUFFERLEN = 2048;
    
    convolva(float Mix = 0.9)
    {
        mix = Mix;
        //volume
        addAndMakeVisible(&send);
        send.setRange(0, 1);
        send.setSliderStyle(send.LinearBarVertical);
        
        //initiate the tepmix
        tempMix = new float[MAXBUFFERLEN];
    }

    ~convolva()
    {
        delete[] tempMix;
    }

    void paint (Graphics& g) override
    {
        return;
    }

    void resized() override
    {
        return;
    }
    
    void reset()
    {
        convolution.reset();
        delete[] tempMix;
        tempMix = new float[MAXBUFFERLEN];
    }
    
    void process(float* signal, int siglen)
    {
        
        //dont process untill samplerate is set.
        if (!is_prepared || !is_loaded || siglen > MAXBUFFERLEN) {
            return;
        }
        
        //copy data over before mixing back in.
        memcpy(tempMix, signal, sizeof(float) * siglen);

        //proccess
        dsp::AudioBlock<float> block (&tempMix, 1, siglen);
        convolution.process(ProcessContextReplacing<float> (block));
        
        //loop through to apply a mix
        for (int i = 0; i < siglen; i++) {
            signal[i] = (tempMix[i] * mix) + (signal[i] * (1-mix));
        }
    }
    
    void setSamplerate(int sr) {
        
        //load spec, with sample rate
        juce::dsp::ProcessSpec spec;
        spec.numChannels = 1;
        spec.sampleRate = sr*2;
        spec.maximumBlockSize = 2048;
        convolution.prepare(spec);
        
        //get everything else ready
        convolution.reset();
        is_prepared = true;
    }
    
    void loadImpulse(const char* file, const int fileSize)
    {
        //loads a file from the rescourses folder
        
        //get IR file
        auto dir = File::getCurrentWorkingDirectory();
        int numTries = 0;
        while (! dir.getChildFile ("Resources").exists() && numTries++ < 15) {
            dir = dir.getParentDirectory();
        }
        convolution.loadImpulseResponse (file, fileSize, false, false, 0);
        is_loaded = true;
    }
    
    void setMix(float val)
    {
        if (val <= 1.0 && val >= 0.0) {
            mix = val;
        }
        return;
    }
    
private:
    enum
    {
        convolutionIndex               // [2]
    };
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (convolva)
    
};
