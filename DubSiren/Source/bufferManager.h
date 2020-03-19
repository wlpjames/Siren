/*
  ==============================================================================

    bufferManager.h
    Created: 23 Jan 2020 1:35:12pm
    Author:  William James
    
    Will dole out array pointers in order to ensure long buffers will be split
    specificaly a way to de-problematize the android iregular systems
  ==============================================================================
*/

#pragma once

class bufferManager {
    
    int currIndex;
    int idealBufferSize = 1024;
    
    
public:
    
    bufferManager()
    {
        
    }
    
    
    int getNextBufferLen(bool forNewBuffer, int bufferInSize) {
        
        if (forNewBuffer){
            //reset values for a new buffer
            currIndex = 0;
        }

        if (currIndex >= bufferInSize) {
            //check if buffer has been filled, return 0 if so
            return 0;
        }
        else if (bufferInSize < 1025) {
            currIndex += bufferInSize;
            return bufferInSize;
        }

        if (currIndex + idealBufferSize > bufferInSize) {
            int adf = 0;
        }

        int output = currIndex + idealBufferSize > bufferInSize? bufferInSize - currIndex : idealBufferSize;
        currIndex += idealBufferSize;



        return output;

    }
    
    
};
