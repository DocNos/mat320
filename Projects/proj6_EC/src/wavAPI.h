#pragma once
#include "wav.h"

// Timer -----------------------------------------------
#include <chrono>
using namespace std;
using timePoint = chrono::time_point<chrono::high_resolution_clock>;;
timePoint NowTime() 
    { return chrono::high_resolution_clock::now();}
chrono::microseconds Duration(timePoint start, timePoint end) 
    { return chrono::duration_cast<chrono::microseconds>(end - start); }

// -----------------------------------------------------

inline vector<float>* CreateSemitones(FilterParams params, int numSteps)
{
    unsigned dur = params.duration;
    vector<float> semitones = vector<float>(dur);
    switch(numSteps)
    {
        case(8): 
        {
            int steps[8] = {0, 2, 4, 5, 7, 9, 11, 12};
            for(unsigned i = 0; i < dur; ++i)
            {           
                semitones[i] = params.baseFrequency *
                    pow(2.f,(steps[i])/ 12.f);                             
            }            
        } break;
        default: break;

    }
    return &semitones;
    
}


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

public:
    Pluck
    (WavHeader, FilterParams);
    
    WavHeader createDefault
    (unsigned, uint16_t, unsigned, uint16_t);
    
    void genRandom();

    void Reset
    (FilterParams);

    void Execute();
    void Delay(FilterParams, int&);
    // y[n] = 0.5 * (x[n] + x[n-1])
    float Lowpass(float);
    // y[n] = -a * y[n-1] + x[n-1] + a * x[n]
    float Allpass(FilterParams, float);
};

