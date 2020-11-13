#include <cstdlib>
#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

// Beep(261, <интервал_в_мс>); //  соответствует ДО
//   Beep(293, интервал_в_мс); //  РЕ
//   Beep(329, интервал_в_мс); //  МИ
//   Beep(349, интервал_в_мс); //  ФА
//   Beep(392, интервал_в_мс); //  СОЛЬ
//   Beep(440, интервал_в_мс); //  ЛЯ
//   Beep(493, интервал_в_мс); //  СИ
static map<int, const char*> frequency_to_name = {
    {261, "C"},
    {293, "D"},
    {329, "E"},
    {349, "F"},
    {392, "G"},
    {440, "A"},
    {493, "H"},
};

void Beep(int frequency, int length) {
    stringstream ss;
    ss << "play -n synth ";
    ss << length / 1000 << " ";
    ss << "pluck " << frequency;
//     system("play -n synth 1 pl E2");
    system(ss.str().c_str());
}

void playAccord(vector<const char*> accord) {
    stringstream ss;
    ss << "play -n synth ";
    ss << 2 << " ";
    for(const char* nota: accord) {
        ss << "pluck " << nota << " ";
    }
    system(ss.str().c_str());
}

int main() {
    Beep(800, 1000);
    Beep(440, 1000);
    playAccord(vector<const char*>{"E2", "A2", "D3", "G3", "B3", "E4"});
    playAccord(vector<const char*>{"G2", "B2", "D3", "G3", "D4", "G4"});
    return 0;
}
