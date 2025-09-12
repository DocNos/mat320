#include <complex>
#include <fstream>
#include <sstream>
#include <vector>
using string = std::string;
using comVec = std::vector<std::complex<double>>; 
using complex = std::complex<double>;
/*
Input files of complex numbers should be one number 
per line of the form: 
123 + 456789i or 123 456789i but not 123+ -456789i.
*/

comVec Read(string filename);

/* Complex numbers rotated by 
    2pi*x (counterclockwise for x > 0 and clockwise
    for x < 0) */
class Rotate{
public:
    Rotate(int _numVals, float _angle, string _filename)
    : numVals_(_numVals), angle_(_angle), filename_(_filename) 
    {}

    void execute();
    string filename_;

private:
    int numVals_;
    float angle_;
    
};
/* 
complex number sum of the first k powers 
 of the Nth root of unity e^(i2pi*N)
 1 + e^(i2pi/N) +(e^(i2pi/N))^2 + ... +(e^(i2pi/N))^k-1 */
class SumUnity{
public:
    SumUnity(int _root, int _length)
    : root_(_root), length_(_length) {}

    void execute();

private:
    int root_;
    int length_;
};
/* the complex inner product of the 
 two vectors given by the text files */
class InnerProd{
public:    
    InnerProd(const comVec& _vec1,const comVec& _vec2
        , string _filename)
    : vec1_(_vec1), vec2_(_vec2), filename_(_filename) {}

    void execute();

private:
    comVec vec1_, vec2_;
    string filename_;

};
/* complex inner product of the 
 vector given by the text file with the complex
 vector of roots of unity:
 ( 1, e^(i2/N), (e^(i2/N))^2, ..., (e^(i2/N))^N-1 )*/
class InnerUnity{
public:
    InnerUnity(const comVec& _vec1
        ,const comVec& _vec2, string _filename)
    : vec1_(_vec1), vec2_(_vec2), filename_(_filename) {}

private:
    comVec vec1_, vec2_;
    string filename_;
};

/*
1) input: command line args, 
argc = 5: bin/complex_calc 1 3 0.25 input/test1.txt
N (number of complex values),
x (angle multiplier), input.txt
 (text file of N complex numbers)
 output: the complex numbers rotated by 
 2pi*x (counterclockwise for x > 0 and clockwise
 for x < 0)
*/

/*
2) input: command line args
argc = 4: bin/complex_calc 2 8 5
: positive integers N and k
 output: complex number sum of the first k powers 
 of the Nth root of unity e^(i2pi*N)
 1 + e^(i2pi/N) +(e^(i2pi/N))^2 + ... +(e^(i2pi/N))^k-1
*/

/*
3)  input: 
argc = 5: 
bin/complex_calc 3 3 input/vec1.txt input/vec2.txt
command line arg N, 
and two text files of N complex numbers each
 output: the complex inner product of the 
 two vectors given by the text files
*/

/*
4) input: command line arg N, and text 
file of N complex numbers
argc = 4 : bin/complex_calc 4 4 input/signal.txt
 output: the complex inner product of the 
 vector given by the text file with the complex
 vector of roots of unity:
 ( 1, e^(i2/N), (e^(i2/N))^2, ..., (e^(i2/N))^N-1 )
*/