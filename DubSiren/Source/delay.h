/*
  ==============================================================================

    delay.h
    Created: 22 Nov 2019 1:27:12pm
    Author:  William James

  ==============================================================================
*/

#pragma once
#include "softClip.h"
#include "convolva.h"


class delay
{
    int write_ind = 0;
    int read_ind = 0;
    
    float mix, decay;
    int tapeLen, sampleRate;
    float* tape;
    const int max_tapeLen = 100000;
    
    float* temp; //usud in the proccessing befor the mix
    const int MAXSIGLEN = 2048;
    
    softClippa SC;
    convolva tapeimpulse;
    
public:
 
    delay(int TapeLen = 20000, float Mix = 1.0f, float Decay = 0.3f, int SampleRate = 48000)
    {
        tapeLen = TapeLen;
        mix = Mix;
        decay = Decay;
        sampleRate = SampleRate;
        
        //init arrays
        tape = (float*) calloc(max_tapeLen, sizeof(float*));
        temp = new float[MAXSIGLEN];
    }
    
    ~delay()
    {
        free(tape);
        delete[] temp;
    }
    
    void process(float* signal, int sigLen)
    {
        float x;
        
        for (int i = 0; i < sigLen; i++) {
            
            x = signal[i];
            
            //read the tape and mix with signal
            temp[i] = readSignal(signal[i]);
            
            //enter into array from write point
            writeSignal(x, sigLen);
            
            SC.process(&tape[i], 1);
            
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
        SC.process(temp, sigLen);
        tapeimpulse.process(temp, sigLen);
        
        for (int i = 0; i < sigLen; i++)
        {
            signal[i] = ( mix * 2 * temp[i] ) + ( (1-mix) * signal[i] );
        }
        
    }
    
    void writeSignal(float signal, int sigLen)
    {
        tape[write_ind] = (tape[write_ind] * decay) + signal; //try a softclip here
    }
    
    float readSignal(float signal)
    {
        return tape[read_ind];
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
        tapeimpulse.setSamplerate(sampleRate);
        tapeimpulse.loadImpulse(BinaryData::cassette_recorder_wav, BinaryData::cassette_recorder_wavSize);
        tapeimpulse.setMix(0.8);
    }
};
