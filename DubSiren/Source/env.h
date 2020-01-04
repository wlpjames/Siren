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
    
    void prepare(int SampleRate)
    {
        samplerate = SampleRate;
        att_inc = 1.0 / (a * samplerate);
        dec_inc = 1.0 / (d * samplerate);
    }
    
    void proccess(float* signal, int sigLen)
    {

        if (currStage == stages::off) {
            for (int i = 0; i < sigLen; i++) {
                signal[i] = 0.0;
            }
            return;
        }
        else if (currStage == stages::playing) {
            return;
        }
        else if (currStage == stages::attacking) {
            coef += (att_inc*sigLen);
            if( coef >= 1.0) {
                currStage = playing;
                coef = 1.0;
                return;
            }
        }
        else if (currStage == stages::decaying) {
            coef -= (dec_inc*sigLen);
            if( coef <= 0.0) {
                currStage = off;
                coef = 0.0;
            }
        }
        //loop through
        for (int i = 0; i < sigLen; i++) {
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
