#ifndef DATA_H
#define DATA_H
#include <map>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>
#include <random>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

enum Scale
{
    Major, Minor5, Minor5_Arpeggio
};

/*
@param centerFreq 
    Frequency to resonate around: θ = 2π * fc / fs
@param bandwidth Maximum bandwidth: R = exp(-π * BW / fs)
*/
struct ResonData 
{
    double centerFreq;
    double bandwidth;
};


/*
@param chunkID 
    RIFF (Resource Interchange File Format)
@param chunkSize 
    File size minus this and preceeding data
@param format 
    "W-A-V-E"
@param fmtLabel 
    Indicates start of format description
@param fmtSize 
    Size of format description (16 for basic Pulse Code Modulation)
@param audioFormat 
    1 for PCM means uncompressed raw audio
@param numChannels 
    1 for mono 
@param sampleRate 
    Defaults to 44100 for CD quality
@param byteRate 
    [sampleRate * numChannels * bitsPerSample/8]
    Number of bytes for each second of audio
@param blockAlign 
    [numChannels * bitsPerSample/8]
    Number of bytes for a complete sample on all channels
@param bitsPerSample
    Defaults to 16
@param subChunk2ID
    Indicates start of data description section
@param dataSize
    NumSamples * numChannels * bitsPerSample/8
    Size of just the audio samples (excludes size of header data)

*/
struct WavHeader 
{
    //------------- File Container Fields
       char chunkID[4];        
       uint32_t chunkSize;     
       char format[4];         

    //------------ Format Description Fields
       char fmtLabel[4];       
       uint32_t fmtSize;       
       uint16_t audioFormat;   
       uint16_t numChannels;   
       uint32_t sampleRate;    
       uint32_t byteRate;      
       uint16_t blockAlign;     
       uint16_t bitsPerSample;       

    //------------- Data Description Fields   
       char subchunk2ID[4];    
       uint32_t dataSize;       
                               
};

struct FilterParams
{
    //--------------- Timing
    unsigned sampleRate = 44100;
    unsigned duration = 8;
    unsigned numSamples = sampleRate * duration;
    double r_val = r_vals.at("rhythm");
    unsigned numNotes = duration;
    float noteDuration = 1;

    //--------------- Pitch
    double frequency = NoteFrequencies.at("middleC");
    Scale scale = Scale::Major;

    //--------------- Data Organization
    uint16_t bitsPerSample = 16;
    unsigned numRand_Samples = 10;
    uint16_t numChannels = 1;
    int clampRange = 15000; 

};

// ------------------------- Presets

const map<string, ResonData> ResonPresets = 
{
    {"vowel_a", {730, 50}},      // Formant for "ah" sound
    {"vowel_e", {530, 50}},      // Formant for "eh" sound
    {"vowel_i", {270, 40}},      // Formant for "ee" sound
    {"vowel_o", {570, 50}},      // Formant for "oh" sound
    {"vowel_u", {440, 40}},      // Formant for "oo" sound
    {"narrow_440", {440, 20}},   // Very narrow at A4
    {"wide_1000", {1000, 200}},  // Wide band at 1kHz
};

const map<string, double> 
NoteFrequencies = 
{
    {"middleC", 261.63}
    , {"c3", (261.63/2)}
    , {"middleD", 293.66}
    , {"eFlat", 311.13}
    , {"middleE", 329.63}
    , {"middleF", 349.23}
    , {"fSharp", 369.99}
    , {"gFlat", 369.99}
    , {"middleG", 392}
    , {"a4", 440}
    , {"bFlat", 466.16}
};

// R Vals for Plucked-String Implementation 
const map<string, float> 
r_vals = 
{
    {"percussion", 0.990f}
    , {"rhythm", 0.995f}
    , {"lead", 0.99985f}
};

// ------------------------------------

#endif