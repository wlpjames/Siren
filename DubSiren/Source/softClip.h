/*
  ==============================================================================

    softClip.h
    Created: 22 Nov 2019 5:53:48pm
    Author:  William James

  ==============================================================================
*/

#pragma once

class softClippa
{

public:
    float mix;
    float e;
    
    softClippa(float Mix = 0.7, float E = 2.0)
    {
        mix = Mix;
        return;
    }

    ~softClippa()
    {
        return;
    }
    
    void process(float* signal, int sigLen)
    {
        
        for (int i = 0; i < sigLen; i++) {
            //but it through a soft clipping
            float val  = ((2.0 / (1.0 + pow(e, 0-(signal[i] * e)))) - 1);
            
            //mix to the signal
            signal[i] = (val * mix) + (signal[i] * (1 - mix));
        }
        return;
    }
    
    void setE(float val) {
        e = val;
    }
    
    void setMix(float val)
    {
        if (val >= 0 && val <= 1) {
            mix = val;
        }
    }
    
};
