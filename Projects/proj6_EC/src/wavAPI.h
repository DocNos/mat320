#pragma once
#include "wav.h"

// Timer -----------------------------------------------
#include <chrono>
using namespace std;
using timePoint = chrono::time_point<chrono::high_resolution_clock>;;
inline timePoint NowTime()
    { return chrono::high_resolution_clock::now();}
inline chrono::microseconds Duration(timePoint start, timePoint end)
    { return chrono::duration_cast<chrono::microseconds>(end - start); }

// -----------------------------------------------------



class Pluck
{
public:
    WavHeader header_;
    FilterPreset preset_;
    queue<float> delayQueue_;
    vector<float> semitones_;
    vector<float> input_;
    vector<int16_t> output_;    

    unsigned duration_;
    unsigned numSamples_;
    float frequencyBase_;
    float lp_Prev_;
    float ap_PrevInput_;
    float ap_PrevOutput_;

public:
    Pluck(FilterPreset);
    
    void genRandom();
    void Reset
    (FilterParams);
    vector<int16_t> Execute();
    void Delay(FilterParams, int&, unsigned);
    // y[n] = 0.5 * (x[n] + x[n-1])
    float Lowpass(float);
    // y[n] = -a * y[n-1] + x[n-1] + a * x[n]
    float Allpass(FilterParams, float);
};

