#include "station.h"
#include "shortTime.h"
#include "middleSpot.h"
#include "play.h"
#include "mainStation.h"
#include "food.h"
#include "common.h"
#include <conio.h>
#include <Windows.h>
#include <thread>
#include <future>

using namespace std;

pair<int, int> decimal_to_time(double decimal) {
    int minutes = static_cast<int>(decimal);
    int seconds = static_cast<int>(std::round((decimal - minutes) * 60));
    return { minutes, seconds };
}

pair <string, pair<string, string>> add_minutes(int hour, int minute, double add) {

    pair<int, int> add_time = decimal_to_time(add);

    int add_minute = add_time.first;

    string final_second = to_string(add_time.second);

    int total_minutes = hour * 60 + minute + add_minute;

    int new_hour = total_minutes / 60;
    int new_minute = total_minutes % 60;

    if (new_hour >= 24) new_hour -= 24;
    string final_hour, final_minute;
    final_hour = to_string(new_hour);
    final_minute = to_string(new_minute);

    return { final_hour, {final_minute, final_second} };
}


int main() {
    while (1) {
        cout << "\n\n\n    ▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤     ▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤     ▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n";
        cout << "    ▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤     ▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤     ▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n";
        cout << "           ▤▤                   ▤▤                   ▤▤\n";
        cout << "           ▤▤                   ▤▤                   ▤▤\n";
        cout << "           ▤▤                   ▤▤                   ▤▤\n";
        cout << "           ▤▤                   ▤▤                   ▤▤\n";
        cout << "           ▤▤                   ▤▤                   ▤▤\n";
        cout << "           ▤▤                   ▤▤                   ▤▤\n\n";
        cout << "                      Transit Tour Treasure\n\n\n\n\n";
        cout << "                        +---------------+\n";
        cout << "                        |  -- ENTER --  |\n";
        cout << "                        +---------------+\n";

        char enter;
        enter = _getch();
        while (enter != 13) {
            enter = _getch();
        }
        system("cls");
        unordered_map<string, double> d;
        unordered_map<string, vector<Station>> stationList;
        unordered_map<string, Station> station;
        unordered_map<string, vector<pair<string, double>>> transinfo;

        ifstream in("subwayInfo.txt");
        string line;
        string name1;
        string name2;
        string key1;
        string key2;
        string lineNum;
        while (getline(in, line)) {
            vector<string> v = split(line, ':');
            lineNum = v[0];
            name1 = v[1];
            key1 = name1 + '/' + lineNum;
            name2 = v[2];
            key2 = name2 + '/' + lineNum;
            if (station.find(key1) == station.end()) {
                station[key1] = Station(name1);
                station[key1].setLine(lineNum);

                stationList[name1].push_back(station[key1]);

            }
            if (station.find(key2) == station.end()) {
                station[key2] = Station(name2);
                station[key2].setLine(lineNum);

                stationList[name2].push_back(station[key2]);
            }
            double time = stod(v[3]);
            double distance = stod(v[4]);

            station[key1].addNeighbor(station[key2], distance, time);
            station[key2].addNeighbor(station[key1], distance, time);
            d[key1] = 1e9;
        }
        d[key2] = 1e9;

        ifstream ifs("transinfo.txt");
        string line2;
        string s_name;
        while (getline(ifs, line2)) {
            vector<string> v2 = split(line2, ':');
            s_name = v2[0];
            string li = v2[1] + ':' + v2[2];//lineinfo
            double movingtime = stod(v2[3]);
            transinfo[s_name].push_back(make_pair(li, movingtime));
        }

        for (const auto& pair1 : station) {
            for (const auto& pair2 : station) {
                if (pair1.second.getName() == pair2.second.getName()) {
                    double dist = 0;
                    double ti = 0;
                    for (const auto& pair : transinfo[pair1.second.getName()]) {
                        if (pair.first == (pair1.second.getLine() + ':' + pair2.second.getLine()) || pair.first == (pair2.second.getLine() + ':' + pair1.second.getLine()))
                            ti = pair.second;
                    }
                    station[pair1.first].addNeighbor(station[pair2.first], dist, ti);
                    station[pair2.first].addNeighbor(station[pair1.first], dist, ti);
                }
            }
        }


        string start, end;
        string optSubway;
        cout << "+------------------+\n";
        cout << "| 1. Best Path     |\n| 2. Find Midpoint |\n";
        cout << "+------------------+\n\n";
        cout << "Enter a number : ";
        while (1) {
            cin >> optSubway;
            if (optSubway == "1" || optSubway == "2")break;
            cout << "Please enter one or two" << endl;
        }

        string first = "Place of departure";
        string second = "Place of destination";

        if (optSubway == "2") {
            first = "PersonA's departure";
            second = "PersonB's departure";
        }
        system("cls");
        if (optSubway == "1") {
            cout << "+-------------+\n";
            cout << "| Path Output |\n";
            cout << "+-------------+\n";
        }
        else {
            cout << "+----------------+\n";
            cout << "| Find Midpoint |\n";
            cout << "+----------------+\n";
        }
        cout << first << " : ";
        cin >> start;

        ShortTime shortTime = ShortTime(d, station);
        auto future = std::async(std::launch::async, [&]() { shortTime.dijkstra(start); });

        if (stationList[start].size() == 1) {
            start = start + '/' + stationList[start][0].getLine();
        }
        else {
            cout << "Select " << first << "'s line. (Just enter the numbers)" << endl;
            for (int i = 0; i < stationList[start].size(); i++) {
                cout << i + 1 << ") " << stationList[start][i].getLine() << '\n';
            }
            string whichLine;
            cout << ">> ";
            cin >> whichLine;
            start = start + '/' + stationList[start][stoi(whichLine) - 1].getLine();
            cout << endl;
        }

        string hour, minute;
        cout << endl << "Enter the departure time." << endl << "Hour(ex. 13) : ";
        cin >> hour;
        cout << "Min(ex. 22) : ";
        cin >> minute;
        cout << endl;


        cout << second << " : ";
        cin >> end;
        string finalend;
        finalend = end;
        if (stationList[end].size() == 1) {
            end = end + '/' + stationList[end][0].getLine();
        }
        else {
            cout << "Select " << second << "'s line. (Just enter the numbers)" << endl;
            for (int i = 0; i < stationList[end].size(); i++) {
                cout << i + 1 << ") " << stationList[end][i].getLine() << '\n';
            }
            string whichLine;
            cout << ">> ";
            cin >> whichLine;
            end = end + '/' + stationList[end][stoi(whichLine) - 1].getLine();
        }
        system("cls");
        while (1) {
            cout << "LOADING.";
            Sleep(1000);
            system("cls");

            if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                break;
            }
            cout << "LOADING..";
            Sleep(1000);
            system("cls");

            if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                break;
            }
            cout << "LOADING...";
            Sleep(1000);
            system("cls");
            if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                break;
            }
        }

        future.get();
        vector<Station> route = shortTime.findRoute(start, end);

        system("cls");
        if (optSubway == "1") {
            cout << "---------------------\n";
            cout << "departure : " << start << '\n';
            cout << "destination : " << end << '\n';
            cout << "---------------------\n\n";
            cout << endl << "Time Taken - " << d[end] + route.size() * 0.8 << endl << endl;

            pair <string, pair<string, string>> result_time = add_minutes(stoi(hour), stoi(minute), d[end] + route.size() * 0.8);
            cout << "Time of arrival - " << result_time.first << " : " << result_time.second.first << " : " << result_time.second.second << endl << endl;

        }
        else if (optSubway == "2") {
            cout << "---------------------------\n";
            cout << "PersonA's departure : " << start << '\n';
            cout << "PersonB's departure : " << end << '\n';
            cout << "---------------------------\n\n";
            for (int i = 0; i < route.size(); i++) {
                calcu = d[route[i].getName() + '/' + route[i].getLine()] * d[route[i].getName() + '/' + route[i].getLine()] + (d[route[route.size() - 1].getName() + '/' + route[route.size() - 1].getLine()] - d[route[i].getName() + '/' + route[i].getLine()]) * (d[route[route.size() - 1].getName() + '/' + route[route.size() - 1].getLine()] - d[route[i].getName() + '/' + route[i].getLine()]);
                if (calcu < minError) {
                    minError = calcu;
                    minNode = route[i];
                }
            }
            cout << "Middle Point : " << minNode.getName() << endl << endl;
            cout << endl << "Time Taken : " << d[end] + route.size() * 0.8 << endl << endl;

            pair <string, pair<string, string>> result_time = add_minutes(stoi(hour), stoi(minute), (d[end] + route.size() * 0.8) / 2);
            cout << "Time of arrival : " << result_time.first << " : " << result_time.second.first << " : " << result_time.second.second << endl << endl;


        }
        //-------------------


        //-------------------

        string stationName;
        string line1;
        int idx = 0;

        if (optSubway == "1") {
            cout << "+------+\n";
            cout << "| Route |" << endl;
            cout << "+------+\n\n";
        }
        else if (optSubway == "2") {
            cout << "-----------------\n";
            cout << "PersonA's Route" << endl;
            cout << "-----------------\n";
        }
        for (; idx < route.size(); idx++) {
            stationName = route[idx].getName();


            if (idx != 0 && route[idx].getName() == route[idx - 1].getName()) {
                line1 = route[idx].getLine();
                line2 = route[idx - 1].getLine();
                for (const auto& info : transinfo[stationName]) {
                    if (info.first == line1 + ":" + line2 || info.first == line2 + ":" + line1) {
                        cout << "| =" << info.second << "분 환승" << "=" << endl;
                        cout << "| \n";
                        break;
                    }
                }
            }

            cout << "* " << stationName << endl;

            if (optSubway == "2" && minNode.getName() == stationName) {
                break;
            }
            if (idx != route.size() - 1) {
                cout << "| \n";
            }
        }
        if (optSubway == "2") {
            cout << "-----------------\n";
            cout << "PersonB's Route" << endl;
            cout << "-----------------\n";
            int idx2 = route.size() - 1;
            for (; idx2 >= idx; idx2--) {
                stationName = route[idx2].getName();

                if (idx2 != 0 && route[idx2].getName() == route[idx2 - 1].getName()) {
                    line1 = route[idx2].getLine();
                    line2 = route[idx2 - 1].getLine();
                    for (const auto& info : transinfo[stationName]) {
                        if (info.first == line1 + ":" + line2 || info.first == line2 + ":" + line1) {
                            cout << "| =" << info.second << "minutes for transition" << "=" << endl;
                            break;
                        }
                    }
                }
                cout << "* " << stationName << endl;
                if (idx2 != idx) {
                    cout << "| \n";
                }
            }
        }
        cout << endl;

        ////////////////
        if (optSubway == "1") {
            cout << end << " - To search for attractions or restaurants, press ENTER. To return to the main screen, press M (To finish: ESC).";
        }
        else {
            cout << stationName << " - To search for attractions or restaurants, press ENTER. To return to the main screen, press M (To finish: ESC).";

        }
        char input;
        input = _getch();
        system("cls");
        if (input == 109) {
            continue;
        }
        if (input == 13) {
            ifstream in1("around.txt");
            string name;
            string lineAround;
            string findplace;
            string findfood;

            string plays;
            string foods;
            int opt1;
            string opt2;
            unordered_map<string, Mainstation> maindata;
            unordered_map<string, Food> fooddata;
            vector<string>v2;
            while (getline(in1, lineAround)) {
                vector<string> v = split(lineAround, ':');
                name = v[0];
                maindata[name] = Mainstation(name);
                maindata[name].setMain(v[1], v[2]);
            }
            in1.close();

            //attractions
            ifstream inPlay("play.txt");
            string linePlay;
            unordered_map<string, Play> playdata;
            while (getline(inPlay, linePlay)) {
                vector<string> v = split(linePlay, ':');
                if (v.size() == 2) {
                    string placeName = v[0];
                    string placeDetail = v[1];
                    playdata[placeName] = Play(placeName, placeDetail);
                }
            }
            inPlay.close();

            //restaurant
            ifstream inFood("food.txt");
            string lineFood;
            string nameFood;
            unordered_map<string, Food> restaurantData;
            while (getline(inFood, lineFood)) {
                vector<string> v = split(lineFood, ':');
                nameFood = v[0];
                restaurantData[nameFood] = Food(nameFood, v[1]);
                restaurantData[nameFood].setFoodMenu(v[2]);
            }
            inFood.close();

            vector<string> plans;
            if (optSubway == "1") {
                name = finalend;
            }
            else {
                name = minNode.getName();
            }
            vector<string> restaurants = maindata[name].getRestaurants();
            while (1) {
                cout << "Please enter 1 for attractions at the final destination, or 2 for restaurants (To finish: press 3) : ";
                cin >> opt1;
                if (opt1 == 1) {
                    maindata[name].printPlaces();
                    cout << "Please select your desired location : ";
                    cin.ignore();
                    getline(cin, findplace);
                    system("cls");
                    playdata[findplace].printPlacedata(findplace);
                    plans.push_back(findplace);
                }
                else if (opt1 == 2) { //Show a brief description of the restaurant // Show menu details when selected
                    for (auto& restaurant : restaurantData) {

                        auto exist = find(restaurants.begin(), restaurants.end(), restaurant.first);
                        if (exist != restaurants.end()) {
                            restaurant.second.printDetail();

                        }
                    }

                    cout << endl << "Please select your desired restaurant : ";
                    cin.ignore();
                    getline(cin, findfood);
                    system("cls");
                    restaurantData[findfood].printFoodMenu();
                    plans.push_back(findfood);
                }
                else if (opt1 == 3) {
                    cout << "Plan for activities : " << endl;
                    int index = 1;
                    for (string plan : plans) {
                        cout << index++ << ". " << plan << endl;
                    }
                    break;
                }
                while (1) {
                    cout << "\nIf you want to check attractions/restaurants again, press 'R', to exit, press 'ESC'.\n";
                    char input1;
                    input1 = _getch();
                    if (input1 == 114) {
                        system("cls");
                        break;
                    }
                    else if (input1 == 27) {
                        exit(0);
                    }
                    else {
                        system("cls");
                        cout << "Please enter again.\n";
                    }
                }
                cout << endl;
            }
        }
        else if (input == 27) {
            exit(0);
        }
        else {
            cout << "Please enter again.\n";
        }

    }
}