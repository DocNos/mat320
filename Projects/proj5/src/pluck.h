#include <iostream>
#include <fstream>
#include <queue>
#include <random>
#include <cmath>
#include <vector>
#include <string>
#include <chrono>
using namespace std;
using timePoint = chrono::time_point<chrono::high_resolution_clock>;;
timePoint NowTime() { return chrono::high_resolution_clock::now();}
chrono::microseconds Duration(timePoint start, timePoint end) 
    { return chrono::duration_cast<chrono::microseconds>(end - start); }
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

    //friend ostream& operator<<(ostream& os, const WavHeader& wav_h)
    //{
    //    os << wav_h.chunkID << wav_h.chunkSize << wav_h.format;
//
    //    os << wav_h.fmtLabel << wav_h.fmtSize << wav_h.audioFormat;
    //    os << wav_h.numChannels << wav_h.sampleRate << wav_h.byteRate;        
    //    os << wav_h.blockAlign << wav_h.bitsPerSample;
    //    
    //    os << wav_h.subchunk2ID << wav_h.dataSize;
    //    return os;
    //}                          
};

struct FilterParams 
{
    float delayLen;     // [D] Exact delay length
    int stepLen;        // [L] Integer delay length, queue step
    float delta;        // Fractional delay
    float ap_Coeff;     // Allpass filter coefficient - delay queue can only hold whole numbers
    // for small phase, reduces to (1 - δ) / (1 + δ)
};

class Pluck
{
private:
    WavHeader header_;
    queue<float> delayQueue_;
    vector<float> semitones_;
    vector<float> input_;
    vector<int16_t> output_;
    

    unsigned duration_;
    float frequencyBase_;
    float lp_Prev_;
    float ap_PrevInput_;
    float ap_PrevOutput_;

    unsigned SAMPLE_RATE = 44100;
    unsigned BITS_PER_SAMPLE = 16;
    int CLAMP_RANGE = 15000;
    float STD_R = 0.99985f;
    unsigned RANDOM_SAMPLES = 100;
    float LOWPASS_COEFF = 0.5f;

public:
    Pluck(unsigned _duration, float _frequency)
    : delayQueue_(queue<float>()), semitones_(vector<float>(_duration))
    , duration_(_duration), frequencyBase_(_frequency), lp_Prev_(0.f)
    , ap_PrevInput_(0.f), ap_PrevOutput_(0.f)
    {
        header_ = createDefault(duration_, 1, SAMPLE_RATE, BITS_PER_SAMPLE);
/*----| 
    [0]F -->> [1](F)2* 2/12 -->> [2](F)2* 4/12 -->> [3](F)2* 5/12 -->> 
    [4](F)2*  7/12 -->> [5](F)2* 9/12 -->> 
    [6](F)2* 11/12 -->> [7](F)2* 12/12              |---*/
        
        int steps[8] = {0, 2, 4, 5, 7, 9, 11, 12};
        for(unsigned i = 0; i < duration_; ++i)
        {           
            semitones_[i] = frequencyBase_ *
                pow(2.f,(steps[i])/ 12.f);                             
        }
        
        //parameters_ = calculateParameters(frequencyBase_);

        // for(unsigned i = 0; i < parameters_.stepLen; ++i) 
        //     delayQueue_.push(0);
        input_ = vector<float>(duration_ * header_.sampleRate, 0.f);
        output_ = vector<int16_t>(duration_ * header_.sampleRate);   
        // genRandom();
    }

    WavHeader createDefault(unsigned audioDuration, uint16_t numChannels
        , unsigned sampleRate, uint16_t bitsSample)
    {
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
    
    FilterParams calculateParameters(float frequency)
    {
        float delayLen = header_.sampleRate / frequency;
        int delayStep = static_cast<int>(floor(delayLen));
        float delayDelta = delayLen - delayStep;
        FilterParams params =
        {
            delayLen
            , delayStep
            , delayDelta
            , (1.f - delayDelta) / (1.f + delayDelta)
        };
        return params;
    }

    void genRandom()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> 
            dis(-CLAMP_RANGE, CLAMP_RANGE);

        for(unsigned i = 0; i < RANDOM_SAMPLES; i++) 
        {
            input_[i] = (dis(gen));
        }
    }

    void Reset(FilterParams curr)
    {
        while(!delayQueue_.empty()) delayQueue_.pop();
        for( int i = 0; i < curr.stepLen; ++i)
            delayQueue_.push(0);
        fill(input_.begin(), input_.end(), 0.f);
        lp_Prev_ = 0.f;
        ap_PrevInput_ = 0.f;
        ap_PrevOutput_ = 0.f;
    }
    
    void WriteWav(string);
    void Execute();    
    void Delay(FilterParams, int&);
    // y[n] = 0.5 * (x[n] + x[n-1])
    float Lowpass(float);
    // y[n] = -a * y[n-1] + x[n-1] + a * x[n]
    float Allpass(FilterParams, float);
};


/*
struct fileheader
{
	char riff_label[4]; // (00) = {'R','I','F','F'}
	unsigned riff_size; // (04) = 36 + data_size
	char file_tag[4]; // (08) = {'W','A','V','E'}
	char fmt_label[4]; // (12) = {'f','m','t',' '}
	unsigned fmt_size; // (16) = 16
	unsigned short audio_format; // (20) = 1
	unsigned short channel_count; // (22) = 1 or 2
	unsigned sampling_rate; // (24) = (anything)
	unsigned bytes_per_second; // (28) = (see above)
	unsigned short bytes_per_sample; // (32) = (see above)
	unsigned short bits_per_sample; // (34) = 8 or 16
	char data_label[4]; // (36) = {'d','a','t','a'}
	unsigned data_size; // (40) = # bytes of data
};


*/