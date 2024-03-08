#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "Graph.h"

using namespace std;

class Reservoir {
private:
    string name_;
    string municipality_;
    string id_;
    string maxDelivery_;
    string code_;
public:
    Reservoir(string name, string municipality, string id, string maxDelivery, string code) {
        this->name_ = name;
        this->municipality_ = municipality;
        this->id_ = id;
        this->maxDelivery_ = maxDelivery;
        this->code_ = code;
    }
    string getName() {
        return this->name_;
    }
    string getMunicipality() {
        return this->municipality_;
    }
    string getIdReservoir() {
        return this->id_;
    }
    string getCodeReservoir() {
        return this->code_;
    }
    string getMaxDelivery() {
        return this->maxDelivery_;
    }
    bool operator < (const Reservoir &a2) const {
        return this->id_ < a2.id_;
    }
};

class Station {
private:
    string id_;
    string code_;
public:
    Station(string id, string code) {
        this->id_ = id;
        this->code_ = code;
    }
    string getIdStation() {
        return this->id_;
    }
    string getCodeStation() {
        return this->code_;
    }
};

class City {
private:
    string name_;
    string id_;
    string code_;
    string demand_;
    string population_;
public:
    City(string name, string id, string code, string demand, string population) {
        this->name_ = name;
        this->id_ = id;
        this->demand_ = demand;
        this->population_ = population;
        this->code_ = code;
    }
    string getNameCity() {
        return this->name_;
    }
    string getIdCity() {
        return this->id_;
    }
    string getDemand() {
        return this->demand_;
    }
    string getPopulation() {
        return this->population_;
    }
    string getCodeCity() {
        return this->code_;
    }
};

void parseReservoir(Graph<string> &supply, vector<Reservoir> &reservoirs) {
    ifstream inputFile("../dataset/Reservoir.csv");
    string line;
    bool isFirstLine = true;
    while (getline(inputFile, line)) {
        vector<string> temp;
        istringstream stream(line);
        string in;
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        while (getline(stream, in, ',')) {
            temp.push_back(in);
        }
        string name = temp[0];
        string municipality = temp[1];
        string id = temp[2];
        string code = temp[3];
        string maxDelivery = temp[4];
        Reservoir r = Reservoir(name, municipality, id, maxDelivery, code);
        reservoirs.push_back(r);
        supply.addVertex(code);
    }
}

void parseStation(Graph<string> &supply, vector<Station> &stations) {
    ifstream inputFile("../dataset/Stations.csv");
    string line;
    bool isFirstLine = true;
    while (getline(inputFile, line)) {
        vector<string> temp;
        istringstream stream(line);
        string in;
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        while (getline(stream, in, ',')) {
            temp.push_back(in);
        }
        string id = temp[0];
        string code = temp[1];
        Station s = Station(id, code);
        stations.push_back(s);
        supply.addVertex(code);
    }
}

void parseCity(Graph<string> &supply, vector<City> &cities) {
    ifstream inputFile("../dataset/Cities.csv");
    string line;
    bool isFirstLine = true;
    while (getline(inputFile, line)) {
        vector<string> temp;
        istringstream stream(line);
        string in;
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        while (getline(stream, in, ',')) {
            temp.push_back(in);
        }
        string name = temp[0];
        string id = temp[1];
        string code = temp[2];
        string demand = temp[3];
        string population = temp[4];
        City c = City(name, id, code, demand, population);
        cities.push_back(c);
        supply.addVertex(code);
    }
}

void parsePipes(Graph<string> &supply) {
    ifstream inputFile("../dataset/Pipes.csv");
    string line;
    bool isFirstLine = true;
    while (getline(inputFile, line)) {
        vector<string> temp;
        istringstream stream(line);
        string in;
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        while (getline(stream, in, ',')) {
            temp.push_back(in);
        }
        string codeA = temp[0];
        string codeB = temp[1];
        string capacity = temp[2];
        string direction = temp[3];
        if (direction == "0") {
            supply.addBidirectionalEdge(codeA, codeB, stod(capacity));
        }
        else {
            supply.addEdge(codeA, codeB, stod(capacity));
        }
    }
}

void menu() {
    cout << "#########################################################" << endl;
    cout << "Choose of the following options:" << endl;
    cout << "[0] - Load data from files;" << endl;
    cout << "[1] - Maximum flow for a city;" << endl;
    cout << "[2] - Evaluate the network;" << endl;
    cout << "[3] - Balance the flow;" << endl;
    cout << "[4] - Evaluate a removal of a reservoir from the network;" << endl;
    cout << "[5] - Evaluate a removal a pipe from the network;" << endl;
    cout << "[6] - Evaluate a hypothetical failre from a pipe;" << endl;
    cout << "[7] - Exit." << endl;
    cout << "#########################################################" << endl;
}

void run() {
    Graph<string> supply;
    vector<Reservoir> reservoirs;
    vector<Station> stations;
    vector<City> cities;
    menu();
    string input;
    while(cin >> input) {
        if (input == "0") {
            parseReservoir(supply, reservoirs);
            parseStation(supply, stations);
            parseCity(supply, cities);
            parsePipes(supply);
            cout << "The data was loaded successfully!" << endl;
            menu();
        }
        if (input == "7") {
            cout << "The program is going to end!" << endl;
            break;
        }
    }
}

int main() {
    run();
    return 0;
}
