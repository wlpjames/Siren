/*
  ==============================================================================

    osc.h
    Created: 19 Sep 2019 3:05:45pm
    Author:  William James

  ==============================================================================
*/

#pragma once
#include <stdio.h>
#include <complex.h>
#include <stdlib.h>

//#include "WaveGen.hpp"


class waveGen {
        
public:
    
    enum waveType {
        sine = 0, square, triangle, saw, pulse
    };
    
    //properties
    int sampleRate;
    float freq, vol;
    
    waveType type = sine;
    
    //params for sine
    std::complex <float> omega, last;
    const float PI = 4.F * atan(1.0f);
    const std::complex <float> I = std::complex <float> (0.0f, 1.0f);

    
    
    //methods
    waveGen(float Freq, float Vol, int SampleRate)
    {
        freq = Freq;
        vol = Vol;
        sampleRate = SampleRate;
        
        last = I;
        omega = getOmega();
    }
    
    void nextFrame(float* signal, int sigLen)
    {
        switch (type) {
            case sine : {getSine(signal, sigLen); return;}
            case square : {getSquare(signal, sigLen); return;}
            case saw : {return;} //getSaw(signal, sigLen);
            case triangle : {return;} //getTriangle(signal, sigLen);
            case pulse : {return;}
        }
        
        return;
    }
    
    std::complex <float> getOmega()
    {
        // a pre calculation of values used in the sine wave function
        //used only when the frequency is updated
        return exp(I * ((float) 2 * PI * freq / ((float) sampleRate)));
    }
    
    void getSine(float* signal, int sigLen)
    {
        for ( int i = 0; i < sigLen; i++) {
            signal[i] = (real(last)) * vol;
            last *= omega;
        }
        return;
    }
    
    void getSquare(float* signal, int sigLen)
    {
        
        for ( int i = 0; i < sigLen; i++) {
            signal[i] = (real(last)) * vol > 0 ? vol : 0-vol;
            last *= omega;
        }

    }
    
    void reset()
    {
        //starts from a new wave
        last = I;
        omega = getOmega();
    }
    
    //MARK: sets
    
    void setVol(float val)
    {
        vol = val;
        return;
    }
    
    void setFreq(float val)
    {
        freq = val;
        omega = getOmega();
        return;
    }
    
    void setSampleRate(float val)
    {
        sampleRate = val;
        omega = getOmega();
        return;
    }
    
    void setType(int t)
    {
        type = static_cast<waveType>(t);
    }
    
};
