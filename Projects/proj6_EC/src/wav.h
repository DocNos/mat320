#include <iostream>
#include <fstream>
#include <queue>
#include <random>
#include <cmath>
#include <vector>
#include <string>

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
    unsigned sampleRate;
    unsigned bitsPerSample;
    float R_Val;
    unsigned numRand_Samples;
    
};
// 220, 440, 261.63
// C Minor blues: C, E♭, F, G♭, G, and B♭
// A minor blues: A, C, D, E♭, E, and G
FilterPreset GetPreset(int preset)
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
    
    switch(preset)
    {
        case(0):
        {
            unsigned sampleRate = 44100;
            float freq = middleC; 
            uint16_t numChannels = 1;
            uint16_t bitsSample = 16;             
            FilterParams params = 
                calculateParameters(sampleRate, freq, 0.5f);  
            WavHeader header = 
                createHeader(params, sampleRate, numChannels, bitsSample);        
            FilterPreset preset0 = 
            {
                params, header, sampleRate
                , bitsSample, 0.99985f, 10
            };
            return preset0;
        }break;
        case(1):
        {

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
(unsigned sampleRate, float frequency, float lowpass_coeff)
    {
        float delayLen = sampleRate / frequency;
        int delayStep = static_cast<int>(floor(delayLen));
        float delayDelta = delayLen - delayStep;
        FilterParams params =
        {
            delayLen
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