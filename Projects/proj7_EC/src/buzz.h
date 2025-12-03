#ifndef BUZZ_H
#define BUZZ_H

#include <vector>
#include <cmath>
#include <map>
using namespace std;
/**
 * BuzzGenerator - Generates a digital buzz signal (periodic impulse train)
 *
 * The digital buzz is a harmonic-rich signal created by summing cosines
 * or by generating an impulse train. It serves as the source signal for
 * spectrum shaping with the reson filter.
 */
struct BuzzParams {
    double buzzFreq;
    int numHarmonics;  
};

const map<string, BuzzParams> buzzPresets = 
{
    {"minDull", {110, 5}}, {"midDull", {110, 10}}
    , {"maxDull", {110, 50}}, {"full110", {110, 200}}
    , {"full220", {220, 100}}, {"full50", {50, 441}}
};

class Buzz {
public:
    BuzzParams params_;
    double sampleRate_;  
    double numSamples_;
    double period_; 

    vector<double> output_;


public:
    Buzz(string _preset, double _sampleRate, double _numSamples)
    : params_(buzzPresets.at(_preset))
    , numSamples_(_numSamples)
    , sampleRate_(_sampleRate)
    , period_(sampleRate_ / params_.buzzFreq)
    , output_(numSamples_)
    {}

    void generateImpulse();

    // Period determined by buzz frequency
    // buzz[n] = (1/K) * Σ cos(2π * k * f_buzz * n / fs) for k=1 to K
    void generateCosines();
};

#endif // BUZZ_GENERATOR_H

/*
50 Hz	882 samples	    Very low, like a bass rumble
110 Hz	401 samples	    Low A, common for vocal formants
220 Hz	200.5 samples	A3, male voice fundamental
440 Hz	100.2 samples	A4, female voice fundamental
*/

/*
This determines how rich/bright vs dull/simple 
the source signal is:
# Harmonics	Calculation	Effect on Sound
5-10	    Manually limited	
    Very dull, almost sine-wave-like. 
    Few harmonics for filter to pick from
20-50	    Manually limited	
    Darker, warmer buzz. Limited harmonic content
100-200	    floor(44100/(2*110)) = 200 for 110 Hz	
    Full, rich buzz. Many harmonics for filter to emphasize
Max safe	floor(SR / (2*freq))	
    Brightest possible without aliasing
*/