#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <map>

using namespace std;

static map<int, const char*> frequency_to_name = {
    {261, "C"},
    {293, "D"},
    {329, "E"},
    {349, "F"},
    {392, "G"},
    {440, "A"},
    {493, "H"},
};

static map<char, int> name_to_frequency = {
    {'C', 261},
    {'D', 293},
    {'E', 329},
    {'F', 349},
    {'G', 392},
    {'A', 440},
    {'H', 493},
};

void Beep(int frequency, int length) {
    stringstream ss;
    ss << "play -n synth ";
    ss << length / 1000. << " ";
    ss << "pluck " << frequency;
    ss << " > /dev/null 2>@1";
    system(ss.str().c_str());
}

void Play(const char* note, int length) {
    Beep(name_to_frequency[note[0]] * (note[1] - '0'), length);
}

void PlaySequence(const char* notes, int length) {
    while (*notes) {
        Beep(name_to_frequency[notes[0]] * (notes[1] - '0'), length);
        notes += 2;
    }
}
