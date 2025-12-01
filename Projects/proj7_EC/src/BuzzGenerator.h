#ifndef BUZZ_GENERATOR_H
#define BUZZ_GENERATOR_H

#include <vector>
#include <cmath>

/**
 * BuzzGenerator - Generates a digital buzz signal (periodic impulse train)
 *
 * The digital buzz is a harmonic-rich signal created by summing cosines
 * or by generating an impulse train. It serves as the source signal for
 * spectrum shaping with the reson filter.
 */
class BuzzGenerator {
private:
    double sampleRate;
    double frequency;
    int numHarmonics;
    double period;

public:
    /**
     * Constructor
     * @param freq The fundamental frequency of the buzz signal (Hz)
     * @param sr The sample rate (Hz), typically 44100
     */
    BuzzGenerator(double freq, double sr);

    /**
     * Generate buzz signal samples
     * @param numSamples Number of samples to generate
     * @return Vector of buzz signal samples in range [-1.0, 1.0]
     */
    std::vector<double> generate(int numSamples);

    /**
     * Generate a single buzz sample at time n
     * @param n Sample index
     * @return Buzz sample value
     */
    double generateSample(int n);

    /**
     * Set the buzz frequency
     * @param freq New frequency in Hz
     */
    void setFrequency(double freq);

    /**
     * Get the current buzz frequency
     * @return Frequency in Hz
     */
    double getFrequency() const;
};

#endif // BUZZ_GENERATOR_H