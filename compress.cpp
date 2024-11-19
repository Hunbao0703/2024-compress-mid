#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <bitset>

using namespace std;

class Node{
public:
    char charData;
    int freqData;
    Node* right;
    Node* left;

    Node(char charV, int freqV) {
        charData = charV;
        freqData = freqV;
        right = NULL;
        left = NULL;
    }
};





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
        if (freq.find(c) != freq.end()) {
            freq[c]++;
        }
        else {
            freq[c] = 1;
        }
    }

    return freq;
}

vector<Node*> nodes_sort(vector<Node*> nodes) {
    sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) {
        return a->freqData > b->freqData;
    });

    return nodes;
}

Node* build_tree(unordered_map<char, int> freq) {
    vector<Node*> nodes;
    for (const auto& row : freq) {
        nodes.push_back(new Node(row.first, row.second));
    }

    
    
    while(nodes.size() > 1) {
        nodes = nodes_sort(nodes);
        
        Node* left = nodes.back();
        nodes.pop_back();
        Node* right = nodes.back();
        nodes.pop_back();

        Node* merged = new Node(NULL, left->freqData + right->freqData);
        merged->left = left;
        merged->right = right;

        nodes.push_back(merged);
    }

    return nodes[0];
}


void build_code_map(Node* tree, unordered_map<char, string>& code_map, string result) {
    if (tree->charData != NULL) {
        code_map[tree->charData] = result;
        return;
    }

    if (tree->left) {
        build_code_map(tree->left, code_map, result + "0");
    }
    if (tree->right) {
        build_code_map(tree->right, code_map, result + "1");
    }
}

string compress(string data, unordered_map<char, string> code_map) {
    string result = "";

    for (const char row : data) {
        result += code_map[row];
    }

    return result;
}

string decompress(string data, unordered_map<char, string> code_map) {
    string result = "";

    unordered_map<string, char> reverse_code_map;
    for (const auto& row : code_map) {
        reverse_code_map[row.second] = row.first;
    }
    
    string tmp = "";
    size_t start = 0; 
    for (size_t i = 0; i < data.size(); ++i) {
        tmp += data[i];
        if (reverse_code_map.find(tmp) != reverse_code_map.end()) {
            result += reverse_code_map[tmp];
            tmp.clear();
            start = i + 1; 
        }
    }
    return result;
}

string serialize_code_map(const unordered_map<char, string>& code_map) {
    stringstream ss;
    for (const auto& [key, value] : code_map) {
        ss << key << ":" << value << ";";
    }
    return ss.str();
}

vector<unsigned char> compress_to_bytes(const string& binary_data) {
    vector<unsigned char> byte_array;

    unsigned char current_byte = 0;
    int bit_count = 0;

    for (char bit : binary_data) {
        current_byte = (current_byte << 1) | (bit - '0'); 
        bit_count++;

        if (bit_count == 8) { 
            byte_array.push_back(current_byte);
            current_byte = 0;
            bit_count = 0;
        }
    }

    
    if (bit_count > 0) {
        current_byte <<= (8 - bit_count); 
        byte_array.push_back(current_byte);
    }

    return byte_array;
}

void save(string data, unordered_map<char, string> code_map, string file_name) {
    string new_code_map = serialize_code_map(code_map);
    uint32_t map_length = new_code_map.size();

    ofstream output_file("./compress_file/" + file_name + ".huf", ios::binary);
    if (!output_file) {
        cout << "OUTPUT ERROR";
        return;
    }
    
    vector<unsigned char> compressed_data = compress_to_bytes(data);

    output_file.write(reinterpret_cast<char*>(&map_length), sizeof(map_length));
    output_file.write(new_code_map.c_str(), map_length);

    output_file.write(reinterpret_cast<char*>(compressed_data.data()), compressed_data.size());

    output_file.close();

    ifstream old_file("./data/" + file_name, ios::ate);
    ifstream new_file("./compress_file/" + file_name + ".huf", ios::ate);
    double old_file_size = old_file.tellg();
    double new_file_size = new_file.tellg();

    cout << "壓縮率: " << new_file_size / old_file_size << endl;

}

unordered_map<char, string> deserialize_code_map(const string& serialized_map) {
    unordered_map<char, string> code_map;
    stringstream ss(serialized_map);
    string item;

    while (getline(ss, item, ';')) {
        if (item.empty()) continue;
        char key = item[0];
        string value = item.substr(2);
        code_map[key] = value;
    }
    return code_map;
}

string decompress_to_binary(const vector<unsigned char>& byte_array, size_t bit_count) {
    string binary_data;
    for (size_t i = 0; i < byte_array.size(); ++i) {
        bitset<8> bits(byte_array[i]);
        string bit_str = bits.to_string();
        binary_data += (i == byte_array.size() - 1) ? bit_str.substr(0, bit_count % 8 ? bit_count % 8 : 8) : bit_str;
    }
    return binary_data;
}

pair<string, unordered_map<char, string>> load(const string& file_name) {
    string compressed_file = "./compress_file/" + file_name;
    ifstream input_file(compressed_file, ios::binary);
    if (!input_file) {
        cerr << "Error: Unable to open file for reading!" << endl;
        return {"", {}};
    }
    uint32_t map_length;
    input_file.read(reinterpret_cast<char*>(&map_length), sizeof(map_length));

    string serialized_map(map_length, '\0');
    input_file.read(&serialized_map[0], map_length);

    unordered_map<char, string> code_map = deserialize_code_map(serialized_map);

    vector<unsigned char> byte_array((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());

    size_t total_bits = byte_array.size() * 8; 
    string data = decompress_to_binary(byte_array, total_bits);

    input_file.close();
    return {data, code_map};
}


int main() {
    int c;
    cout << "壓縮請輸入 1\n解壓縮請輸入 2\n輸入:";
    cin >> c;
    if (c == 1) {
        string file_name;
        cout << "請輸入檔案名稱:";
        cin >> file_name;

        string data = read_file(file_name);

        unordered_map<char, int> freq = cal_freq(data);

        Node* tree = build_tree(freq);

        unordered_map<char, string> code_map;
        build_code_map(tree, code_map, "");

        string compress_data = compress(data, code_map);

        save(compress_data, code_map, file_name);
    }
    else if (c == 2) {
        string file_name;
        cout << "請輸入檔案名稱:";
        cin >> file_name;

        auto [data, code_map] = load(file_name);
        
        string decompress_data = decompress(data, code_map);

        ofstream output_flie("./output/" + file_name.replace(file_name.find(".txt.huf"), 8, "") + "_decompress.txt", ios::out);
        output_flie.write(decompress_data.c_str(), decompress_data.size());
        output_flie.close();
    }


    system("pause");
    return 0;
}  







