/*
  ==============================================================================

    waveTable.h
    Created: 19 Sep 2019 3:05:54pm
    Author:  William James

  ==============================================================================
*/

#pragma once

#include <stdio.h>
#include <iostream>
#include <complex.h>
#include <stdlib.h>
#include <math.h>
#include "osc.h"



class waveTable {
    
public:
    float** tables[4];
    int num_tables;
    int numTypes = 4;

    float harmonicVals[4][200];
    
    int harm_vals_num = 50;
    const int highest_freq = 6000;
    float freq, desired_freq, vol, glideRate;
    float freqOffset = 1;
    int sampleRate;
    int table_index = 0;
    waveGen* osc;
    
    enum waveTypes {
        sine = 0, triangle, saw , square
    };
    
    //type
    waveTypes type;

    waveTable(float Freq, float Vol, int SampleRate, int Num_tables)
    {
        freq = Freq;
        vol = Vol;
        sampleRate = SampleRate;
        num_tables = Num_tables;
        glideRate = 1;
        type = sine;
        
        osc = new waveGen(freq, vol, sampleRate);
        
        //malloc the tables
        for (int i = 0; i < numTypes; i++) {
            tables[i] = (float**) calloc(num_tables, sizeof(float*));
        }
        
        harmonicVals[0][0] = 1.0;
        harmonicVals[1][0] = 1.0;
        harmonicVals[2][0] = 1.0;
        harmonicVals[3][0] = 1.0;
        
        //this needs to be sorted out, but in general, order of tables follows the
        
        for (int i = 1; i < harm_vals_num; i++) {
            
            harmonicVals[(int)waveTypes::sine][i] = 0; //the sine
            harmonicVals[(int)waveTypes::saw][i] = (1.0f / float(i+1)) / 4; //the saw
            
            //if i is odd
            if (i % 2 == 0) {
                harmonicVals[(int)waveTypes::square][i] = (1.0f / float(i+1)) / 4; //the square
                harmonicVals[(int)waveTypes::triangle][i] = (1.0f / float(pow(i+1, 2))); //the tri
            }
            else {
                harmonicVals[(int)waveTypes::square][i] = 0; //the square
                harmonicVals[(int)waveTypes::triangle][i] = 0; //the tri
            }
            
        }
        
        //exception for sine
        harmonicVals[0][0] = 1.0; //the sine
    }
    
    ~waveTable()
    {
        
        //free all the table data
        for(int i = 0; i < numTypes; i++) {
            
            for (int j = 0; j < num_tables; j++) {
                
                if (tables[i][j] != nullptr) {
                    free(tables[i][j]);
                }
            }
        }
    }
    
    
    void createTables()
    {
            
        for (int i = 0; i < numTypes; i++) {
            
            for (int j = 0; j < num_tables; j++) {
                
                if (tables[i][j]) {
                    free(tables[i]);
                }
                //array of zeros
                tables[i][j] = (float*) calloc(sampleRate, sizeof(float*));
                
                
                buildTable((j + 1) * (highest_freq / num_tables), &tables[i][j][0], harmonicVals[i]); // temp only create system to choose where are harmonics
                
                normalize(&tables[i][j][0], sampleRate, 0.5);
            }
        }
    }
    
    void reset() {
        table_index = 0;
        return;
    }
    
    void buildTable(int band_max, float* table, float* harmonicV)
    {

        int num_harmonics = std::min(harm_vals_num, (sampleRate / 2) / band_max);
        
        float harmonic[sampleRate];
        //in 1s for saw?
        for (int i = 0; i < num_harmonics; i++) {
                
            if (harmonicV[i] == 0) {
                continue;
            }
            
            osc->reset();
            osc->setFreq((float) (i+1));
            osc->setVol(harmonicV[i]);
            osc->nextFrame(&harmonic[0], sampleRate);
            combineArrays(table, &harmonic[0]);
        }
        return;
    }
    
    void combineArrays(float* Arr1, float* Arr2)
    {
        //add to table;
        for (int i = 0; i < sampleRate; i++) {
            Arr1[i] += Arr2[i];
        }
        return;
    }
    
    void nextFrame(float* signal, int sigLen)
    {
        freq = calc_freq();
        float* band = selectBand();
        for (int i = 0; i < sigLen; i++) {
            
            table_index += freq;
            
            if (table_index >= sampleRate) {
                table_index -= sampleRate;
            }
            
            signal[i] = band[table_index] * vol; //found exception here
        }
        
    }
    
    float calc_freq()
    {
        //a function to act as a makeshift lp filte on fequency changes
        //gets called on evey buffe call
        //freqOsett is here set by the LFO with a val between 1 and 2 for a max 1 octave offset
        float adaptive_glide_rate = (desired_freq - freq) / (glideRate);
        return (freq + adaptive_glide_rate) * freqOffset;
    }
    
    void normalize(float* signal, int sigLen, float max_val)
    {
        //this is not very efficiant
        //find highest value
        float highVal = 0;
        for (int i = 0; i < sigLen; i++)
        {
            if (abs(signal[i]) > highVal) {
                highVal = abs(signal[i]);
            }
        }
        for (int i = 0; i < sigLen; i++) {
            signal[i] /= (highVal / max_val);
        }
    }
    
    float* selectBand()
    {
        
        //band type
        int typeind = static_cast<int>(type);
        
        
        //select a band at a frequency just above the set freq
        float band_increment = (float) highest_freq / (float) num_tables;
        float band_freq = band_increment;
        for (int i = 0; i < num_tables; i++) {
            if (band_freq >= freq) {
                return tables[typeind][i];
            }
            band_freq += band_increment;
        }
        
        return tables[typeind][0];
    }
    
    void setFreq(float val)
    {
        desired_freq = floor(val);
        return;
    }
    
    void setAmp(float val)
    {
        vol = val;
        return;
    }
    
    void setSampleRate(int Samplerate)
    {
        sampleRate = Samplerate;
        return;
    }
    
    void setGlideRate(int val)
    {
        glideRate = val;
    }
    
    void setFreqOffset(float val)
    {
        freqOffset = val;
    }
    
    void setType(int t)
    {
        type = static_cast<waveTypes>(t);
    }
};
