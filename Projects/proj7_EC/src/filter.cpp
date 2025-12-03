#include "filter.h"

void ResonFilter::Execute()
{


}

double ResonFilter::ExecuteSingle(int index)
{
    double feedForward = 
        params_.gain * output_[index] + params_.coeff_a1;
    double feedBack = 
        output_[ (index - 1)% 0 ] - params_.coeff_a2 
        * output_[ (index - 2)% 0];
    return feedForward * feedBack;
}

void ResonFilter::Reset()
{

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

