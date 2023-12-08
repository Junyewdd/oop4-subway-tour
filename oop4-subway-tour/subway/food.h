#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Food {
private:
    string name;
    string detail;
    vector < pair < string, string >> foodMenu;


public:
    Food() {
    }
    Food(string name, string detail) {
        this->name = name;
        this->detail = detail;
    }
    string Getdetail() {
        return detail;
    }
    void printDetail() {
        if (!name.empty()) {
            cout << "<" << name << "> " << detail << endl;
        }
    }
    void setFoodMenu(string menus) {
        vector<string> foods = split(menus, ',');
        for (string food : foods) {
            vector<string> foodInfo = split(food, ' ');
            foodMenu.push_back(make_pair(foodInfo[0], foodInfo[1]));
        }

    }

    vector<pair<string, string>> Getfoodsave() {
        return foodMenu;
    }

    void printFoodMenu() {
        for (pair<string, string> foodInfo : foodMenu) {
            cout << "<" << foodInfo.first << "> " << foodInfo.second << "Won" << endl;
        }
    }
};