#include "wavAPI.h"

Pluck::Pluck(FilterPreset _preset)
    : header_(_preset.header), preset_(_preset)
    , delayQueue_(queue<float>())
    , semitones_(CreateSemitones(_preset.params, _preset.scale))
    , duration_(_preset.params.duration)
    , frequencyBase_(_preset.params.baseFrequency)
    , lp_Prev_(0.f), ap_PrevInput_(0.f), ap_PrevOutput_(0.f)
{    
    input_ = vector<float>(duration_ * header_.sampleRate, 0.f);
    output_ = vector<int16_t>(duration_ * header_.sampleRate);   
}

void Pluck::Execute()
{
    int offset = 0;
    //timePoint start = NowTime();
    for(unsigned i = 0; i < duration_; ++i)
    {
        unsigned sampleRate = preset_.sampleRate;
        float freq = semitones_[i];
        auto lp_coeff = preset_.params.lowpass_Coefficent;
        FilterParams currParams = 
            calculateParameters(preset_.scale, sampleRate, freq, lp_coeff);
        Reset(currParams);
        genRandom();
        Delay(currParams, offset);
        offset += header_.sampleRate;
    }
    //timePoint end = NowTime();
    //double seconds = Duration(start, end).count() / 1000000.0;
    //cout << "Execution for " << frequencyBase_ << " complete. Took "
    //    << seconds << " seconds" << endl;
    //string filename = "output/pluck_" + to_string((int)frequencyBase_) + ".wav";
    //WriteWav(filename, header_, output_);
}

void Pluck::Delay(FilterParams currParams, int& offset)
{
    float decayMult = pow(preset_.R_Val, currParams.stepLen);
    for(unsigned i = 0; i < header_.sampleRate; ++i)
    {
        float delayed = delayQueue_.front();
        delayQueue_.pop();
        float delayOut = (delayed * decayMult) + input_[i];
        float lowpassOut = Lowpass(delayOut);
        float allpassOut = Allpass(currParams, lowpassOut);
        output_[offset + i] = 
            static_cast<int16_t>(allpassOut);
        // Feedback - "string vibration"
        delayQueue_.push(allpassOut);
    }    
}

float Pluck::Lowpass(float currSample)
{
    float processed = preset_.params.lowpass_Coefficent 
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

void Pluck::genRandom()
{
    random_device rd;
    mt19937 gen(rd());
    auto CLAMP_RANGE = preset_.clampRange;
    auto RANDOM_SAMPLES = preset_.numRand_Samples;
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






