#include <iostream>
#include <fstream>
#include <queue>
#include <random>
#include <cmath>
#include <vector>
#include <string>

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
    unsigned sampleRate;
    unsigned bitsPerSample;
    float R_Val;
    unsigned numRand_Samples;
    int clampRange;
    
};
// 220, 440, 261.63
// C Minor blues: C, E♭, F, G♭, G, and B♭
// A minor blues: A, C, D, E♭, E, and G
inline FilterPreset GetPreset(int preset)
{
    float middleC = 261.63;
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
            float freq = middleC;  
            float rVal = percussionR;  
            FilterParams params = 
                calculateParameters(Minor5, sampleRate, freq, 0.5f);  
            WavHeader header = 
                createHeader(params, sampleRate, numChannels, bitsSample);        
            FilterPreset preset = 
            {
                params, header, Minor5
                , sampleRate, bitsSample, rVal
                , 10, clampRange
            };
            return preset;
        }break;
        case(1):
        {
            float freq = a4;   
            float rVal = leadR;   
            FilterParams params = 
                calculateParameters(Minor5, sampleRate, freq, 0.5f);  
            WavHeader header = 
                createHeader(params, sampleRate, numChannels, bitsSample);        
            FilterPreset preset = 
            {
                params, header, Minor5
                , sampleRate, bitsSample, rVal
                , 10, clampRange
            };
            return preset;
        }break;
        default: return FilterPreset();

    }
}

class WavHandler
{
public:
    unsigned SAMPLE_RATE = 44100;
    unsigned BITS_PER_SAMPLE = 16;
    int CLAMP_RANGE = 15000;
    float STD_R = 0.99985f;
    unsigned RANDOM_SAMPLES = 100;
    float LOWPASS_COEFF = 0.5f;


public:
    void ReadWav(string);
    WavHeader CreateHeader();
};

class Sample
{
public:
    WavHeader header_;
    FilterParams params_;

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
            int steps[6] = {0, 3, 5, 6, 7, 10};
            for(unsigned i = 0; i < dur; ++i)
            {           
                semitones[i] = params.baseFrequency *
                    pow(2.f,(steps[i])/ 12.f);                             
            }  
        }break;
        default: break;

    }
    return semitones;
    
}


inline WavHeader createHeader(FilterParams params, float sampleRate
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
    , float frequency, float lowpass_coeff)
    {
        int steps = (scale == Major) ? (8) : (6);
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