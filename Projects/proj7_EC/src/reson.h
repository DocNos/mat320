#ifndef RESON_H
#define RESON_H

#include <vector>
#include <cmath>

/**
 * ResonFilter - Second-order IIR bandpass filter (reson filter)
 *
 * Transfer Function: H(z) = (1 - R²) / (1 - 2R*cos(θ)*z^(-1) + R²*z^(-2))
 * Difference Equation: y[n] = gain * x[n] + a1 * y[n-1] - a2 * y[n-2]
 *
 * Where:
 *   θ = 2π * fc / fs  (fc = center frequency, fs = sample rate)
 *   R = exp(-π * BW / fs)  (BW = bandwidth in Hz)
 *   a1 = 2 * R * cos(θ)
 *   a2 = R²
 *   gain = 1 - R²
 */
class ResonFilter {
private:
    double R;           // Pole radius
    double theta;       // Pole angle
    double coeff_a1;    // 2*R*cos(theta)
    double coeff_a2;    // R*R
    double gain;        // 1 - R*R
    double y_n1;        // y[n-1] - previous output
    double y_n2;        // y[n-2] - output two samples ago

    double sampleRate;
    double centerFreq;
    double bandwidth;

public:
    /**
     * Constructor
     * @param fc Center frequency in Hz
     * @param bw Bandwidth in Hz
     * @param sr Sample rate in Hz (typically 44100)
     */
    ResonFilter(double fc, double bw, double sr);

    /**
     * Process a single input sample through the filter
     * @param input Input sample value
     * @return Filtered output sample
     */
    double process(double input);

    /**
     * Process multiple samples
     * @param input Vector of input samples
     * @return Vector of filtered output samples
     */
    std::vector<double> process(const std::vector<double>& input);

    /**
     * Reset filter state (clear history)
     */
    void reset();

    /**
     * Update filter parameters
     * @param fc New center frequency in Hz
     * @param bw New bandwidth in Hz
     */
    void setParameters(double fc, double bw);

    /**
     * Get the current center frequency
     * @return Center frequency in Hz
     */
    double getCenterFrequency() const;

    /**
     * Get the current bandwidth
     * @return Bandwidth in Hz
     */
    double getBandwidth() const;

    /**
     * Get the quality factor Q = fc / BW
     * @return Quality factor
     */
    double getQualityFactor() const;

private:
    /**
     * Calculate filter coefficients from parameters
     */
    void calculateCoefficients();
};

#endif // RESON_FILTER_H