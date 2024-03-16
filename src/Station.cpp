//
// Created by afdom on 16/03/2024.
//

#include "Station.h"

Station::Station(string id, string code) {
this->id_ = id;
this->code_ = code;
}
string Station::getIdStation() {
    return this->id_;
}
string Station::getCodeStation() {
    return this->code_;
}