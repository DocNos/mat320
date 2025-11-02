#include "lowpass.h"
// #include <algorithm>
#include <fstream>
#include <complex>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

short* Lowpass::FilterEq()
{
    short* input = reinterpret_cast<short*>(wavData_);
    filtered_[0] = input[0];
    for(int i = 1; i < N_; ++i)
    {
        filtered_[i] = input[i] + 
            static_cast<short>(coefficent_ * input[i-1]);
    }
    return filtered_;
}

// Normalize Hz value to -1.5dB
// 
short Lowpass::Normalize(short rawInput)
{
    


}

short* Lowpass::ConvertWave()
{
    // convert to WAVE file
    float const norm = 2.0f/float(N_),
              MAX = float((1<<15)-1);
    short* samples = FilterEq();
    //short *samples = reinterpret_cast<short*>(wavData_);
    for (unsigned i=0; i < count_; ++i) 
    {      
        samples[i] = Normalize(samples[i]);
    }
}

char* Lowpass::ReadWav(string wavFile)
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
    int N = atoi(argv[2]);
    char* wav = argv[3];

    if(abs(coeff) >= 1) 
    {
        cout << "Coefficent should be less than 1" << endl;
        return 0;
    }
    if(N <= 0)
    {
        cout << "Not enough iterations" << endl;
        return 0;
    }
    Lowpass lowpass = Lowpass(coeff, N, wav);
    lowpass.FilterEq();
    
    return 1;
}