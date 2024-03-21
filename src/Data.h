//
// Created by afdom on 16/03/2024.
//

#ifndef PROJETO_DA_DATA_H
#define PROJETO_DA_DATA_H
#include "Graph.h"
#include "string"
#include <iostream>
#include "sstream"
#include "fstream"
#include "City.h"
#include "Station.h"
#include "Reservoir.h"
#include "unordered_map"
#include "list"
#include "set"
using namespace std;
class Data {
private:
    Graph<string> supply;
    unordered_map<string,Reservoir> reservoirs_;
    unordered_map<string,City> cities_;
    unordered_map<string,Station> stations_;

public:
    void parseReservoir();
    void parseCity();
    void parseStation();
    void parsePipes();
    Graph<string> getSupply();
    unordered_map<string,City> getCities();
    unordered_map<string,Reservoir> getReservoiers();
    unordered_map<string,Station> getStations();

};


#endif //PROJETO_DA_DATA_H
