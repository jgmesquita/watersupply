//
// Created by afdom on 16/03/2024.
//

#ifndef PROJETO_DA_STATION_H
#define PROJETO_DA_STATION_H
#include "iostream"
#include "string"

using namespace std;
/**
 * @class Station
 * @brief Identifies the Station class by its id and code.
 */
class Station {
private:
    string id_;
    string code_;
public:
    Station() = default;
    Station(string id, string code);
    string getIdStation();
    string getCodeStation();
};


#endif //PROJETO_DA_STATION_H
