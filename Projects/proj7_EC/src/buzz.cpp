#include "buzz.h"

void Buzz::generateImpulse()
{
    int period = static_cast<int>(round(period_));
    for(int n = 0; n < numSamples_; n+=period)
    {
        output_[n] = 1.0 / period; // normalize
    }
}

void Buzz::generateCosines()
{
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
        output_[n] = sum;
    }
}