//
// Created by afdom on 16/03/2024.
//

#include "Data.h"

Graph<string> Data::getSupply() {
    return this->supply;
}
unordered_map<string,City> Data::getCities() {
    return this->cities_;
}
unordered_map<string,Reservoir> Data::getReservoiers() {
    return this->reservoirs_;
}
unordered_map<string,Station> Data::getStations(){
    return this->stations_;
}

 void Data::parseReservoir() {
    ifstream reservoirs("../dataset/Reservoir.csv");
    string line;
    getline(reservoirs,line); //read and ignore first line
     while (getline(reservoirs, line)) {
         string name, municipality, id, maxDelivery, code;
         istringstream iss(line);
         getline(iss, name, ',');
         getline(iss, municipality, ',');
         getline(iss, id, ',');
         getline(iss, code, ',');
         getline(iss, maxDelivery);
         Reservoir r = Reservoir(name,municipality,id, stod(maxDelivery),code);
         reservoirs_[code] = r;
         supply.addVertex(code);
     }

}

void Data::parseStation() {
    ifstream stations("../dataset/Stations.csv");
    string line;
    getline(stations,line); //read and ignore first line
    while (getline(stations, line)) {
        string ID,code;
        istringstream iss(line);
        getline(iss, ID, ',');
        getline(iss, code);
        Station s = Station(ID,code);
        stations_[code] = s;
        supply.addVertex(code);
    }
}

void Data::parseCity() {
    ifstream cities("../dataset/Cities.csv");
    string line;
    getline(cities,line); //read and ignore first line
    while (getline(cities, line)) {
        string name, id, code, demand, population;
        istringstream iss(line);
        getline(iss, name, ',');
        getline(iss, id, ',');
        getline(iss, code, ',');
        getline(iss, demand, ',');
        getline(iss, population);
        City c = City(name,id,code, stod(demand),population);
        cities_[code] = c;
        supply.addVertex(code);
    }

}

void Data::parsePipes() {
    ifstream pipes("../dataset/Pipes.csv");
    string line;
    getline(pipes,line); //read and ignore first line
    while (getline(pipes, line)) {
        string source, target, capacity, direction;
        istringstream iss(line);
        getline(iss, source, ',');
        getline(iss, target, ',');
        getline(iss, capacity, ',');
        getline(iss, direction);

        if(direction == "0"){
            supply.addBidirectionalEdge(source,target, stod(capacity));
        }
        else supply.addEdge(source,target, stod(capacity));
    }


}