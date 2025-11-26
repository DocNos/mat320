#include "pluck.h"

void Pluck::Execute()
{
    int offset = 0;
    timePoint start = NowTime();
    for(unsigned i = 0; i < duration_; ++i)
    {
        cout << "Processing note " << (i+1) << "/8 ("
            << semitones_[i] << " Hz) >>>" << endl;
        FilterParams currParams = 
            calculateParameters(semitones_[i]);
        Reset(currParams);
        genRandom();
        Delay(currParams, offset);
        offset += header_.sampleRate;
    }
    timePoint end = NowTime();
    double seconds = Duration(start, end).count() / 1000000.0;
    cout << "Execution for " << frequencyBase_ << " complete. Took "
        << seconds << " seconds" << endl;
    string filename = "output/pluck_" + to_string((int)frequencyBase_) + ".wav";
    WriteWav(filename);
}

void Pluck::Delay(FilterParams currParams, int& offset)
{
    float decayMult = pow(STD_R, currParams.stepLen);
    for(unsigned i = 0; i < header_.sampleRate; ++i)
    {
        float delayed = delayQueue_.front();
        delayQueue_.pop();
        float delayOut = (delayed * decayMult) + input_[i];
        float lowpassOut = Lowpass(delayOut);
        float allpassOut = Allpass(currParams, lowpassOut);
        output_[offset + i] = 
            static_cast<int16_t>(allpassOut);
        // Feedback - "string vibration"
        delayQueue_.push(allpassOut);
    }    
}

float Pluck::Lowpass(float currSample)
{
    float processed = LOWPASS_COEFF 
        * (currSample + lp_Prev_);
    lp_Prev_ = currSample;
    return processed;
}

float Pluck::Allpass(FilterParams currParams, float currSample)
{
    // y[n] = -a * y[n-1] + x[n-1] + a * x[n]
    float a = currParams.ap_Coeff;
    float output = (-a * ap_PrevOutput_) + ap_PrevInput_ + (a * currSample);

    // Update state for next call
    ap_PrevInput_ = currSample;
    ap_PrevOutput_ = output;

    return output;
}

void Pluck::WriteWav(string filename)
{
    fstream out(filename ,ios_base::binary|ios_base::out);
    if(!out) 
    {
        cerr << "Error could not open " << filename << endl;
        return;
    }
    out.write(reinterpret_cast<const char*>(&header_), sizeof(WavHeader));
    auto outputSize = output_.size() * sizeof(int16_t);
    out.write(reinterpret_cast<const char*>(output_.data())
            , outputSize);
    out.close();
} 

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cerr << "no frequency found" << endl;
        return 1;
    }
    Pluck newPluck(8, stof(argv[1]));
    newPluck.Execute();
    return 0;
}