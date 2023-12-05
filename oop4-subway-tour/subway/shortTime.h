#include "station.h"
#include <iostream>
#include <unordered_map>
#include <queue>
#include <stack>
using namespace std;

class ShortTime {
private:
    priority_queue<pair<double, string>>pq; // 시간, 이름
    unordered_map<string, Station> previous; // 이전 역 저장
    unordered_map<string, double>& d;
    unordered_map<string, Station>& stations;
public:
    ShortTime(unordered_map<string, double>& distance, unordered_map<string, Station>& stationsInfo)
        : d(distance), stations(stationsInfo) {
    }
    void dijkstra(string start) {
        d[start] = 0;
        pq.push({ 0, start });

        while (!pq.empty()) {
            double time = -pq.top().first; //현재 노드까지의 비용
            string keynow = pq.top().second; // 현재 노드
            pq.pop();

            if (d[keynow] < time) // 이미 최단경로를 체크한 노드인 경우 패스
                continue;
            for (int i = 0; i < stations[keynow].getNeighbor().size(); i++) {
                double cost = time + stations[keynow].getNeighbor()[i].second.first;
                string next = stations[keynow].getNeighbor()[i].first.getName() + '/' + stations[keynow].getNeighbor()[i].first.getLine();

                if (cost < d[next]) {
                    d[next] = cost;
                    pq.push(make_pair(-cost, next));
                    previous[next] = stations[keynow];
                }
            }
        }
    }

    vector<Station> findRoute(string start, string end) //객체의 역 이름 넣기
    {
        //경로 추적
        vector<Station> route;
        for (Station station = stations[end]; (station.getName() + '/' + station.getLine()) != start; station = previous[station.getName() + '/' + station.getLine()]) {
            route.insert(route.begin(), station);
        }
        route.insert(route.begin(), stations[start]);
        return route;
    }
};