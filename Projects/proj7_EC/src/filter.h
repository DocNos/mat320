#ifndef FILTER_H
#define FILTER_H
#define _USE_MATH_DEFINES

#include <map>
#include <tuple>
#include <cmath>
#include "wav.h"
#include "buzz.h"
using namespace std;

enum Scale
{
    Major, Minor5, Minor5_Arpeggio
};


// Map notes to names?
const map<string, double> 
NoteFrequencies = {
    {"middleC", 261.63}, {"c3", (261.63/2)}, {"middleD", 293.66}
    , {"eFlat", 311.13}, {"middleE", 329.63}, {"middleF", 349.23}
    , {"fSharp", 369.99}, {"gFlat", 369.99}, {"middleG", 392}
    , {"a4", 440}, {"bFlat", 466.16}
};

const map<string, float> 
r_vals = {
    {"percussion", 0.990f}, {"rhythm", 0.995f}
    , {"lead", 0.99985f}
};


struct FilterParams
{
    //--------------- Timing
    unsigned sampleRate = 44100;
    unsigned duration = 8;
    unsigned numSamples = sampleRate / duration;
    double r_val = r_vals.at("rhythm");
    unsigned numNotes = duration;
    float noteDuration = 1;

    //--------------- Pitch
    double frequency = NoteFrequencies.at("middleC");
    Scale scale = Scale::Major;

    //--------------- Data Organization
    unsigned bitsPerSample = 16;
    unsigned numRand_Samples = 10;
    unsigned numChannels = 1;
    int clampRange = 15000; 

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
    double centerFrequency; // Which frequency to emphasize
    double R;           // Pole radius
    double theta;       // Pole angle
    double coeff_a1;    // 2*R*cos(theta)
    double coeff_a2;    // R*R
    double gain;        // 1 - R*R
    double bandwidth;
};

inline WavHeader createHeader(Filter filter)
{
    FilterParams params = filter.baseParams_;
    unsigned audioDuration = params.duration;
    unsigned sampleRate = params.sampleRate;
    unsigned bitsSample = params.bitsPerSample;

    unsigned headerSize = sizeof(WavHeader) 
        - (sizeof(WavHeader::chunkID) + sizeof(WavHeader::chunkSize));
    unsigned numSamples = audioDuration * sampleRate;
    unsigned byteRate = sampleRate * params.numChannels * bitsSample/8;
    uint16_t blockAlignment = params.numChannels * bitsSample/8;
    unsigned totalSize = numSamples * params.numChannels * bitsSample/8;
    WavHeader header = 
    {
        {'R', 'I', 'F', 'F'}   // Chunk ID
        , headerSize + totalSize    
        , {'W', 'A', 'V', 'E'} // File Format
        , {'f', 'm', 't', ' '} // sub-chunk ID
        , 16                   // sub-chunk size
        , 1                    // Audio Format (??)
        , params.numChannels          // Mono: 1 Channel
        , sampleRate           // Sample rate
        , byteRate          
        , blockAlignment                 
        , bitsSample       
        , {'d', 'a', 't', 'a'} // this data is made out of data
        , totalSize
    };
    return header;
}

class Filter
{
public:
    Filter(FilterParams _params)
    : baseParams_(_params)
    {
        header_ = createHeader(this);
    }
    FilterParams baseParams_;
    WavHeader header_;
    

};


struct ResonPreset 
{
    double centerFreq;
    double bandwidth;
};

const map<string, ResonPreset> ResonPresets = 
{
    {"vowel_a", {730, 50}},      // Formant for "ah" sound
    {"vowel_e", {530, 50}},      // Formant for "eh" sound
    {"vowel_i", {270, 40}},      // Formant for "ee" sound
    {"vowel_o", {570, 50}},      // Formant for "oh" sound
    {"vowel_u", {440, 40}},      // Formant for "oo" sound
    {"narrow_440", {440, 20}},   // Very narrow at A4
    {"wide_1000", {1000, 200}},  // Wide band at 1kHz
};
/**
 * ResonFilter - Second-order IIR bandpass filter (reson filter)
 *
 * Transfer Function: H(z) = (1 - R²) / (1 - 2R*cos(θ)*z^(-1) + R²*z^(-2))
 * Difference Equation: y[n] = gain * x[n] + a1 * y[n-1] - a2 * y[n-2]
 */
class ResonFilter : public Filter
{
public:
    ResonParams params_;
    Buzz buzz_;
    vector<double> output_;    
    double y_n1;        // y[n-1] - previous output
    double y_n2;        // y[n-2] - output two samples ago

public:
    ResonFilter(FilterParams _params)
    : Filter(_params)
    , buzz_("full110"
        , baseParams_.sampleRate, baseParams_.numSamples)
    {
        calcParameters("vowel_a");
        buzz_.generateCosines();
        output_ = buzz_.output_;
    }
// ----------------------- Processing
/*
 For each sample n from 0 to total_samples:
   a. Generate buzz sample x[n]
   b. Filter: y[n] = reson.process(x[n])
   c. Store y[n] in output buffer
 Normalize output to prevent clipping
*/
    //y[n] = gain * x[n] + a1 * y[n-1] - a2 * y[n-2]
    void Execute();
    double ExecuteSingle(int index);

    void Reset();



// ----------------------- Parameter Setup
    void calcParameters(string);
    // θ = 2π * fc / fs
    double calcTheta(){
        return 2*M_PI 
        *(baseParams_.frequency / baseParams_.sampleRate);
    }

    // R = exp(-π * BW / fs) 
    double calcRadius(){
        return exp(-M_PI * 
            (params_.bandwidth / baseParams_.sampleRate));
    }

    // a1 = 2 * R * cos(θ)
    // a2 = R²
    tuple<double,double> calcCoefficents(){
        double a1 = 2 * params_.R 
            * cos(static_cast<float>(params_.theta));
        double a2 = pow(params_.R, 2);
        return tuple<double, double>(a1, a2);    
    }

    // gain = 1 - R²
    double calcGain(){
        return 1 - pow(params_.R, 2);
    }
};




inline vector<float> CreateSemitones
(FilterParams params, Scale scale)
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
                semitones[i] = params.frequency *
                    pow(2.f,(steps[i]% 8)/ 12.f);                             
            }            
        } break;
        case(Minor5):
        {
            int steps[8] = {0, 3, 5, 6, 7, 10, 12, 15};
            for(unsigned i = 0; i < dur; ++i)
            {
                semitones[i] = params.frequency *
                    pow(2.f,(steps[i % 8])/ 12.f);
            }  
        }break;
        case(Minor5_Arpeggio):
        {
            int steps[8] = {0, 5, 3, 10, 6, 7, 12, 15};
            for(unsigned i = 0; i < dur; ++i)
            {
                semitones[i] = params.frequency *
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
    PluckParams params =
    {
        steps, delayLen
        , delayStep
        , delayDelta
        , (1.f - delayDelta) / (1.f + delayDelta)
        , lowpass_coeff
    };
    return params;
}

#endif

/*
{
    double middleC = 261.63;
    double c3 = middleC / 2;
    double middleD = 293.66;
    double eFlat = 311.13;
    double middleE = 329.63;
    double middleF = 349.23;
    double fSharp = 369.99;
    double gFlat = fSharp;
    double middleG = 392;
    double a4 = 440;
    double bFlat = 466.16; 
};*/


/*
int clampRange = 15000; 
    
    unsigned sampleRate = 44100;    
    uint16_t numChannels = 1;
    uint16_t bitsSample = 16; 

    float percussionR = 0.990f;
    float rhythmR = 0.995f;
    float leadR = 0.99985f;
*/


