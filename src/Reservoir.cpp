//
// Created by afdom on 16/03/2024.
//

#include "Reservoir.h"
Reservoir::Reservoir(std::string name, std::string municipality, std::string id, double maxDelivery,
                     std::string code) {
    this->name_ = name;
    this->municipality_ = municipality;
    this->id_ = id;
    this->maxDelivery_ = maxDelivery;
    this->code_ = code;
}
string Reservoir::getName() {
    return this->name_;
}
string Reservoir::getMunicipality() {
    return this->municipality_;
}
string Reservoir::getIdReservoir() {
    return this->id_;
}
string Reservoir::getCodeReservoir() {
    return this->code_;
}
double Reservoir::getMaxDelivery() {
    return this->maxDelivery_;
}
