#include <iostream>
#include <fstream>
#include <queue>
#include <random> 
#include <cmath>
#include <vector>
using namespace std;

struct WavHeader 
{
       char chunkID[4];        // "RIFF"
       uint32_t chunkSize;     // File size - 8
       char format[4];         // "WAVE"
       char fmtLabel[4];       // "fmt "
       uint32_t fmtSize;       // 16 for PCM
       uint16_t audioFormat;   // 1 for PCM
       uint16_t numChannels;   // 1 for mono
       uint32_t sampleRate;    // 44100
       uint16_t bitsPerSample; // 16
       uint32_t byteRate;      // sampleRate * numChannels * bitsPerSample/8
       uint16_t blockAlign;    // numChannels * bitsPerSample/8
       char subchunk2ID[4];    // "data"
       uint32_t dataSize;      // NumSamples * numChannels * bitsPerSample/8
};

class Pluck
{
private:
    WavHeader header_;

public:
    Pluck()
    {
        header_ = createDefault()
    }

    WavHeader createDefault(unsigned fileSize, unsigned numChannels
        , unsigned numSamples, unsigned sampleRate, unsigned bitsSample)
    {
        unsigned byteRate = sampleRate * numChannels * bitsSample/8;
        unsigned blockAlignment = numChannels * bitsSample/8;
        unsigned totalSize = numSamples * numChannels * bitsSample/8;
        WavHeader header = {
            {'R', 'I', 'F', 'F'}   // Chunk ID
            , fileSize             // File size (8)
            , {'W', 'A', 'V', 'E'} // File Format
            , {'f', 'm', 't', ' '} // sub-chunk ID
            , 16                   // sub-chunk size
            , 1                    // Audio Format (??)
            , numChannels          // Mono: 1 Channel
            , sampleRate           // Sample rate
            , bitsSample           // 
            , byteRate             // ??       
            , blockAlignment       // (??) How to fit 
            , {'d', 'a', 't', 'a'} // this data is made out of data
            , totalSize
        };

    }

};

/*
struct fileheader
{
	char riff_label[4]; // (00) = {'R','I','F','F'}
	unsigned riff_size; // (04) = 36 + data_size
	char file_tag[4]; // (08) = {'W','A','V','E'}
	char fmt_label[4]; // (12) = {'f','m','t',' '}
	unsigned fmt_size; // (16) = 16
	unsigned short audio_format; // (20) = 1
	unsigned short channel_count; // (22) = 1 or 2
	unsigned sampling_rate; // (24) = (anything)
	unsigned bytes_per_second; // (28) = (see above)
	unsigned short bytes_per_sample; // (32) = (see above)
	unsigned short bits_per_sample; // (34) = 8 or 16
	char data_label[4]; // (36) = {'d','a','t','a'}
	unsigned data_size; // (40) = # bytes of data
};


*/