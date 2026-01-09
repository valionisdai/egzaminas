#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>



using namespace std;

class zodzio_info {
    public:
        vector<int> eilutes;
        int kiekis = 0;
};

string valymas (string zodis);

int main() {

    unordered_map<string, zodzio_info> p_zodziai;
  
    ifstream rd("tekstas.txt");
    string word, rez;
    if (!rd.is_open()) {
        cerr << "Nepavyko atidaryti failo!" << endl;
    }

    string line, zodis;
    int eilnr = 0;
    
    while(getline(rd, line))
    {
        istringstream iss(line);
        while (iss >> zodis)
        {
            zodis = valymas(zodis);
            if (zodis.empty()) continue;
            auto & info = p_zodziai[zodis];
            info.kiekis++;
        }
    }

    ofstream wr("rez.txt");
    wr << left << setw(25) << "Zodis" << setw(8) << "Kiekis" << "Eilutes" << endl;
    wr << string(60, '-') << endl;

    for (auto& i : p_zodziai) {
        if (i.second.kiekis > 1) 
            wr << left << setw(25) << i.first << setw(8) << i.second.kiekis << endl;
    }
    return 0;
}

string valymas (string zodis)
{
    zodis.erase(remove_if(zodis.begin(), zodis.end(), [](unsigned char c) { return ispunct(c); }), zodis.end());
    zodis.erase(zodis.begin(), find_if(zodis.begin(), zodis.end(), [](unsigned char c){ return !isspace(c); }));
    zodis.erase(find_if(zodis.rbegin(), zodis.rend(), [](unsigned char c){ return !isspace(c); }).base(), zodis.end());   
    transform(zodis.begin(), zodis.end(), zodis.begin(), ::tolower);
    return zodis;
}
