#include "station.h"
#include <iostream>
#include <unordered_map>
#include <queue>
#include <stack>
using namespace std;

class ShortTime {
private:
    priority_queue<pair<double, string>>pq; // time, name
    unordered_map<string, Station> previous; // store the previous station
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
            double time = -pq.top().first; //Costs to the current node
            string keynow = pq.top().second; // current node
            pq.pop();

            if (d[keynow] < time) // If the node has already checked the shortest path, pass
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

    vector<Station> findRoute(string start, string end) //Insert the station name of the object.
    {
        //trace the route
        vector<Station> route;
        for (Station station = stations[end]; (station.getName() + '/' + station.getLine()) != start; station = previous[station.getName() + '/' + station.getLine()]) {
            route.insert(route.begin(), station);
        }
        route.insert(route.begin(), stations[start]);
        return route;
    }
};