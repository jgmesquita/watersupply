//
// Created by afdom on 16/03/2024.
//

#ifndef PROJETO_DA_MENU_H
#define PROJETO_DA_MENU_H
#include "Data.h"
/**
 * @class Menu
 * @brief Represents the user-friendly interface for the water supply management system.
 * It shows available functionalities and their corresponding outputs in a clear, organized,
 * and logical manner, facilitating seamless program utilization and straightforward result
 * interpretation.
 */
class Menu {
private:
    Data d;
public:
    Menu();
    void Max_Amount_Water(Graph<string> s);
    bool Max_Amount_Water_specific(string city_code,Graph<string> s);
    Graph<string> getSupy();
    list<pair<City,double>> Meet_Costumer_needs(const Graph<string> supply);
    void Balance_Load(Graph<string> s);
    bool Remove_Water_Reservoir(string reservoi_code,Graph<string> s);
    bool Maintenance_Station(string station_code,const Graph<string> a);
    vector<string> Remove_Station_noeffect(Graph<string> s);
    bool Remove_Pipe(Graph<string> s,std::string source, std::string target);
    void Critical_Pipe_allCities(Graph<string> s);
    bool Critical_Pipe_City(Graph<string> s,string city_code);
    list<pair<City,double>> edmondsKarp(Graph<string> g);
    void augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f);
    double findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t);
    bool findAugmentingPath(Graph<string> *g, Vertex<string> *s, Vertex<string> *t);
    void testAndVisit(std::queue< Vertex<string>*> &q, Edge<string> *e, Vertex<string> *w, double residual);


};
#endif //PROJETO_DA_MENU_H
