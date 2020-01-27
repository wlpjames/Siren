/*
  ==============================================================================

    env.h
    Created: 12 Dec 2019 3:43:04pm
    Author:  William James
    
    A supper simple, non vaviable envalope for a dub siren

  ==============================================================================
*/

#pragma once

enum stages {
    attacking=0, playing, decaying, off
};

class env {
    
public:
    
    float a, d; //attack + decay in seconds
    int samplerate;
    int curr = 0;
    float att_inc, dec_inc;
    float coef = 0;
    stages currStage;
    
    /*
     A : attack lengths in seconds
     D : decay length in seconds
    */
    env(float A, float D)
    {
        a = A;
        d = D;
    }
    
    void reset()
    {
        currStage = stages::off;
        curr = 0;
        coef = 0;
    }
    
    void prepare(int SampleRate)
    {
        samplerate = SampleRate;
        att_inc = 1.0 / (a * samplerate);
        dec_inc = 1.0 / (d * samplerate);
    }
    
    void proccess(float* signal, int sigLen)
    {

        //loop through
        for (int i = 0; i < sigLen; i++) {
            
            if (currStage == stages::off) {
                coef = 0.0;
            }
            else if (currStage == stages::playing) {
                return;
            }
            else if (currStage == stages::attacking) {
                coef += (att_inc);
                if( coef >= 1.0) {
                    currStage = playing;
                    return;
                }
            }
            else if (currStage == stages::decaying) {
                coef -= (dec_inc);
                if( coef <= 0.0) {
                    currStage = off;
                    coef = 0.0;
                }
            }
            
            signal[i] *= coef;
        }
        
    }
    
    void setPlaying(bool val)
    {

        if (val && (currStage == stages::off || currStage == stages::decaying)) {
            currStage = stages::attacking;
        }
        else if (val == false && ( currStage == stages::playing || currStage == stages::attacking )) {
            currStage = stages::decaying;
        }
        return;
    }
    
public:
};
