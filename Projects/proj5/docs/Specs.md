# Plucked String Filter - Project Specification Document

## Project Overview
Implement a digital plucked string filter that generates a major scale using the Karplus-Strong algorithm with extensions. The program will create a WAV file containing 8 notes (one major scale) with each note lasting 1 second.

## Technical Specifications

### Audio Parameters
- **Sample Rate**: 44,100 Hz (mono)
- **Output Format**: WAV file
- **Duration**: 8 seconds total (1 second per note)
- **Amplitude Range**: -15,000 to 15,000 (16-bit signed integers)

### Filter Parameters
- **Decay Factor (R)**: 0.99985
- **Initial Excitation**: 100 random samples per note
- **Filter Structure**: Three-stage filter system (as per Figure 8.1, page 119)

## Implementation Steps

### Step 1: Set Up Project Structure
1. Create `pluck.cpp` file
2. Include necessary headers:
   - `<iostream>` - for I/O operations
   - `<fstream>` - for file writing
   - `<queue>` - for STL queue
   - `<random>` - for random number generation
   - `<cmath>` - for mathematical operations
   - `<vector>` - for data storage

### Step 2: Implement WAV File Handler
1. Create or adapt WAV header structure:
   ```cpp
   struct WavHeader {
       char chunkID[4];        // "RIFF"
       uint32_t chunkSize;     // File size - 8
       char format[4];         // "WAVE"
       char subchunk1ID[4];    // "fmt "
       uint32_t subchunk1Size; // 16 for PCM
       uint16_t audioFormat;   // 1 for PCM
       uint16_t numChannels;   // 1 for mono
       uint32_t sampleRate;    // 44100
       uint32_t byteRate;      // sampleRate * numChannels * bitsPerSample/8
       uint16_t blockAlign;    // numChannels * bitsPerSample/8
       uint16_t bitsPerSample; // 16
       char subchunk2ID[4];    // "data"
       uint32_t subchunk2Size; // NumSamples * numChannels * bitsPerSample/8
   };
   ```
2. Implement function to write WAV header
3. Implement function to update data size in header

### Step 3: Calculate Filter Parameters
For each frequency, calculate:
1. **Delay length (D)**: `D = sampleRate / frequency`
2. **Integer delay (L)**: `L = floor(D)`
3. **Fractional delay (δ)**: `δ = D - L`
4. **Allpass coefficient (a)**: `a = (1 - δ) / (1 + δ)`

Example verification (440 Hz):
- D = 44100 / 440 = 100.227...
- L = 99
- δ = 0.727
- a = 0.158

### Step 4: Implement Three-Stage Filter System

#### Stage 1: Delay Loop with Feedback
1. Initialize STL queue with L zeros
2. For each sample:
   - Pop front element from queue
   - Multiply by R^L (decay factor)
   - Add to current input
   - Pass to next stage

#### Stage 2: Low-Pass Filter
Implement simple first-order low-pass filter:
```cpp
y[n] = 0.5 * (x[n] + x[n-1])
```

#### Stage 3: All-Pass Filter for Fractional Delay
Using the phase response theory from your reference material:
```cpp
H(ω) = (e^(-jω) - a) / (1 - a*e^(-jω))
```
Time-domain implementation:
```cpp
y[n] = -a * y[n-1] + x[n-1] + a * x[n]
```

### Step 5: Generate Major Scale Frequencies
Starting frequency F (from command line), generate 8 frequencies:

| Note | Step | Frequency Formula | Semitones from Start |
|------|------|------------------|---------------------|
| 1 | Do | F | 0 |
| 2 | Re | F × 2^(2/12) | 2 (whole step) |
| 3 | Mi | F × 2^(4/12) | 4 (whole step) |
| 4 | Fa | F × 2^(5/12) | 5 (half step) |
| 5 | Sol | F × 2^(7/12) | 7 (whole step) |
| 6 | La | F × 2^(9/12) | 9 (whole step) |
| 7 | Ti | F × 2^(11/12) | 11 (whole step) |
| 8 | Do | F × 2^(12/12) = 2F | 12 (half step) |

### Step 6: Main Processing Loop

```pseudocode
for each note in scale:
    1. Calculate filter parameters (D, L, δ, a)
    2. Reset filter states
    3. Initialize queue with L zeros
    4. Generate 100 random samples (-15000 to 15000)
    5. For 44100 samples (1 second):
        a. Process through delay loop
        b. Apply low-pass filter
        c. Apply all-pass filter
        d. Store output sample
    6. Write samples to output buffer
```

### Step 7: Program Structure

```cpp
int main(int argc, char* argv[]) {
    // 1. Parse command line for starting frequency
    if (argc != 2) {
        std::cerr << "Usage: pluck <frequency>" << std::endl;
        return 1;
    }
    float startFreq = std::stof(argv[1]);
    
    // 2. Initialize WAV file and parameters
    const int SAMPLE_RATE = 44100;
    const float R = 0.99985f;
    const int RANDOM_SAMPLES = 100;
    
    // 3. Create output buffer for 8 seconds
    std::vector<int16_t> outputBuffer;
    
    // 4. Generate each note of the scale
    float frequency = startFreq;
    for (int note = 0; note < 8; note++) {
        // Calculate parameters
        // Generate note
        // Advance frequency
    }
    
    // 5. Write WAV file
    writeWavFile("output.wav", outputBuffer);
    
    return 0;
}
```

## Key Implementation Notes

### Queue Management
- Use `std::queue<float>` for the delay line
- Initialize with L zeros: `for(int i = 0; i < L; i++) delayQueue.push(0.0);`
- During processing: `pop()` from front, process, `push()` to back

### Filter State Management
- Reset all filter states between notes
- Maintain previous sample values for low-pass and all-pass filters
- Clear the delay queue and reinitialize for each note

### Random Number Generation
```cpp
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(-15000, 15000);

for(int i = 0; i < 100; i++) {
    initialSamples[i] = dis(gen);
}
```

### Phase Response Considerations
From the theory material:
- The all-pass filter provides frequency-independent delay
- When a₁ = 1, the phase delay is exactly half a sample
- This maintains the harmonic structure of the plucked string

## Testing and Verification

1. **Frequency Check**: Verify 440 Hz produces parameters matching the example
2. **Scale Check**: Ensure proper major scale intervals
3. **Audio Quality**: Listen for:
   - Clean note attacks
   - Natural decay (using R = 0.99985)
   - Proper pitch relationships
   - No clipping or distortion

## Deliverables

1. `pluck.cpp` - Complete source code
2. Compiled executable
3. Sample output WAV file
4. All files in a single zipped folder

## Optional Enhancements

1. Add command-line options for:
   - Custom decay factor (R)
   - Different scales
   - Variable note duration
   
2. Implement amplitude envelope shaping
3. Add vibrato or other modulation effects

## References

- Chapter 6: Plucked String Filter implementation
- Chapter 4: Phase response theory (equations 7.1-7.7)
- Figure 8.1 (page 119): Three-stage filter structure