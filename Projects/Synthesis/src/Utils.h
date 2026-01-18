#include <vector>
#include <ostream>

#include "Data.h"

using namespace std;

// ------------------------------------ Signal Manipulation

inline vector<int16_t> Mix
(const vector<vector<int16_t>>& voices, unsigned numSamples)
{
    int numVoices = voices.size();
    vector<int16_t> mixed(numSamples);
    
    for(unsigned i = 0; i < numSamples; ++i)
    {
        int32_t sum = 0;
        for(int j = 0; j < numVoices; ++j)
        {
            sum += voices[j][i];
        }
        mixed[i] = static_cast<int16_t>((sum / numVoices));
    }
    return mixed;
}

inline vector<int16_t> BoostAmplitude(vector<int16_t> samples, int16_t boost)
{
    for(size_t i = 0; i < samples.size(); ++i) 
    {
        int32_t boosted = 
        static_cast<int32_t>(samples[i]) * boost;
        samples[i] = 
        static_cast<int16_t>
        (clamp(boosted, -32768, 32767));
    }
    return samples;
}

// ------------------------------------


// ------------------------------------ Wav 
inline void WriteWav(const string& filename
    , const WavHeader& header
    , const vector<int16_t>& samples)
{
    ofstream wavFile(filename, ios::binary);
    if (!wavFile.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing" << endl;
        return;
    }

    // Write the header
    wavFile.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));

    // Write the sample data
    wavFile.write(reinterpret_cast<const char*>(samples.data()), samples.size() * sizeof(int16_t));

    wavFile.close();
    cout << "WAV file written: " << filename << " (" << samples.size() << " samples)" << endl;
}
