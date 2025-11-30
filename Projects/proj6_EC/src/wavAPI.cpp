#include "wavAPI.h"

Pluck::Pluck(WavHeader _header, FilterParams _params)
    : delayQueue_(queue<float>()), semitones_(vector<float>(_params.duration))
    , duration_(_params.duration), frequencyBase_(_params.baseFrequency), lp_Prev_(0.f)
    , ap_PrevInput_(0.f), ap_PrevOutput_(0.f)
{
    header_ = createDefault(duration_, 1, SAMPLE_RATE, BITS_PER_SAMPLE);
    int steps[8] = {0, 2, 4, 5, 7, 9, 11, 12};
    for(unsigned i = 0; i < duration_; ++i)
    {           
        semitones_[i] = frequencyBase_ *
            pow(2.f,(steps[i])/ 12.f);                             
    }
    input_ = vector<float>(duration_ * header_.sampleRate, 0.f);
    output_ = vector<int16_t>(duration_ * header_.sampleRate);   
}

float Pluck::Lowpass(float currSample)
{
    float processed = LOWPASS_COEFF 
        * (currSample + lp_Prev_);
    lp_Prev_ = currSample;
    return processed;
}

float Pluck::Allpass(FilterParams currParams, float currSample)
{
    // y[n] = -a * y[n-1] + x[n-1] + a * x[n]
    float a = currParams.ap_Coeff;
    float output = (-a * ap_PrevOutput_) + ap_PrevInput_ + (a * currSample);

    // Update state for next call
    ap_PrevInput_ = currSample;
    ap_PrevOutput_ = output;

    return output;
}

WavHeader Pluck::createDefault(unsigned audioDuration, uint16_t numChannels
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

FilterParams Pluck::calculateParameters(float frequency)
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

void Pluck::genRandom()
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

void Pluck::Reset(FilterParams curr)
{
    while(!delayQueue_.empty()) delayQueue_.pop();
    for( int i = 0; i < curr.stepLen; ++i)
        delayQueue_.push(0);
    fill(input_.begin(), input_.end(), 0.f);
    lp_Prev_ = 0.f;
    ap_PrevInput_ = 0.f;
    ap_PrevOutput_ = 0.f;
}






