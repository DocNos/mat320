import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as sciSig
import os

def calcCentVal(a0, a1):
    '''
    Get x distinct L values in range
    Check how many are within 5 cents of a pitch on 
    a even tempered piano
    '''
    sampleRate = 44100
    val = int(a0)
    lVals = []
    while val <= a1:
        lVals.append(int(sampleRate / val))
        val *= 2
    
    count = len(lVals)
    num = lVals[0] - lVals[count-1]+1
    print(f"Problem [1]: frequency range between A{a0} and A{a1}")
    print("This range has", num, "distinct pitches")
    print("the L-Values in this range are", lVals[::-1])

    cent5Upper = pow(2, (5/1200))
    cent5Lower = pow(2, (-5/1200))

    pianoFreqs = []
    currFreq = a0
    while currFreq <= a1:
        pianoFreqs.append(currFreq)
        semitone = pow(2, 1/12) * currFreq        
        currFreq = semitone
    # print("Piano frequencies in this range are", pianoFreqs)

    cent5Tolerance = []
    for pianoFreq in pianoFreqs:
        for j in lVals:
            combFreq = sampleRate / j
            freqRatio = combFreq / pianoFreq
            if (cent5Lower <= freqRatio) & (freqRatio <= cent5Upper):
                cent5Tolerance.append(combFreq)
    print("Frequencies in this range that are in the 5 cent tolerance are", cent5Tolerance)

def Resonance_PluckedString(L_Val, R_Val, numHarmonix):
    numPoints = 2000    
    frequencyArray = np.linspace(0, np.pi, numPoints)
    normalOmega = []
    for omega in frequencyArray:
        normalOmega.append(omega / (2* np.pi))

    def TransferFunction(freqArray, L, R):
        numerator_r = []
        numerator_i = []
        denom_r = []
        denom_i = []
        for omega in freqArray:
            real_n = np.cos((L + 1)* omega) + np.cos(L*omega)
            numerator_r.append(real_n)
            
            real_d = 2*np.cos( (L + 0.5)* omega) - R*np.cos(omega) - pow(R, L)
            denom_r.append(real_d)
            
            imag_n = np.sin((L+1)* omega) + np.sin(L*omega)
            numerator_i.append(imag_n)
            
            imag_d = 2*np.sin((L + 0.5) * omega) - R*np.sin(omega)
            denom_i.append(imag_d)
        return numerator_r, numerator_i, denom_r, denom_i
    transferRes = TransferFunction(frequencyArray, L_Val, R_Val)
    
    def MagnitudeResponse(transferResults):
        '''
    Magnitude = sqrt((real_n² + imag_n²) / (real_d² + imag_d²))
        '''
        num_r = np.array(transferResults[0])
        denom_r = np.array(transferResults[2])
        num_i = np.array(transferResults[1])
        denom_i = np.array(transferResults[3])

        numeratorMagnitudes = []
        for numR, numI in zip(num_r, num_i):
            num_mag = (numR **2) + (numI **2)
            numeratorMagnitudes.append(num_mag)
        
        denominatorMagnitudes = []
        for denomR, denomI in zip(denom_r, denom_i):
            denom_mag = (denomR ** 2) + (denomI **2)
            denominatorMagnitudes.append(denom_mag)
        
        decibels = []
        for mag_N, mag_D in zip(numeratorMagnitudes, denominatorMagnitudes):
            result = np.sqrt(mag_N/mag_D)
            result = 20 * np.log10(result)
            decibels.append(result)
        return decibels
    decibels = np.array(MagnitudeResponse(transferRes))
    
    def ResonanceMarkers(harmonics, lVal):
        lRatio = lVal + 0.5
        expected = np.arange(1, harmonics+1) / lRatio
        omegaExpected = []
        for omega in expected:
            omegaExpected.append(omega * np.pi)
        return omegaExpected
    expected = np.array(ResonanceMarkers(numHarmonix, L_Val))
    
    def ExpectedMagnitude(frequencyArray, decibelArray, expectedOmega):
        markerMag = []
        for omega in expectedOmega:
            closest = np.argmin(np.abs(frequencyArray - omega))
            markerMag.append(decibelArray[closest])
        return markerMag
    
    def Plot(omegaArray, normalizedFreq, decibelMag, expectedOmega):
        """
        Plot the frequency response of a plucked string filter
        Args:
            omegaArray: np.array 
                Frequency values in the range [0, π]
            normalizedFreq: list 
                Omega values normalized to [0, 0.5]
            decibelMag: np.array 
                Magnitude response in dB
            expectedOmega: np.array 
                Expected calculated resonance frequencies (peaks) in radians
        """
        plt.figure(figsize=(10,6))
        plt.plot(normalizedFreq, decibelMag, 'b-', linewidth=1)
        plt.ylim(-20,50)
        plt.ylabel("Magnitude Response, dB")

        plt.xlim(-0.025, 0.5)
        plt.xlabel("Frequency, fractions of sample rate")

        magOmegas = ExpectedMagnitude(omegaArray, decibelMag, expectedOmega)
        plt.plot(expectedOmega / (2 * np.pi), magOmegas, 'v', markersize=8, color='red')
        
        plt.title("Plucked String Filter Response")
        plt.grid(True)
        os.makedirs('../output', exist_ok=True)
        plt.savefig('../output/pluckedString.png')
    Plot(frequencyArray,normalOmega, decibels, expected)



def main():
    # calcCentVal(220, 1760)
    Resonance_PluckedString(32, 0.999, 16)

if __name__ == "__main__":
    main()


# def printResults_transfer(transfer_result):
#     num_r = np.array(transferRes[0])
#     denom_r = np.array(transferRes[2])
#     for n,d in zip(num_r,denom_r):
#         print(f"Real part transfer result: {n/d}")# 
#     num_i = np.array(transferRes[1])
#     denom_i = np.array(transferRes[3])
#     for n,d in zip(num_i,denom_i):
#         print(f"Imaginary part transfer result: {n/d}")
    