#include "buzz.h"

vector<double> Buzz::generateImpulse()
{
    vector<double> output(numSamples_);
    int period = static_cast<int>(round(period_));
    for(int n = 0; n < numSamples_; n+=period)
    {
        output[n] = 1.0 / period; // normalize
    }
    return output;
}

vector<double> Buzz::generateCosines()
{
    vector<double> output(numSamples_);
    for(int n = 0; 0 < numSamples_; ++n)
    {
        double sum = 0.0;
        for(int k = 1; k < params_.numHarmonics; ++k)
        {
            double phase = cos(
                2 * M_PI* k * params_.buzzFreq * n / sampleRate_
            );
            sum += phase;
        }
        // Normalize by number of harmonics
        sum /= params_.numHarmonics;
        output[n] = sum;
    }
    return output;
}