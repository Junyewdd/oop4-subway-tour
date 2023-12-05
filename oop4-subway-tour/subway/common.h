#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stack>

vector<string> split(string input, char delimiter) {
    vector<string> answer;
    stringstream ss(input);
    string temp;

    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }

    return answer;
}