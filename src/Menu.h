//
// Created by afdom on 16/03/2024.
//

#ifndef PROJETO_DA_MENU_H
#define PROJETO_DA_MENU_H
#include "Data.h"

class Menu {
private:
    Data d;
public:
    Menu();
    void Max_Amount_Water();
    bool Max_Amount_Water_specific(string city_code);
    list<pair<City,double>> Meet_Costumer_needs();
    void Balance_Load();
    bool Remove_Water_Reservoir(string reservoi_code);
    bool Maintenance_Station(string station_code);
    void edmondsKarp(Graph<string> *g, string source, string target);
    void augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f);
    double findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t);
    bool findAugmentingPath(Graph<string> *g, Vertex<string> *s, Vertex<string> *t);
    void testAndVisit(std::queue< Vertex<string>*> &q, Edge<string> *e, Vertex<string> *w, double residual);


};
#endif //PROJETO_DA_MENU_H
