#ifndef FFT2_H
#define FFT2_H

#include <cmath>
#include <string>
using string = std::string;
#include <iostream>
#include <complex>
using complex = std::complex<double>;
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
using c_vector = std::vector<complex>;
using u_vector = std::vector<unsigned int>;




c_vector Read(string filename, u_int num);
//u_int bitReverse(u_int input, u_int numBits);
//u_int getNumBits(u_int N);

c_vector fft_InPlace(c_vector& data);

class FFT2 
{
public: // Vars
    u_int size_;
    string filename_;
    c_vector numbers_;
    
public:
    FFT2(u_int _size, string _filename)
    : size_(_size) , filename_(_filename) 
    {}

    void Read()
    {
        string line;
        std::ifstream file(filename_);
        //c_vector numbers;

        if(!file.is_open())
        {
            std::cerr << "Error opening file " << filename_ << std::endl;
            return;  
        }
        while(std::getline(file, line))
        {
            if (line.empty()) continue;        
            double real, imag;
            std::istringstream stream(line);
            // confirm input is in complex form (double, double)
            if(stream >> real >> imag) 
            {
                complex num(real, imag);
                numbers_.emplace_back(num);
            }
        }
        file.close();
    }
    
    int Verify()
    {
        if (!isPowerTwo(size_)) 
        {
            std::cerr << "Error: N must be a positive power of 2" << std::endl;
            return 1;
        }
        if(numbers_.size() !=
        static_cast<size_t>(size_))
        {
            std::cerr << "Error: Expected " << size_ << " complex numbers, got " << numbers_.size() << std::endl;
            return 1;
        }
        return 0;
    }
    
    void Execute();

public: // Bit reversal
    // Ensure is positive
    // Compare N and N-1 to see if N has only one bit set
    bool isPowerTwo(u_int N) 
    { 
        return N > 0 && ((N & (N-1)) == 0);
    }
    // Determine how many bits are needed to represent input
    u_int countBits(u_int N)
    {
        u_int numBits = 0;
        while(N > 1)
        {
            // Right shift binary representation
            // same as division (base two)
            N >>=1;
            ++numBits;
        }
        return numBits;
    }
    // Convert to binary
    u_vector Convert(u_int N)
    {
        u_int numBits = countBits(N);
        // make correct size and fill with 0s
        u_vector binary(numBits, 0);
        // build output right to left.
        for(u_int i = 0; i < numBits; ++i)
        {
            // numbits - 1 -> last index
            binary[ (numBits - 1) - i] =
            // AND mask, checking if the index of input
            // is set. 
            (N & (1 << i)) ? 1 : 0;
        }
        return binary;
    }
    // swap-in-place
    u_int Reverse(u_int input)
    {
        u_int numBits = countBits(size_);
        u_int reversed = 0;
        for(u_int i = 0; i < numBits; ++i)
        {
            u_int last = ((numBits - 1) - i);
            // checking opposite ends of bitstring
            if(input & (1 << i))
            { 
                // using OR to set particular bit
                reversed |= (1 << last);
            }
        }
        return reversed;
    }


};

class FFT
{
public:
    FFT(string _filename, int _size)
    : filename_(_filename), size_(_size)
    , numbers_(c_vector()), result_(c_vector())
    {}
    void Read();
    //   X[k] = Σ(even indices) + Σ(odd indices)
    //   = E[k] + W_N^k * O[k]
    // W_N^k = e^(-2πik/N) = cos(-2πk/N) + i·sin(-2πk/N)
    // X[k] = E[k] + W_N^k * O[k]
    // X[k + N/2] = E[k] - W_N^k * O[k]
        // W_N^(k + N/2) = W_N^k * W_N^(N/2) = W_N^k * (-1) = -W_N^k
    c_vector Execute(c_vector input);
    void Print();



    string filename_;
    int size_;
    c_vector numbers_;
    c_vector result_;
};

class DFT
{
public:
    DFT(u_int _size, c_vector _vec)
    : size_(_size), vec1_(_vec) {}

    void Execute();
    void Print();
private:    
    u_int size_;
    c_vector vec1_;
    c_vector result_;

};
#endif // FFT2_H

