#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;


class Node {

public:
    char charData;
    int freq;
    Node* right;
    Node* left;

    Node(char charV, int freqV) {
        charData = charV;
        freq = freqV;
        right = nullptr;
        left = nullptr;
    }
};


unordered_map<char, int> cal_freq(const string& data) {
    unordered_map<char, int> freq;
    
    for (char c : data) {
        if (freq.find(c) != freq.end()) {
            freq[c]++;
        }
        else {
            freq[c] = 1;
        }
    }

    return freq;
}


int main() {
    string data = "test";

    unordered_map<char, int> freq = cal_freq(data);

    for (const auto& row: freq) {
        cout << row.first << row.second;
    }

    return 0;

}














