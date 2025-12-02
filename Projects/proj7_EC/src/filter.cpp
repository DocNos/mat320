#include "filter.h"

void ResonFilter::calcParameters(string preset)
{
    ResonPreset settings = ResonPresets.at(preset);
    baseParams_.frequency = settings.centerFreq;
    params_.bandwidth_ = settings.bandwidth;
    
    params_.theta = calcTheta();
    params_.R = calcRadius();
    tuple<double, double> coeff = calcCoefficents();
    params_.coeff_a1 = get<0>(coeff);
    params_.coeff_a2 = get<1>(coeff);
    params_.gain = calcGain();

}

ResonFilter CreateReson()
{
    FilterParams baseParams;
}