//
// Created by afdom on 16/03/2024.
//

#ifndef PROJETO_DA_CITY_H
#define PROJETO_DA_CITY_H
#include "iostream"
#include "string"

using namespace std;
/**
 * @class City
 * @brief Identifies the City class by its name, id, code, demand(rate of demand or need of delivery of water) and population (total population).
 */
class City {
    private:

        string name_;
        string id_;
        string code_;
        double demand_;
        string population_;
    public:
        City() = default;
        City(string name, string id, string code, double demand, string population);
        string getNameCity();
        string getIdCity();
        double getDemand();
        string getPopulation();
        string getCodeCity();
        bool operator == (const City &a2) const {
        return this->code_ == a2.code_;
    }
};


#endif //PROJETO_DA_CITY_H
