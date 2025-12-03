#include "filter.h"

void ResonFilter::Execute()
{
    for(int i = 2; i < baseParams_.numSamples; ++i)
    {
        output_[i] = ExecuteSingle(i);
    }

}

void ResonFilter::Write(string filename)
{
    WriteWav(filename, header_, Normalize());
}

double ResonFilter::ExecuteSingle(int index)
{
    double x_n = output_[index];
    double y_n = 
        params_.gain * x_n  
        + (params_.coeff_a1 * y_n1) 
        - (params_.coeff_a2 * y_n2);
    y_n2 = y_n1;
    y_n1 = y_n;
    
    return y_n;
}

vector<int16_t> ResonFilter::Normalize()
{
    double maxVal = 0.0;
    // Normalize relative to other samples - find max
    for(double sample: output_){
        maxVal = max(abs(sample), maxVal);
    }
    vector<int16_t> converted(output_.size());
    for(int i = 0; i < output_.size(); ++i)
    {
        double normalized = (maxVal > 1.0) 
            ? (output_[i] / maxVal) : (output_[i]);
        converted[i] = static_cast<int16_t>(
            normalized * INT16_MAX);
    }
    return converted;
}


void ResonFilter::calcParameters(string preset)
{
    ResonPreset settings = ResonPresets.at(preset);
    baseParams_.frequency = settings.centerFreq;
    params_.bandwidth = settings.bandwidth;
    
    params_.theta = calcTheta();
    params_.R = calcRadius();
    tuple<double, double> coeff = calcCoefficents();
    params_.coeff_a1 = get<0>(coeff);
    params_.coeff_a2 = get<1>(coeff);
    params_.gain = calcGain();

}

