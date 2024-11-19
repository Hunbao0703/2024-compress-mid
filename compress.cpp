#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;





string read_file(const string& file_name) {
    ifstream ifs("./data/" + file_name, ios::in);
    if (!ifs.is_open()){ 
        cout << "file error";
        return 0;
    }

    stringstream ss;
    ss << ifs.rdbuf();
    string str(ss.str());
    ifs.close();
    return str;
}

unordered_map<char, int> cal_freq(const string& data) {
    unordered_map<char, int> freq;

    for (char c : data) {
        if (freq.find(c) != freq.end() && c != '\0') {
            freq[c]++;
        }
        else {
            freq[c] = 1;
        }
    }

    return freq;
}


int main() {
    int c;
    cout << "���Y�п�J 1\n�����Y�п�J 2\n��J:";
    cin >> c;
    if (c == 1) {
        string flie_name;
        cout << "�п�J�ɮצW��:";
        cin >> flie_name;

        string data = read_file(flie_name);

        unordered_map<char, int> freq = cal_freq(data);

        
    }

    
}







