#include <string>
#include <iostream>
#include <vector>
#include <cmath>
using string = std::string;
using std::ostream;
using std::cout;
using std::endl;
using std::vector;

class Lowpass
{
public:
    // wavData should be float?
    char* wavData_; 
    short* filtered_;
    char header_[44];   
    float coefficent_;
    int iterations_;

    unsigned size_;
    unsigned rate_;
    unsigned count_;


public:
    Lowpass(float _coefficent, int _N, char* wav)
    : coefficent_(_coefficent), iterations_(_N)
    {
        ReadWav(wav);
        // filtered should be length of samples
        filtered_ = new short[count_];
    }
    ~Lowpass()
    {
        delete[] wavData_;
        delete[] filtered_;
    }
    // Friend function for << operator overload
    friend ostream& operator<<(ostream& os, const Lowpass& lp)
    {
        vector<short> inData(lp.size_);
        for(unsigned i = 0; i < lp.size_; ++i)
        {
            inData.push_back(static_cast<short>(lp.wavData_[i]));            
        }   
        vector<short> outData(lp.size_);     
        for(unsigned i = 0; i < lp.size_; ++i)
        {
            short db = 20 * log10(lp.filtered_[i] / pow(2, 15));
            outData.push_back(db);
        }
        os << "Input" << "    " << "Output" << endl;
        for (auto it_i = inData.begin();
        it_i != inData.end();)
        {
            for (auto it_o = outData.begin(); 
            it_o != outData.end(); ++it_i, ++it_o )
            {                
                os << *it_i << "    " << *it_o
                << endl;
            }
            return os;
        }
        return os;
    }
    float toDb(float p1, float p2)
    {
        return 20.f * log10(p2/p1);
    }
    float fromDb(float db, float p1)
    {
        return pow(10, db / 20) * p1;
    }
    // y[t] = x[t] + a1 * x[t-1]
    void FilterEq();
    void Normalize(float);

    void ReadWav(string _file);
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
