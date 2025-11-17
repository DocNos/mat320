#include <iostream>
#include <fstream>
#include <queue>
#include <random> 
#include <cmath>
#include <vector>

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
    float delayLen;     // [D] Exact delay length
    int stepLen;        // [L] Integer delay length, queue step
    float delta;        // Fractional delay
    float allpassCoeff; // Allpass filter coefficient - delay queue can only hold whole numbers
    // for small phase, reduces to (1 - δ) / (1 + δ)
};

class Pluck
{
private:
    WavHeader header_;
    FilterParams parameters_;
    queue<float> delayQueue_;
    vector<float> input_;
    vector<int16_t> output_;

    unsigned duration_;
    float frequency_;
    unsigned SAMPLE_RATE = 44100;
    unsigned BITS_PER_SAMPLE = 16;
    int CLAMP_RANGE = 15000;
    float STD_R = 0.99985f;
    unsigned RANDOM_SAMPLES = 100;

public:
    Pluck(unsigned _duration, float _frequency)
    : delayQueue_(queue<float>())
    , duration_(_duration)
    , frequency_(_frequency)
    {
        header_ = createDefault(duration_, 1, SAMPLE_RATE, BITS_PER_SAMPLE);
        parameters_ = calculateParameters(frequency_);

        for(unsigned i = 0; i < parameters_.stepLen; ++i) 
            delayQueue_.push(0);
        input_ = vector<float>(duration_ * header_.sampleRate);
        output_ = vector<int16_t>(duration_ * header_.sampleRate);   
        genRandom();
    }

    WavHeader createDefault(unsigned audioDuration, unsigned numChannels
        , unsigned sampleRate, unsigned bitsSample)
    {
        unsigned headerSize = sizeof(WavHeader) 
            - (sizeof(WavHeader::chunkID) + sizeof(WavHeader::chunkSize));

        unsigned numSamples = audioDuration * sampleRate;
        unsigned byteRate = sampleRate * numChannels * bitsSample/8;
        unsigned blockAlignment = numChannels * bitsSample/8;
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

        for(int i = 0; i < RANDOM_SAMPLES; i++) 
        {
            input_.push_back(dis(gen));
        }
    }

/*    void WriteWav()
    {
        short* samples = reinterpret_cast<short*>(wavData_);
        for(unsigned i = 0; i < count_; ++i)
        {
            // Convert float to short with clamping
            float val = filtered_[i];            
            if (val > CLAMP_RANGE) val = CLAMP_RANGE;
            if (val < -CLAMP_RANGE) val = -CLAMP_RANGE;
            samples[i] = static_cast<short>(val);
        }

        fstream out("output.wav",ios_base::binary|ios_base::out);
        out.write(header_, 44);
        out.write(wavData_, size_);
    } */


    void Lowpass();
    void Delay();

};

class Lowpass
{
public:
    // wavData should be float?
    char* wavData_; 
    float* filtered_;
    vector<float> normalizedInput_;

    char header_[44];   
    float coefficent_;
    int iterations_;

    unsigned size_;
    unsigned rate_;
    unsigned count_;

    unsigned _MAX16_ = 32767;


public:
    Lowpass(float _coefficent, int _N, char* wav)
    : coefficent_(_coefficent), iterations_(_N)
    {
        filtered_ = new float[count_];
    }
    ~Lowpass()
    {
        delete[] wavData_;
        delete[] filtered_;
    }
    
    void FilterEq()
    {
        // Use normalized input (±1.5 range)
        float* input = normalizedInput_.data();
        filtered_[0] = input[0];
        for(unsigned i = 1; i < count_; ++i)
        {
            filtered_[i] = input[i] + coefficent_ * input[i-1];
        }
        for(int j = 1; j < iterations_; ++j)
        {
            filtered_[0] = filtered_[0];
            for(unsigned i = 1; i < count_; ++i)
            {
                filtered_[i] = filtered_[i] + coefficent_ * filtered_[i-1];
            }
        }
    }
    
    void Normalize(float targetDB)
    {
        float maxSample = 0.f;
        for(unsigned i = 0; i < count_; ++i)
        {
            float abs = fabs(filtered_[i]);
            maxSample = (abs > maxSample) ? abs : maxSample;
        }

        // M2 = 32767 * 10^(-1.5/20)
        unsigned maxInt = 32767;
        float M2 = pow(10, targetDB / 20) * maxInt;
        float normalFactor = M2 / maxSample;

        for(unsigned i = 0; i < count_; ++i)
        {
            filtered_[i] = filtered_[i] * normalFactor;
        }
    }
 
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