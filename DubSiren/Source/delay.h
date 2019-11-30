/*
  ==============================================================================

    delay.h
    Created: 22 Nov 2019 1:27:12pm
    Author:  William James

  ==============================================================================
*/

#pragma once
#include "softClip.h"

class delay
{
    int write_ind = 0;
    int read_ind = 0;
    
    float mix, decay;
    int tapeLen, sampleRate;
    float* tape;
    int max_tapeLen = 100000;
    
    softClippa SC;
    
public:
 
    delay(int TapeLen = 20000, float Mix = 0.3f, float Decay = 0.3f, int SampleRate = 48000)
    {
        tapeLen = TapeLen;
        mix = Mix;
        decay = Decay;
        sampleRate = SampleRate;
        
        tape = (float*) calloc(max_tapeLen, sizeof(float*));
    }
    
    ~delay()
    {
        free(tape);
    }
    
    void process(float* signal, int sigLen)
    {
        float x;
        for (int i = 0; i < sigLen; i++) {
            
            x = signal[i];
            
            //read the tape and mix with signal
            signal[i] = readSignal(signal[i]);
            
            //enter into array from write point
            writeSignal(x, sigLen);
            
            //SC.process(&tape[i], 1);
            
            //advance read points
            write_ind++;
            if (write_ind >= tapeLen) {
                write_ind = 0;
            }
            
            read_ind++;
            if (read_ind >= tapeLen) {
                read_ind = 0;
            }
            
        }
        
        //run the signal through a soft clipping algorythm
        SC.process(signal, sigLen);
        
    }
    
    void writeSignal(float signal, int sigLen)
    {
        tape[write_ind] = (tape[write_ind] * decay) + signal; //try a softclip here
    }
    
    float readSignal(float signal)
    {
        return (signal * (1-mix)) + (tape[read_ind] * mix);
    }
    
    void setMix(float val)
    {
        mix = val;
    }
    
    void setDecay(float val)
    {
        decay = val;
    }
    
    void setTapeLen(float sec)
    {
        int l = sampleRate * sec;
        
        if (l > max_tapeLen) {
            tapeLen = max_tapeLen;
            return;
        }
        
        tapeLen = l;
        return;
    }
    
    void setSampleRate(int val) {
        sampleRate = val;
    }
};
