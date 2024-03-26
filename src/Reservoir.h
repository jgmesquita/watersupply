//
// Created by afdom on 16/03/2024.
//

#ifndef PROJETO_DA_RESERVOIR_H
#define PROJETO_DA_RESERVOIR_H
#include "iostream"
#include "string"
using namespace std;
/**
 * @class City
 * @brief Portrays a Reservoir, identified it by name, municipality where it is located, id, its max_delivery(maximum delivery of water) and code.
 */

class Reservoir {
    private:
        string name_;
        string municipality_;
        string id_;
        double maxDelivery_;
        string code_;
    public:
        Reservoir() = default;
        Reservoir(string name, string municipality, string id, double maxDelivery, string code);
        string getName();
        string getMunicipality();
        string getIdReservoir();
        string getCodeReservoir();
        double getMaxDelivery();
        bool operator < (const Reservoir &a2) const {
            return this->id_ < a2.id_;
        }
};


#endif //PROJETO_DA_RESERVOIR_H
