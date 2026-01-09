#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <regex>
#include <unordered_set>


using namespace std;

class zodzio_info {
    public:
        vector<int> eilutes;
        int kiekis = 0;
};

string valymas (string zodis);

int main() {

    unordered_map<string, zodzio_info> p_zodziai;
    vector<string> urls;

    regex urlreg(R"((https?:\/\/)?(www\.)?[a-zA-Z0-9-]+\.[a-zA-Z]{2,})");

    ifstream rd("tekstas.txt");
    string word, rez;
    if (!rd.is_open()) {
        cerr << "Nepavyko atidaryti failo!" << endl;
    }

    // TLDu skaitymas is failo 
    unordered_set<string> ttld;
    
    ifstream tf("url.txt");
    string t;
    while (tf >> t)
    {
        transform(t.begin(), t.end(), t.begin(), ::tolower);
        ttld.insert(t);
    }

    string line, zodis;
    int eilnr = 0;
    
    while(getline(rd, line))
    {
        eilnr++;
        string temp = line;
        smatch match;
        vector<pair<size_t, size_t>> urltask;
        while(regex_search(temp, match, urlreg))
        {
            string url = match.str();
            bool tikras_url = false;
            size_t pos = url.find_last_of('.');
            if (pos != string::npos)
            {
                string tld = url.substr(pos + 1);
                transform(tld.begin(), tld.end(), tld.begin(), ::tolower);
                if (ttld.count(tld))
                {
                    tikras_url = true;
                    urls.push_back(url);
                }
            }
            if (tikras_url)
            {
                size_t start = match.position() + (line.length() - temp.length());
                size_t end = start + match.length();
                urltask.push_back({ start, end });
            }

            temp = match.suffix().str();
        }

        for(auto it = urltask.rbegin(); it != urltask.rend(); ++it) {
            line.replace(it->first, it->second - it->first, " ");
        }

        line = regex_replace(line, regex(R"(\s+)"), " ");
        
        if (all_of(line.begin(), line.end(), [](unsigned char c){ return isspace(c); })) continue;

        istringstream iss(line);
        while (iss >> zodis)
        {
            zodis = valymas(zodis);
            if (zodis.empty()) continue;
            auto & info = p_zodziai[zodis];
            info.kiekis++;
            if (info.eilutes.empty() || info.eilutes.back() != eilnr)
                info.eilutes.push_back(eilnr);
        }
    }

    ofstream wr("rez.txt");
    wr << left << setw(25) << "Zodis" << setw(8) << "Kiekis" << "Eilutes" << endl;
    wr << string(60, '-') << endl;

    for (auto& i : p_zodziai) {
        if (i.second.kiekis > 1) 
        {
            wr << left << setw(25) << i.first << setw(8) << i.second.kiekis;
            for (auto eil : i.second.eilutes)
                wr << eil << " ";
            wr << endl;
        }
    }


    ofstream urlwr("nuorodos.txt");
    for (const auto& url : urls)
        urlwr << url << endl;

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
