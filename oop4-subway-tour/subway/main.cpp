#include "station.h"
#include "shortTime.h"
#include "middleSpot.h"
#include "play.h"
#include "mainStation.h"
#include "food.h"
#include "common.h"
#include <thread>
#include <future>

using namespace std;

unordered_map<string, vector<Station>> stationList;

int main() {
    ////// 정보 정리
    unordered_map<string, double> d;
    unordered_map<string, Station> station; //역 이름/호선, 역 객체
    unordered_map<string, vector<pair<string, double>>> transinfo;//역 이름/<line:line,이동 시간>

    // 역 간 정보 정리
    ifstream in("subwayInfo.txt");
    string line;
    string name1;
    string name2;
    string key1;
    string key2;
    string lineNum;
    while (getline(in, line)) {// line:역:역:시간:거리
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

    //환승역 정보 저장
    ifstream ifs("transinfo.txt");
    string line2;
    string s_name;
    while (getline(ifs, line2)) {//역 이름:line1:line2:이동시간
        vector<string> v2 = split(line2, ':');
        s_name = v2[0];
        string li = v2[1] + ':' + v2[2];//lineinfo
        double movingtime = stod(v2[3]);
        transinfo[s_name].push_back(make_pair(li, movingtime));
    }

    //환승역끼리 neighbor
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


    //////사용자 입력 시작
    string start, end;
    string optSubway;    
    cout << "1. 경로 출력\n2. 중간 지점 찾기" << endl<<"숫자를 입력하세요 : ";
    while (1) {
        cin >> optSubway;
        if (optSubway == "1" || optSubway == "2")break;
        cout << "1, 2 중 입력해주세요"<<endl;
    }

    string first = "출발지";
    string second = "도착지";

    if (optSubway == "2") {
        cout << "중간 지점 정하기"<<endl;
        first = "PersonA 출발지";
        second = "PersonB 출발지";
    }

    //출발역 설정
    cout << first << " : ";
    cin >> start;

    ShortTime shortTime = ShortTime(d, station);
    auto future = std::async(std::launch::async, [&]() { shortTime.dijkstra(start); });

    if (stationList[start].size() == 1) {
        start = start + '/' + stationList[start][0].getLine();
    }
    else {
        cout << first<<"의 호선을 선택해주세요. (숫자만 입력해주세요)" << endl;
        for (int i = 0; i < stationList[start].size(); i++) {
            cout << i + 1 << ". " << stationList[start][i].getLine() << '\n';
        }
        string whichLine;
        cin >> whichLine;
        start = start + '/' + stationList[start][stoi(whichLine) - 1].getLine();
        cout << endl;
    }

    //도착역 설정
    cout << second<<" : ";
    cin >> end;
    string finalend;
    finalend = end;
    if (stationList[end].size() == 1) {
        end = end + '/' + stationList[end][0].getLine();
    }
    else {
        cout << second<<"의 호선을 선택해주세요. (숫자만 입력해주세요)" << endl;
        for (int i = 0; i < stationList[end].size(); i++) {
            cout << i + 1 << ". " << stationList[end][i].getLine() << '\n';
        }
        string whichLine;
        cin >> whichLine;
        end = end + '/' + stationList[end][stoi(whichLine) - 1].getLine();
    }

    future.get();
    vector<Station> route = shortTime.findRoute(start, end);

    if (optSubway == "1") {
        cout << endl << "걸린 시간 : " << d[end] << endl << endl;
    }
    else if (optSubway == "2") {
        for (int i = 0; i < route.size(); i++) {
            calcu = d[route[i].getName() + '/' + route[i].getLine()] * d[route[i].getName() + '/' + route[i].getLine()] + (d[route[route.size() - 1].getName() + '/' + route[route.size() - 1].getLine()] - d[route[i].getName() + '/' + route[i].getLine()]) * (d[route[route.size() - 1].getName() + '/' + route[route.size() - 1].getLine()] - d[route[i].getName() + '/' + route[i].getLine()]);
            if (calcu < minError) {
                minError = calcu;
                minNode = route[i];
            }
        }
        cout << "중간 지점 : " << minNode.getName() << endl << endl;
    }
    //-------------------
    
    
    //-------------------

    string stationName;
    string line1;
    int idx = 0;

    if (optSubway == "1")cout << "경로 : " << endl;
    else if (optSubway == "2")cout << "PersonA의 경로 : " << endl;

    for (; idx < route.size(); idx++) {
        stationName = route[idx].getName();
        

        if (idx!=0 && route[idx].getName() == route[idx-1].getName()) {
            line1 = route[idx].getLine();
            line2 = route[idx - 1].getLine();
            for (const auto& info : transinfo[stationName]) {
                if (info.first == line1 + ":" + line2 || info.first == line2 + ":" + line1) {
                    cout << "=" << info.second << "분 환승" << "=" << endl;
                    break;
                }
            }
        }

        cout << stationName << endl;

        if (optSubway == "2" && minNode.getName() == stationName) {
            break;
        }
    }
    if (optSubway == "2") {
        cout << endl<<"PersonB의 경로 : " << endl;
        int idx2 = route.size() - 1;
        for (; idx2 >= idx; idx2--) {
            stationName = route[idx2].getName();  // 변경된 부분

            if (idx2 != 0 && route[idx2].getName() == route[idx2 - 1].getName()) {
                line1 = route[idx2].getLine();
                line2 = route[idx2 - 1].getLine();
                for (const auto& info : transinfo[stationName]) {
                    if (info.first == line1 + ":" + line2 || info.first == line2 + ":" + line1) {
                        cout << "=" << info.second << "분 환승" << "=" << endl;
                        break;
                    }
                }
            }

            cout << stationName << endl;
        }
    }
    cout << endl;

    ////////////////

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
    // main 데이터 저장
    while (getline(in1, lineAround)) {
        vector<string> v = split(lineAround, ':');
        name = v[0];
        maindata[name] = Mainstation(name);
        maindata[name].setMain(v[1], v[2]);
    }
    in1.close();

    //볼거리
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

    //음식점
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
        cout << "최종 목적지에서의 볼거리를 원하시면 1번, 맛집을 원하시면 2번을 입력해주세요(끝내기 : 3번) : ";
        cin >> opt1;
        if (opt1 == 1) {
            maindata[name].printPlaces();
            cout << "원하는 장소를 선택해주세요 : ";
            cin.ignore();
            getline(cin, findplace);
            playdata[findplace].printPlacedata(findplace);
            plans.push_back(findplace);
        }
        else if (opt1 == 2) { //맛집에 대한 간략한 설명 보여주고 //선택하면 메뉴 구체적으로 보여주기
            for (auto& restaurant : restaurantData) {
                
                auto exist = find(restaurants.begin(), restaurants.end(), restaurant.first);
                if (exist != restaurants.end()) {
                    restaurant.second.printDetail();

                }
            }

            cout <<endl<< "원하는 식당을 선택해주세요. : ";
            cin.ignore();
            getline(cin, findfood);
            restaurantData[findfood].printFoodMenu();
            plans.push_back(findfood);
        }
        else if (opt1 == 3) {
            cout << "놀 거리 계획 : " << endl;
            int index = 1;
            for (string plan : plans) {
                cout << index++ << ". " << plan << endl;
            }
            break;
        }
        cout << endl;
    }

}