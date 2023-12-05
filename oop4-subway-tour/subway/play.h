#pragma once
#include "common.h"
using namespace std;

class Play {
private:
    string name;
    string detail;
public:
    Play(){}
    Play(string name) {
        this->name = name;
    }
    Play(string name, string detail) {
        this->name = name;
        this->detail = detail;
    }
    void printPlacedata(string endplace) {
        cout <<"<" << name << "> " << detail << endl;
    }
};
