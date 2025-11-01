#include <string>
using string = std::string;
class Lowpass
{
private:
    /* data */
public:
    Lowpass(float coefficent, int N, string wav);
    ~Lowpass();

    /*
    Filter coefficent a1
    an integer number n of times to run the filter
    and a wave file:
    ./lowpass < a1 > < n > < inputwav >
 */
    void Read();
};


