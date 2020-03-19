

/*
  ==============================================================================

    tapeDelay.h
    Created: 04 Jan 2020 1:27:12pm
    Author:  William James

  ==============================================================================
*/

#pragma once
#include "softClip.h"
#include "convolva.h"


class tapeDelay
{
    int write_ind = 0;
    int read_ind = 0;
    
    float decay, delayTime;
    int inputSampleRate;
    
    const int nativeSR = 100000;
    float* tape;
    
    convolva tapeimpulse;
    softClippa SC;
    
    LagrangeInterpolator interpolator1;
    LagrangeInterpolator interpolator2;
    

public:
 
    tapeDelay( float DelayTime = 0.1f, float Decay = 0.3f) : tapeimpulse(0.85), SC(0.5)
    {

        delayTime = DelayTime;
        decay = Decay;
        
        //init arrays
        tape = (float*) calloc(nativeSR, sizeof(float));
        
        //this may not be necesary
        tapeimpulse.setSamplerate(nativeSR);
        tapeimpulse.loadImpulse(BinaryData::cassette_recorder_wav, BinaryData::cassette_recorder_wavSize);
        
    }
    
    ~tapeDelay()
    {
        free(tape);
    }
    
    void reset() {
        //will reinit all values to stop any distortion on app resating and navigation
        free(tape);
        tape = (float*) calloc(nativeSR, sizeof(float));
        interpolator1.reset();
        interpolator2.reset();
        tapeimpulse.reset();
    }
    
    void processToTape(float* signal, int sigLen)
    {
        float x;
        //run the signal through a soft clipping algorythm
        
        for (int i = 0; i < sigLen; i++) {
            
            x = signal[i];
            
            //read the tape and mix with signal
            signal[i] = readSignal();
            
            //enter into array from write point
            writeSignal(x);
            
            SC.process(&tape[write_ind], 1);

        }
        
    }
    
    void process(float* signal, int sigLen)
    {

        //resample signal
        float r =  ((float) nativeSR / delayTime) / (float) inputSampleRate;     // resampling rate (output/input) //change include tape len

        int ny = (sigLen * r);  // expected output size // risk of rounding errors
        
        float resampled[ny];
        
        int num_written = interpolator1.process(((float)inputSampleRate / ((float)nativeSR / delayTime)), signal, resampled, ny, sigLen, 0);

        
        //write resampled to tape, read from tape, and proccess
        processToTape(resampled, ny);
        //processToTape(signal, sigLen);

        //resample to this sample rate
        num_written = interpolator2.process(( ((float) nativeSR / delayTime) / (float) inputSampleRate ), resampled, signal, sigLen, ny, 0);
        tapeimpulse.process(signal, sigLen);
        //interpolator1.reset();
        //interpolator2.reset();
    }
    
    
    void writeSignal(float signal)
    {
        tape[ write_ind ] = ( tape[ write_ind ] * decay ) + signal; //try a softclip here
        write_ind++;
        if (write_ind >= nativeSR) {
            write_ind = 0;
        }
    }
    
    float readSignal()
    {
        float x = tape[read_ind++];
        if (read_ind >= nativeSR) {
            read_ind = 0;
        }
        return x;
    }
    
    void setDecay(float val)
    {
        decay = val;
    }
    
    void setDelayTime(float sec)
    {
        delayTime = sec;
    }
    
    void setInputSampleRate(int val) {

        //perhaps
        inputSampleRate = val;
        
    }
};
