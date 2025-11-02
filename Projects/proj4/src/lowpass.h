#include <string>
using string = std::string;
class Lowpass
{
public:
    char* wavData_; 
    short* filtered_;
    char header_[44];   
    float coefficent_;
    int N_;

    unsigned size_;
    unsigned rate_;
    unsigned count_;


public:
    Lowpass(float _coefficent, int _N, char* wav)
    : coefficent_(_coefficent), N_(_N)
    {
        ReadWav(wav);
        filtered_ = new short[N_];
    }
    ~Lowpass()
    {
        delete[] wavData_;
        delete[] filtered_;
    }

    // y[t] = x[t] + a1 * x[t-1]
    short* FilterEq();
    short Normalize(short rawInput);
    
    short* ConvertWave();
    char* ReadWav(string _file);
    void WriteOut();
};


/*
    Filter coefficent a1
    an integer number n of times to run the filter
    and a wave file:
    ./lowpass < a1 > < n > < inputwav >
*/
/*
WAV File Structure:
Bytes 0-43:   WAV header (metadata)
Bytes 24-27:  Sample rate (unsigned int)
Bytes 40-43:  Data size in bytes (unsigned int)
Bytes 44+:    Actual audio samples
*/
