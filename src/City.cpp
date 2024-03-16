//
// Created by afdom on 16/03/2024.
//

#include "City.h"

City::City(string name, string id, string code, double demand, string population) {
this->name_ = name;
this->id_ = id;
this->demand_ = demand;
this->population_ = population;
this->code_ = code;
}
string City::getNameCity() {
    return this->name_;
}
string City::getIdCity() {
    return this->id_;
}
double City::getDemand() {
    return this->demand_;
}
string City::getPopulation() {
    return this->population_;
}
string City::getCodeCity() {
    return this->code_;
}