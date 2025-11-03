#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <complex>
#include <cstdlib>

#include <iostream>
#include <climits>
using namespace std;
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
    float* filtered_;
    vector<float> normalizedInput_;

    char header_[44];   
    float coefficent_;
    int iterations_;

    unsigned size_;
    unsigned rate_;
    unsigned count_;

    unsigned _MAX16_ = 32767;


public:
    Lowpass(float _coefficent, int _N, char* wav)
    : coefficent_(_coefficent), iterations_(_N)
    {
        ReadWav(wav);
        Normalize_Signal(normalizedInput_, -1.5);
        // filtered should be length of samples
        filtered_ = new float[count_];
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
    float toDb(float p2)
    {
        return 20.f * log10(fabs(p2)/_MAX16_);
    }
    float fromDb(float db, float p1)
    {
        return pow(10, db / 20) * p1;
    }
    // y[t] = x[t] + a1 * x[t-1]

    void FilterEq();
    /*
    Reference = 32767 (max 16-bit): 
        "how far below the maximum possible value is this sample?" 
        dBFS (decibels relative to Full Scale). 
        A sample at maximum would be 0 dBFS, and everything else is negative.
    Reference = maximum sample in the file: 
        "how far below the loudest sample is this sample?" 
        The loudest sample would be 0 dB, everything else negative.
    */
    void Normalize_Signal(vector<float>& input)
    {
        fstream out1("./out/sampleInput.txt", ios::out);
        float max = 0.f;
        for(auto sample : input)
        {
            out1 << sample << endl;
            float abs = fabs(sample);
            max = (abs > max) ? (abs) : max;
        }
        float normalFactor = 1.5 / max;

        fstream out2("./out/normalizedInput.txt", ios::out);
        for(unsigned i = 0; i < count_; ++i)
        {
            input[i] = input[i] * normalFactor;
            out2 << input[i] << endl;
        }
    }
    void Normalize(float);

    void ReadWav(string _file);
    void WriteOut();

    
};


//cout << "Max sample val: " << max << " Normalize Factor: " << normalFactor << endl;
        //fstream out("./out/normalizedInput.txt", ios::out);
        //fstream out2("./out/normalizedInput_db.txt", ios::out);
        // for(auto sample: input)
        // {
        //     out << sample * normalFactor << endl;
        //     out2 << toDb(sample * normalFactor) << endl;
        // }

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
