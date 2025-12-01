#ifndef FILTER_H
#define FILTER_H

//#include "wav.h"

enum Scale
{
    Major, Minor5, Minor5_Arpeggio
};

class Filter
{
public:
    WavHeader header_;
    FilterParams baseParams_;
    

};

class ResonFilter : public Filter
{
public:
    ResonParams params_;

};

struct FilterParams
{
    //--------------- Timing
    unsigned sampleRate;
    unsigned duration;
    double r_val;
    unsigned numNotes;
    float noteDuration;

    // -------------- Pitch
    double frequency;
    Scale scale;

    // -------------- Data Organization
    unsigned bitsPerSample;
    unsigned numRand_Samples;
    unsigned numChannels;
    int clampRange; 

};

struct PluckParams 
{
    unsigned duration;
    float delayLen;     // [D] Exact delay length
    int stepLen;        // [L] Integer delay length, queue step
    float delta;        // Fractional delay
    float ap_Coeff;     // Allpass filter coefficient - delay queue can only hold whole numbers
    // for small phase, reduces to (1 - δ) / (1 + δ)
    float lowpass_Coefficent;
};

struct ResonParams
{
    //double R;           // Pole radius
    double theta;       // Pole angle
    double coeff_a1;    // 2*R*cos(theta)
    double coeff_a2;    // R*R
    double gain;        // 1 - R*R
    double y_n1;        // y[n-1] - previous output
    double y_n2;        // y[n-2] - output two samples ago
    double bandwidth_;
};




inline vector<float> CreateSemitones
(PluckParams params, Scale scale)
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
                    pow(2.f,(steps[i]% 8)/ 12.f);                             
            }            
        } break;
        case(Minor5):
        {
            int steps[8] = {0, 3, 5, 6, 7, 10, 12, 15};
            for(unsigned i = 0; i < dur; ++i)
            {
                semitones[i] = params.baseFrequency *
                    pow(2.f,(steps[i % 8])/ 12.f);
            }  
        }break;
        case(Minor5_Arpeggio):
        {
            int steps[8] = {0, 5, 3, 10, 6, 7, 12, 15};
            for(unsigned i = 0; i < dur; ++i)
            {
                semitones[i] = params.baseFrequency *
                    pow(2.f,(steps[i % 8])/ 12.f);
            }  
        }break;
        default: break;

    }
    return semitones;
    
}

inline PluckParams calculateParameters
(Scale scale, unsigned sampleRate
, float frequency, float lowpass_coeff, unsigned numNotes)
{
    unsigned steps = numNotes;
    //unsigned steps = (scale == Major) ? (8) : (6);
    float delayLen = sampleRate / frequency;
    int delayStep = static_cast<int>(floor(delayLen));
    float delayDelta = delayLen - delayStep;
    DelayParams params =
    {
        frequency, steps, delayLen
        , delayStep
        , delayDelta
        , (1.f - delayDelta) / (1.f + delayDelta)
        , lowpass_coeff
    };
    return params;
}

#endif