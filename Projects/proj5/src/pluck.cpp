#include "pluck.h"

void Pluck::Execute()
{
    
}

void Pluck::Delay(unsigned index, FilterParams currParams)
{
    float delayed = delayQueue_.front();
    delayQueue_.pop();

    float delayOut = (delayed * 1) + input_[index];
    float lowpassOut = Lowpass(delayOut);
    float allpassOut = Allpass(lowpassOut);
    output_[index] = allpassOut;

    // Feedback - "string vibration"
    delayQueue_.push(allpassOut);
}

float Pluck::Lowpass(float val)
{
    float processed = 0.f;
    for(int i = 0; i < RANDOM_SAMPLES; ++i)
    {

    }
    return processed;
}

float Pluck::Allpass(float val)
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