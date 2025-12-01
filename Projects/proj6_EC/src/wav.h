#include <iostream>
#include <fstream>
#include <queue>
#include <random>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

enum Scale
{
    Major, Minor5
};

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

struct FilterParams 
{
    float baseFrequency;
    unsigned duration;
    float delayLen;     // [D] Exact delay length
    int stepLen;        // [L] Integer delay length, queue step
    float delta;        // Fractional delay
    float ap_Coeff;     // Allpass filter coefficient - delay queue can only hold whole numbers
    // for small phase, reduces to (1 - δ) / (1 + δ)
    float lowpass_Coefficent;
};
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
    
};

inline vector<float> CreateSemitones
(FilterParams params, Scale scale)
{
    unsigned dur = params.duration;
    vector<float> semitones = vector<float>(dur);
    switch(scale)
    {
        case(Major): 
        {
            int steps[8] = {0, 2, 4, 5, 7, 9, 11, 12};
            for(unsigned i = 0; i < dur; ++i)
            {           
                semitones[i] = params.baseFrequency *
                    pow(2.f,(steps[i])/ 12.f);                             
            }            
        } break;
        case(Minor5):
        {
            int steps[8] = {0, 3, 5, 6, 7, 10, 12, 15};
            for(unsigned i = 0; i < dur; ++i)
            {
                semitones[i] = params.baseFrequency *
                    pow(2.f,(steps[i % 8])/ 12.f);
            }  
        }break;
        default: break;

    }
    return semitones;
    
}


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

inline FilterParams calculateParameters
(Scale scale, unsigned sampleRate
    , float frequency, float lowpass_coeff, unsigned numNotes)
    {
        unsigned steps = numNotes;
        //unsigned steps = (scale == Major) ? (8) : (6);
        float delayLen = sampleRate / frequency;
        int delayStep = static_cast<int>(floor(delayLen));
        float delayDelta = delayLen - delayStep;
        FilterParams params =
        {
            frequency, steps, delayLen
            , delayStep
            , delayDelta
            , (1.f - delayDelta) / (1.f + delayDelta)
            , lowpass_coeff
        };
        return params;
    }

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

// 220, 440, 261.63
// C Minor blues: C, E♭, F, G♭, G, and B♭
// A minor blues: A, C, D, E♭, E, and G
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
        case(0):
        {
            float freq = c3;  
            float rVal = rhythmR;  
            FilterParams params = 
                calculateParameters(Minor5, sampleRate, freq, 0.5f, 32);  
            WavHeader header = 
                createHeader(params, sampleRate, numChannels, bitsSample);        
            FilterPreset preset = 
            {
                params, header, Minor5, 32
                , sampleRate, bitsSample, rVal
                , 10, clampRange
                , 0.25f
            };
            return preset;
        }break;
        case(1):
        {
            float freq = middleC;   
            float rVal = leadR;   
            FilterParams params = 
                calculateParameters(Minor5, sampleRate, freq, 0.5f, 8);  
            WavHeader header = 
                createHeader(params, sampleRate, numChannels, bitsSample);        
            FilterPreset preset = 
            {
                params, header, Minor5, 8
                , sampleRate, bitsSample, rVal
                , 10, clampRange
                , 1.f
            };
            return preset;
        }break;
        default: return FilterPreset();

    }
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