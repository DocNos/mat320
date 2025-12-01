#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include "BuzzGenerator.h"
#include "ResonFilter.h"
#include "wav.h"

using namespace std;

int main() {
    // User input variables
    double centerFreq;
    double bandwidth;
    double buzzFreq;
    double duration;
    string outputFilename;

    // Get user input
    cout << "=== Spectrum Shaping: Reson Filter + Digital Buzz ===" << endl;
    cout << endl;

    cout << "Enter center frequency (Hz): ";
    cin >> centerFreq;

    cout << "Enter bandwidth (Hz): ";
    cin >> bandwidth;

    cout << "Enter buzz frequency (Hz): ";
    cin >> buzzFreq;

    cout << "Enter duration (seconds): ";
    cin >> duration;

    cout << "Enter output filename (e.g., output.wav): ";
    cin >> outputFilename;

    // Add output directory prefix if not present
    if (outputFilename.find('/') == string::npos) {
        outputFilename = "output/" + outputFilename;
    }

    cout << endl;
    cout << "Generating audio with:" << endl;
    cout << "  Center frequency: " << centerFreq << " Hz" << endl;
    cout << "  Bandwidth: " << bandwidth << " Hz" << endl;
    cout << "  Buzz frequency: " << buzzFreq << " Hz" << endl;
    cout << "  Duration: " << duration << " seconds" << endl;
    cout << "  Output: " << outputFilename << endl;
    cout << endl;

    // Constants
    const double SAMPLE_RATE = 44100.0;
    const int NUM_SAMPLES = static_cast<int>(duration * SAMPLE_RATE);

    // TODO: Create BuzzGenerator instance

    // TODO: Create ResonFilter instance

    // TODO: Generate buzz signal

    // TODO: Filter the buzz signal through reson filter

    // TODO: Normalize output to prevent clipping

    // TODO: Convert float samples to int16_t

    // TODO: Create WAV header using wav.h structs

    // TODO: Write WAV file using WriteWav function from wav.h

    cout << "Audio generation complete!" << endl;

    return 0;
}