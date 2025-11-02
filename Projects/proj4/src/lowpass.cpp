#include "lowpass.h"
// #include <algorithm>
#include <fstream>
#include <complex>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

void Lowpass::ConvertWave()
{
    // convert to WAVE file
    float const norm = 2.0f/float(N_),
              MAX = float((1<<15)-1);
    short *samples = reinterpret_cast<short*>(wavData_);
    for (unsigned i=0; i < count_; ++i) 
    {
      // int k = 0.5f*(2*i - N_)/H,
      //     kp1 = k + 1;
      // if (k < 0)
      //   k = kp1 = 0;
      // if (kp1 >= int(wcount))
      //   k = kp1 = int(wcount) - 1;
      // float x = (i - 0.5f*N)/H - k,
      //       y = norm*(coef[k] + (coef[kp1] - coef[k])*x);
      samples[i] = short(min(MAX,abs(y)));
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
    Lowpass lowpass(coeff, N, wav);

    return 1;
}