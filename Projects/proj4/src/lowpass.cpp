#include "lowpass.h"
// #include <algorithm>
#include <fstream>
#include <complex>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <climits>
using namespace std;

void Lowpass::FilterEq()
{
    short* input = reinterpret_cast<short*>(wavData_);
    for(int j = 0; j < iterations_; ++j)
    {
        filtered_[0] = input[0];
        for(int i = 1; i < count_; ++i)
        {
            filtered_[i] = static_cast<short>(
                input[i] + coefficent_ * input[i-1]);
        }
    }
    
}

// Normalize Hz value to -1.5dB
// 
void Lowpass::Normalize(float targetDB)
{
    float maxSample = 0.f;
    for(unsigned i = 0; i < count_; ++i)
    {
        auto abs = fabs(static_cast<float>(filtered_[i]));
        maxSample = (abs > maxSample) ? abs : maxSample; 
    }
    unsigned maxInt = pow(2, 15);
    // db = 20 * log10(hz value / sampleRef)
    // 10^(db/20) * sampleRef = hz value
    float maxHz = pow(10, targetDB / 20) * maxInt;
    float normal = maxHz / maxSample;
    for(unsigned i = 0; i < count_; ++i)
    {
        filtered_[i] = static_cast<short>(
            filtered_[i] * normal
        );
    }
}

void Lowpass::ConvertWave()
{
    // convert to WAVE file
    float const norm = 2.0f/float(iterations_),
              MAX = float((1<<15)-1);
    for (unsigned i=0; i < count_; ++i) 
    {      
        //filtered_[i] = Normalize(samples[i]);
    }
}

void Lowpass::ReadWav(string wavFile)
{
    fstream in(wavFile, ios_base::binary|ios_base::in);
    if (!in)
        return;

    // Header metadata - will write seperately.
    in.read(header_,44);

    size_ = *reinterpret_cast<unsigned*>(header_+40);
    rate_ = *reinterpret_cast<unsigned*>(header_+24);
    // 2 byte samples
    count_ = size_/2;
    wavData_ = new char[size_];
    in.read(wavData_,size_);

    cout << "The sample rate is:  " << rate_ << endl;
    cout << "The data size is:  " << size_ << endl;
}

void Lowpass::WriteOut()
{
    fstream out("output.wav",ios_base::binary|ios_base::out);
    out.write(header_, 44);
    out.write(wavData_, size_);
}

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        cout << "Insufficent arguements" << endl;
        return 0;
    }
    
    float coeff = atof(argv[1]);
    int iterations = atoi(argv[2]);
    char* wav = argv[3];

    if(abs(coeff) >= 1) 
    {
        cout << "Coefficent should be less than 1" << endl;
        return 0;
    }
    if(iterations <= 0)
    {
        cout << "Not enough iterations" << endl;
        return 0;
    }
    Lowpass lowpass = Lowpass(coeff, iterations, wav);
    lowpass.FilterEq();
    lowpass.Normalize(-1.5);
    
    return 1;
}