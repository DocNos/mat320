#include "wavAPI.h"

int main(int argc, char* argv[])
{
    Pluck filterPercussion(GetPreset(0));
    Pluck filterLead(GetPreset(1));
    vector<int16_t> percussionOut = filterPercussion.Execute();
    vector<int16_t> leadOut = filterLead.Execute();

    vector<vector<int16_t>> voices = { percussionOut, leadOut};
    vector<int16_t> mixed = Mix(voices, filterPercussion.numSamples_);

    WriteWav("output/lead.wav", filterLead.header_, leadOut);
    WriteWav("output/percussion.wav", filterPercussion.header_, percussionOut);
    WriteWav("output/mixed.wav", filterLead.header_, mixed);
    return 0;
}