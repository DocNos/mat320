import numpy as np
import matplotlib as mpl
import scipy.signal as sciSig

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

def Resonance_PluckedString(L_Val, R_Val, sampleRate):
    

def main():
    calcCentVal(220, 1760)
    Resonance_PluckedString(32, 0.999, 44100)

if __name__ == "__main__":
    main()
    