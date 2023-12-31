#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include "station.h"
#include <iostream>
using namespace std;

double minError = 1e9;
Station minNode;
double calcu;

unordered_map<string, double> shortTime2(string start, unordered_map<string, double>& d, unordered_map<string, Station>& stations) //객체의 역 이름 넣기
{
    priority_queue<pair<double, string>>pq; // time, name
    unordered_map<string, Station> previous; // store the previous station

    pq.push({ 0,start }); //start -> start,  id of the depart
    d[start] = 0;

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
    return d;
}


Station findMiddle(vector<string> personStart, vector<unordered_map<string, double>>& d, unordered_map<string, Station>& stations) {

    for (int i = 0; i < personStart.size(); i++) {
        d[i] = shortTime2(personStart[i], d[i], stations);
    }
    for (auto station : stations) {
        for (int i = 0; i < d.size(); i++) {
            for (int j = 0; j < d.size(); j++) {
                if (i == j) {
                    continue;
                }
                else {
                    calcu = 0;
                    calcu += (d[i][station.first] - d[j][station.first]) * (d[i][station.first] - d[j][station.first]);
                }
                if (minError > calcu) {
                    minError = calcu;
                    minNode = station.second;
                }
            }
        }

    }
    return minNode;
}