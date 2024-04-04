//
// Created by afdom on 16/03/2024.
//

#include "Data.h"
/**
 * function that retrieves the graph that contains all the information about the water supply management system - complexity O(1)
 * @return graph whose vertexes correspond to any of the structures (City, Reservoirs or Pumping stations) and the Edges to pipes
 */
Graph<string> Data::getSupply() {
    return this->supply;
}
/**
 * function that retrieves an unordered_map that stores key value pairs
 * of city codes and their corresponding Cities, allowing for constant lookup of Cities by their code - complexity O(1)
 * @return unordered_map that stores key value pairs
 * of city codes and their corresponding Cities
 */
unordered_map<string,City> Data::getCities() {
    return this->cities_;
}
/**
 * function that retrieves an unordered_map that stores key value pairs
 * of reservoir codes and their corresponding Reservoirs, allowing for constant lookup of Reservoirs by their code - complexity O(1)
 * @return unordered_map that stores key value pairs
 * of reservoir codes and their corresponding Reservoirs
 */
unordered_map<string,Reservoir> Data::getReservoirs() {
    return this->reservoirs_;
}
/**
 * function that retrieves an unordered_map that stores key value pairs
 * of Pumping Station codes and their corresponding Pumping Stations, allowing for constant lookup of Pumping Stations by their code - complexity O(1)
 * @return unordered_map that stores key value pairs
 * of Pumping Stations codes and their corresponding Pumping Stations
 */
unordered_map<string,Station> Data::getStations(){
    return this->stations_;
}
/**
 * function that parses the data from the Reservoir.csv file and uses it to build a graph accordingly
 * - complexity O(N) where N is the number of lines in the file
 */
void Data::parseReservoir() {
    ifstream reservoirs("../dataset/Reservoirs.csv");
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
/**
 * function that parses the data from the Stations.csv file and uses it to build a graph accordingly
 * - complexity O(N) where N is the number of lines in the file
 */
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
/**
 * function that parses the data from the Cities.csv file and uses it to build a graph accordingly
 * - complexity O(N) where N is the number of lines in the file
 */
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
/**
 * function that parses the data from the Pipes.csv file and uses it to build a graph accordingly
 * - complexity O(N) where N is the number of lines in the file
 */
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
