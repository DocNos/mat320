#ifndef FILTER_H
#define FILTER_H
#define _USE_MATH_DEFINES

#include "buzz.h"
#include "Data.h"

using namespace std;


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

class Filter;





class Filter
{
public:
    Filter()
    : baseParams_()
    {
        header_ = createHeader();
    }

    Filter(FilterParams _params)
    : baseParams_(_params)
    {
        header_ = createHeader();
    }
    FilterParams baseParams_;
    WavHeader header_;

    WavHeader createHeader()
{
    FilterParams params = baseParams_;
    unsigned audioDuration = params.duration;
    unsigned sampleRate = params.sampleRate;
    uint16_t bitsSample = params.bitsPerSample;

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

    ResonFilter(string _buzzPreset, string _resonPreset)
    : Filter()
    , buzz_(_buzzPreset
        , baseParams_.sampleRate, baseParams_.numSamples)
    , y_n1(0.0), y_n2(0.0)
    {
        calcParameters(_resonPreset);
        output_ = buzz_.output_;
    }

    ResonFilter(FilterParams _params
    , string _buzzPreset, string _resonPreset)
    : Filter(_params)
    , buzz_(_buzzPreset
        , baseParams_.sampleRate, baseParams_.numSamples)
    , y_n1(0.0), y_n2(0.0)
    {
        calcParameters(_resonPreset);
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

    void Write(string);

private:
    double ExecuteSingle(int index);

    vector<int16_t> Normalize();

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

inline bool FindReson(string preset)
{
    return (ResonPresets.find(preset) != ResonPresets.end());
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


