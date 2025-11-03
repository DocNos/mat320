#include "lowpass.h"
// #include <algorithm>



void Lowpass::FilterEq()
{
    // Use normalized input (±1.5 range)
    float* input = normalizedInput_.data();

    // First iteration: input -> filtered
    filtered_[0] = input[0];
    for(unsigned i = 1; i < count_; ++i)
    {
        filtered_[i] = input[i] + coefficent_ * input[i-1];
    }

    // Subsequent iterations: filtered -> filtered
    for(int j = 1; j < iterations_; ++j)
    {
        filtered_[0] = filtered_[0];
        for(unsigned i = 1; i < count_; ++i)
        {
            filtered_[i] = filtered_[i] + coefficent_ * filtered_[i-1];
        }
    }
}

// Normalize filtered output to -1.5dB
void Lowpass::Normalize(float targetDB)
{
    float maxSample = 0.f;
    for(unsigned i = 0; i < count_; ++i)
    {
        float abs = fabs(filtered_[i]);
        maxSample = (abs > maxSample) ? abs : maxSample;
    }

    // M2 = 32767 * 10^(-1.5/20)
    unsigned maxInt = 32767;
    float M2 = pow(10, targetDB / 20) * maxInt;
    float normalFactor = M2 / maxSample;

    for(unsigned i = 0; i < count_; ++i)
    {
        filtered_[i] = filtered_[i] * normalFactor;
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

    short* samples = reinterpret_cast<short*>(wavData_);
    for(unsigned i = 0; i < count_; ++i)
    {
        normalizedInput_.push_back(static_cast<float>(samples[i]));
    }

    cout << "The sample rate is:  " << rate_ << endl;
    cout << "The data size is:  " << size_ << endl;
}

void Lowpass::WriteOut()
{
    short* samples = reinterpret_cast<short*>(wavData_);
    for(unsigned i = 0; i < count_; ++i)
    {
        // Convert float to short with clamping
        float val = filtered_[i];
        if (val > 32767.f) val = 32767.f;
        if (val < -32768.f) val = -32768.f;
        samples[i] = static_cast<short>(val);
    }

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
    //lowpass.Normalize(-1.5);
    // cout << lowpass;
    lowpass.WriteOut();

    return 0;
}