#include "pluck.h"

void Pluck::Execute()
{
    int offset = 0;
    for(int i = 0; i < duration_; ++i)
    {
        FilterParams currParams = 
            calculateParameters(semitones_[i]);
        Reset(currParams);
        genRandom();
        Delay(currParams, offset);
        offset += header_.sampleRate;
    }
}

void Pluck::Delay(FilterParams currParams, int& offset)
{
    float decayMult = pow(STD_R, currParams.stepLen);
    for(int i = 0; i < header_.sampleRate; ++i)
    {
        float delayed = delayQueue_.front();
        delayQueue_.pop();
        float delayOut = (delayed * decayMult) + input_[i];
        float lowpassOut = Lowpass(delayOut);
        float allpassOut = Allpass(lowpassOut);
        output_[offset + i] = 
            static_cast<int16_t>(allpassOut);
        // Feedback - "string vibration"
        delayQueue_.push(allpassOut);
    }    
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
    float processed = 0.f;

    return processed;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cerr << "no frequency found" << endl;
        return 1;
    }
    Pluck newPluck(8, stof(argv[1]));

    return 0;
}