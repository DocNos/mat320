#ifndef WAV_H
#define WAV_H

#include <iostream>
#include <fstream>
#include <queue>
#include <random>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

// #include "filter.h"
using namespace std;

// Header is interpreted sequentially by a parser. 
struct WavHeader 
{
    //------------- File Container Fields
       char chunkID[4];        // RIFF (Resource Interchange File Format)
       uint32_t chunkSize;     // File size minus this and preceeding data
       char format[4];         // "WAVE"

    //------------ Format Description Fields
       char fmtLabel[4];       // Indicates start of format description
       uint32_t fmtSize;       // Size of format description (16 for basic Pulse Code Modulation)
       uint16_t audioFormat;   // 1 for PCM means uncompressed raw audio
       uint16_t numChannels;   // 1 for mono
       uint32_t sampleRate;    // 44100 (CD Quality)
       uint32_t byteRate;      // How many bytes for each second of audio [sampleRate * numChannels * bitsPerSample/8]
       uint16_t blockAlign;    // Describes how many bytes for a complete sample on all channels [numChannels * bitsPerSample/8]
       uint16_t bitsPerSample; // 16       

    //------------- Data Description Fields   
       char subchunk2ID[4];    // Indicates start of data description
       uint32_t dataSize;      // Size of just the audio samples (exculding size of header) 
                               // NumSamples * numChannels * bitsPerSample/8                         
};

inline vector<int16_t> Mix
(const vector<vector<int16_t>>& voices, unsigned numSamples)
{
    int numVoices = voices.size();
    vector<int16_t> mixed(numSamples);
    
    for(unsigned i = 0; i < numSamples; ++i)
    {
        int32_t sum = 0;
        for(int j = 0; j < numVoices; ++j)
        {
            sum += voices[j][i];
        }
        mixed[i] = static_cast<int16_t>((sum / numVoices));
    }
    return mixed;
}

inline vector<int16_t> BoostAmplitude(vector<int16_t> samples, int16_t boost)
{
    for(size_t i = 0; i < samples.size(); ++i) 
    {
        int32_t boosted = 
        static_cast<int32_t>(samples[i]) * boost;
        samples[i] = 
        static_cast<int16_t>
        (clamp(boosted, -32768, 32767));
    }
    return samples;
}

inline void WriteWav(const string& filename, const WavHeader& header, const vector<int16_t>& samples)
{
    ofstream wavFile(filename, ios::binary);
    if (!wavFile.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing" << endl;
        return;
    }

    // Write the header
    wavFile.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));

    // Write the sample data
    wavFile.write(reinterpret_cast<const char*>(samples.data()), samples.size() * sizeof(int16_t));

    wavFile.close();
    cout << "WAV file written: " << filename << " (" << samples.size() << " samples)" << endl;
}


/*
struct FilterPreset
{
    FilterParams params;
    WavHeader header;
    Scale scale;
    unsigned numNotes;
    unsigned sampleRate;
    unsigned bitsPerSample;
    float R_Val;
    unsigned numRand_Samples;
    int clampRange;
    float noteDuration;
    
};*/

/*
inline WavHeader createHeader(Filter filter)
{
    FilterParams params = filter.baseParams_;
    unsigned audioDuration = params.duration;
    unsigned sampleRate = params.sampleRate;
    unsigned bitsSample = params.bitsPerSample;

    unsigned headerSize = sizeof(WavHeader) 
        - (sizeof(WavHeader::chunkID) + sizeof(WavHeader::chunkSize));
    unsigned numSamples = audioDuration * sampleRate;
    unsigned byteRate = sampleRate * params.numChannels * bitsSample/8;
    uint16_t blockAlignment = params.numChannels * bitsSample/8;
    unsigned totalSize = numSamples * params.numChannels * bitsSample/8;
    WavHeader header = 
    {
        {'R', 'I', 'F', 'F'}   // Chunk ID
        , headerSize + totalSize    
        , {'W', 'A', 'V', 'E'} // File Format
        , {'f', 'm', 't', ' '} // sub-chunk ID
        , 16                   // sub-chunk size
        , 1                    // Audio Format (??)
        , params.numChannels          // Mono: 1 Channel
        , sampleRate           // Sample rate
        , byteRate          
        , blockAlignment                 
        , bitsSample       
        , {'d', 'a', 't', 'a'} // this data is made out of data
        , totalSize
    };
    return header;
}
*/

/*
inline WavHeader createHeader(FilterParams params, uint32_t sampleRate
, uint16_t numChannels, uint16_t bitsSample)
{
    auto audioDuration = params.duration;
    
    unsigned headerSize = sizeof(WavHeader) 
        - (sizeof(WavHeader::chunkID) + sizeof(WavHeader::chunkSize));
    unsigned numSamples = audioDuration * sampleRate;
    unsigned byteRate = sampleRate * numChannels * bitsSample/8;
    uint16_t blockAlignment = numChannels * bitsSample/8;
    unsigned totalSize = numSamples * numChannels * bitsSample/8;
    WavHeader header = {
        {'R', 'I', 'F', 'F'}   // Chunk ID
        , headerSize + totalSize    
        , {'W', 'A', 'V', 'E'} // File Format
        , {'f', 'm', 't', ' '} // sub-chunk ID
        , 16                   // sub-chunk size
        , 1                    // Audio Format (??)
        , numChannels          // Mono: 1 Channel
        , sampleRate           // Sample rate
        , byteRate          
        , blockAlignment                 
        , bitsSample       
        , {'d', 'a', 't', 'a'} // this data is made out of data
        , totalSize
    };
    return header;
}
*/


// 220, 440, 261.63
// C Minor blues: C, E♭, F, G♭, G, and B♭
// A minor blues: A, C, D, E♭, E, and G
/*
inline FilterPreset GetPreset(int preset)
{
    float middleC = 261.63;
    float c3 = middleC / 2;
    float middleD = 293.66;
    float eFlat = 311.13;
    float middleE = 329.63;
    float middleF = 349.23;
    float fSharp = 369.99;
    float gFlat = fSharp;
    float middleG = 392;
    float a4 = 440;
    float bFlat = 466.16; 
    int clampRange = 15000; 
    
    unsigned sampleRate = 44100;    
    uint16_t numChannels = 1;
    uint16_t bitsSample = 16; 

    float percussionR = 0.990f;
    float rhythmR = 0.995f;
    float leadR = 0.99985f;
    switch(preset)
    {
        // Percussion
        case(0):
        {
            float freq = c3;  
            float rVal = percussionR;  
            FilterParams params = 
                calculateParameters(Major, sampleRate, freq, 0.5f, 32);  
            WavHeader header = 
                createHeader(params, sampleRate, numChannels, bitsSample);        
            FilterPreset preset = 
            {
                params, header, Major, 32
                , sampleRate, bitsSample, rVal
                , 10, clampRange
                , 0.25f
            };
            return preset;
        }break;
        // Lead
        case(1):
        {
            float freq = middleC;   
            float rVal = leadR;   
            FilterParams params = 
                calculateParameters(Minor5_Arpeggio, sampleRate, freq, 0.5f, 8);  
            WavHeader header = 
                createHeader(params, sampleRate, numChannels, bitsSample);        
            FilterPreset preset = 
            {
                params, header, Minor5_Arpeggio, 8
                , sampleRate, bitsSample, rVal
                , 10, clampRange
                , 1.f
            };
            return preset;
        }break;
        // Melody
        case(2):
        {
            float freq = middleC;  
            float rVal = rhythmR;  
            FilterParams params = 
                calculateParameters(Minor5, sampleRate, freq, 0.5f, 32);  
            WavHeader header = 
                createHeader(params, sampleRate, numChannels, bitsSample);        
            FilterPreset preset = 
            {
                params, header, Minor5_Arpeggio, 32
                , sampleRate, bitsSample, rVal
                , 10, clampRange
                , 0.25f
            };
            return preset;
        }break;
        default: return FilterPreset();

    }
}
*/


//struct FilterParams 
//{
//    float baseFrequency;
//    unsigned duration;
//    float delayLen;     // [D] Exact delay length
//    int stepLen;        // [L] Integer delay length, queue step
//    float delta;        // Fractional delay
//    float ap_Coeff;     // Allpass filter coefficient - delay queue can only hold whole numbers
//    // for small phase, reduces to (1 - δ) / (1 + δ)
//    float lowpass_Coefficent;
//};

#endif 