#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "common.h"
using namespace std;

class Mainstation {
private:
    string name;
    vector<string> places;
    vector<string> restaurants;

public:
    Mainstation(string name) {
        this->name = name;
    }
    Mainstation() {}

    void setMain(string placeInfo, string foodInfo) {
        places = split(placeInfo, ',');
        restaurants = split(foodInfo, ',');
    }

    vector<string> getPlaces() {
        return places;
    }
    vector<string> getRestaurants() {
        return restaurants;
    }
    void printPlaces() {
        cout << "Attractions at the final destination :" << endl;
        for (const auto& place : places) {
            vector<string> v1 = split(place, ',');
            for (const auto& item : v1) {
                cout << item << endl;
            }
        }
        cout << endl;
    }

};