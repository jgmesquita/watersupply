//
// Created by afdom on 16/03/2024.
//


//Funçoes remocao de reservatorios, arestas e pipes dar output a cidades que ficaram ainda mais prejudicadas - done
//alterar a ultima fucnçao para dar os pipes que removidos afetariam cada cidade - done
//compor erros na ultima funçao - done
//compor falta de novas cidades afetadas - done
// pensar naquela funçao de aplicar so uma vex o maxflow <<<< pouco importante ???
//mostrar as cidades que passaram a estar afetadas depois de aplicar o algoritmo de balance - done
//fazer output para ficheiro - !!!!!
#include "Menu.h"
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <map>
#include <cstdio>
/**
 * Menu constructor - creates a Data object and parse the csv files
 */
Menu::Menu() {
    this->d = Data();
    d.parseReservoir();
    d.parseStation();
    d.parseCity();
    d.parsePipes();
}
/**
 * function that retrieves the graph that contains all the information about the water supply management system - complexity O(1)
 * @return graph whose vertexes correspond to any of the structures (City, Reservoirs or Pumping stations) and the Edges to pipes
 */
Graph<string> Menu::getSupy(){
    return d.getSupply();
}
/**
 * calculates the max amount of water that can reach a specif city - complexity O(VE^2), where V is the number of vertices and E is the number of edges in the graph s
 * @param s graph that contain all the information about the water supply management system
 * @param city_code code of the specific city that we want to calculate the max amount of water that reaching it
 * @return true if the the desired city exits, along with its max amount of water, and false otherwise
 */
bool Menu::Max_Amount_Water_specific(string city_code, Graph<string> s){
    std::ofstream file("../MaxFlowResult.txt", ios_base::app);

    if(d.getCities().find(city_code) == d.getCities().end()) return false;
    list<pair<City,double>> r = edmondsKarp(s);
    for (auto p : r) {
        if(p.first.getCodeCity() == city_code){
            cout << p.first.getNameCity() << " - " << p.first.getCodeCity() << " - " << p.second << " m^3 of water supplied!" << '\n';
            file << ' ' << p.first.getNameCity() << " - " << p.first.getCodeCity() << " - " << p.second << " m^3 of water supplied!" << '\n';
        }
    }
    file << " -------------------------------------------------------------" << '\n';
    file << '\n';
    file.close();
    return true;
}
/**
 * calculates the max amount of water that can reach any city - complexity O(VE^2), where V is the number of vertices and E is the number of edges in the graph s
 * @param s graph that contain all the information about the water supply management system
 * @return it outputs to both a file and directly to the console the max amount of water for each city
 */
void Menu::Max_Amount_Water(Graph<string> s) {
    std::ofstream file("../MaxFlowResult.txt", ios_base::app);
    list<pair<City,double>> r = edmondsKarp(s);
    double total = 0;
    for (auto p : r) {
        cout << p.first.getNameCity() << " - " << p.first.getCodeCity() << " - " << p.second << " m^3 of water supplied!" << '\n';
        file << ' ' << p.first.getNameCity() << " - " << p.first.getCodeCity() << " - " << p.second << " m^3 of water supplied!" << '\n';
        total += p.second;

    }
    cout << "The maxflow for the virtual super sink is: " << total << '\n';
    file << " The maxflow for the virtual super sink is: " << total << '\n';
    file << " -------------------------------------------------------------" << '\n';
    file << '\n';
    file.close();

}

/**
 * auxiliary function to the edmondsKarp algorithm that checks if a vertex has been visited and if the residual capacity of an edge is not null- complexity O(1)
 * @param q queue of the vertexes that belong to the augmenting path
 * @param w vertex of an augmenting path that is verified if it has not been visited
 * @param residual is the edge residual capacity
 * @param e edge of a augmenting path
 * @return
 */
void Menu::testAndVisit(std::queue< Vertex<string>*> &q, Edge<string> *e, Vertex<string> *w, double residual) {
// Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
// Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}
/**
 * auxiliary function to the edmondsKarp algorithm that finds the shortest unvisited augmenting path of the graph g- complexity O(V + E), where V is the number of vertices and E is the number of edges in the graph g
 * @param g graph that contain all the information about the water supply management system
 * @param s is the virtual super source of the graph
 * @param t is the virtual super sink of the graph
 * @return true if it finds a new shortest path and false otherwise
 */
bool Menu::findAugmentingPath(Graph<string> *g, Vertex<string> *s, Vertex<string> *t) {
// Mark all vertices as not visited
    for(auto v : g->getVertexSet()) {
        v->setVisited(false);
    }
// Mark the source vertex as visited and enqueue it
    s->setVisited(true);
    std::queue<Vertex<string> *> q;
    q.push(s);
// BFS to find an augmenting path
    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
// Process outgoing edges
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
// Process incoming edges
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
// Return true if s path to the target is found, false otherwise
    return t->isVisited();
}
/**
 * auxiliary function to the edmondsKarp algorithm that finds the minimum residual capacity along an augmenting path- complexity O(V), where V is the number of vertices in the graph g
 * @param s is the virtual super source of the graph
 * @param t is the virtual super sink of the graph
 * @return the minimum residual capacity along an augmenting path
 */
double Menu::findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t) {
    double f = INF;
// Traverse the augmenting path to find the minimum residual capacity
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
// Return the minimum residual capacity
    return f;
}
/**
 * auxiliary function to the edmondsKarp algorithm that updates the flow along an augmenting path- complexity O(V), where V is the number of vertices int the graph g
 * @param s is the virtual super source of the graph
 * @param t is the virtual super sink of the graph
 * @param f is the minimum residual capacity along an augmenting path used to update the flow of the edges that compose the path
 * @return
 */
void Menu::augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f) {
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

/**
 * function that applies the edmondsKarp algorithm to system graph in order to find the flow that passes/gets to the different
 * structures of the water supply system- complexity O(VE^2), where V is the number of vertices and E is the number of edges in the graph g
 * @param g graph that contain all the information about the water supply management system
 * @return a list of pairs of each city and its corresponding supply of water
 */
list<pair<City,double>> Menu::edmondsKarp(Graph<string> g) {
    list<pair<City,double>> r;
    string super_source = "SS";
    string super_target = "ST";
    g.addVertex(super_source);
    g.addVertex(super_target);
    for(Vertex<string>* v : g.getVertexSet()){
        if(v->getInfo()[0] == 'R')
            g.addEdge(super_source,v->getInfo(),d.getReservoirs()[v->getInfo()].getMaxDelivery());
    }
    //create s super sink
    for(Vertex<string>* v : g.getVertexSet()){
        if(v->getInfo()[0] == 'C') g.addEdge(v->getInfo(),super_target,d.getCities()[v->getInfo()].getDemand());
    }
    Vertex<string>* s = g.findVertex(super_source);
    Vertex<string>* t = g.findVertex(super_target);


    for (auto v : g.getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
    while( findAugmentingPath(&g, s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
    for(Vertex<string>* v : g.getVertexSet()){
        if(v->getInfo()[0] == 'C'){
            double value = 0.0;
            for(auto e : v->getIncoming()){
                value += e->getFlow();
            }
            City temp = d.getCities()[v->getInfo()];
            r.push_back(make_pair(temp,value));

        }
    }

    for(Vertex<string>* v : g.getVertexSet()){
        if(v->getInfo()[0] == 'R')
            g.removeEdge(super_source,v->getInfo());
    }
    //create s super sink
    for(Vertex<string>* v : g.getVertexSet()){
        if(v->getInfo()[0] == 'C') g.removeEdge(v->getInfo(),super_target);
    }
    g.removeVertex(super_source);
    g.removeVertex(super_target);

    return r;
}


/**
 * function that balances the load across the system based on capacity scaling, it is similar to the EdmondsKarp algorithm, however it finds the paths on a subgraph that contains only edges
 * with a capacity greater or equal to the current edge max capacity. This max edge capacity initially corresponds to the maximum edge capacity of the graph but it will decrease by half
 * after every iteration. It ends when there are no more paths between the super source SS
 * and the super sink ST. Basically this function finds the augmenting paths and calculates the network flow in descending order of edge capacity - complexity O((V + E) * log(delta)), where V is the number of vertices, E is the number of edges, and delta is the initial value of the delta parameter.
 * @param a graph that contain all the information about the water supply management system
 * @return
 */
void Menu::Balancing_func(Graph<std::string> g) {
    double delta = 14000;
    list<pair<City,double>> r;
    string super_source = "SS";
    string super_target = "ST";
    g.addVertex(super_source);
    g.addVertex(super_target);
    for(Vertex<string>* v : g.getVertexSet()){
        if(v->getInfo()[0] == 'R')
            g.addEdge(super_source,v->getInfo(),d.getReservoirs()[v->getInfo()].getMaxDelivery());
    }
    //create s super sink
    for(Vertex<string>* v : g.getVertexSet()){
        if(v->getInfo()[0] == 'C') g.addEdge(v->getInfo(),super_target,d.getCities()[v->getInfo()].getDemand());
    }
    Vertex<string>* s = g.findVertex(super_source);
    Vertex<string>* t = g.findVertex(super_target);

    for (auto v : g.getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
    unordered_map<Edge<string>*,double> restore_weights;

    while(delta >= 1) {
        for(auto v : g.getVertexSet()){
            for(auto e : v->getAdj()){
                if(e->getWeight() < delta){
                    restore_weights[e] = e->getWeight();
                    e->setWeight(0);
                }
            }
        }
        while (findAugmentingPath(&g, s, t)) {
            double f = findMinResidualAlongPath(s, t);
            augmentFlowAlongPath(s, t, f);
        }

        for(auto p : restore_weights){
            p.first->setWeight(p.second);
        }

        delta /= 2;

    }


    for(Vertex<string>* v : g.getVertexSet()){
        if(v->getInfo()[0] == 'R')
            g.removeEdge(super_source,v->getInfo());
    }
    //create s super sink
    for(Vertex<string>* v : g.getVertexSet()){
        if(v->getInfo()[0] == 'C') g.removeEdge(v->getInfo(),super_target);
    }
    g.removeVertex(super_source);
    g.removeVertex(super_target);

}




/**
 * function that retrieves the cities where there is a deficit in its water demand - complexity O(VE^2), where V is the number of vertices and E is the number of edges in the graph a
 * @param a graph that contain all the information about the water supply management system
 * @return a list of pairs of each city where there is a deficit in its water demand and its corresponding water deficit
 */
list<pair<City,double>> Menu::Meet_Costumer_needs(const Graph<string> a){
    list<pair<City,double>> result;

    list<pair<City,double>> r = edmondsKarp(a);

    for(auto p : r){
        if(p.second < p.first.getDemand()){
            result.push_back(make_pair(p.first,(p.first.getDemand() - p.second)));
        }
    }
    return result;
}

/**
 * function that calculates the maximum difference, the variance and the average of the difference between the capacity and the flow that passes through each pipe,
 * before and after applying a balancing function. - complexity O((V + E) * log(delta)), where V is the number of vertices, E is the number of edges, and delta is the initial value of the delta parameter
 * this time complexity corresponds to the balancing function, which is responsible for most of the time consumption and work of this particular function
 * @param s graph that contain all the information about the water supply management system
 * @return it outputs directly to the console the before mentioned metrics before and after applying the balancing algorithm
 */
void Menu::Balance_Load(Graph<string> s) {
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    set<string> cities_affected;
    for(auto p : l) cities_affected.insert(p.first.getCodeCity());
    unordered_map<string,double> temp;
    for(auto p : l) temp[p.first.getCodeCity()] = (p.first.getDemand() - p.second);
    list<pair<City,double>> result;
    bool bidirectional = false;
    result = edmondsKarp(s);
    double counter = 0.0;
    double average = 0, variance = 0, maxdiff=0;
    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            if(!e->isSelected()) {
                if (e->getReverse() == nullptr) { //if not bidirectional
                    if (e->getWeight() - e->getFlow() > maxdiff) maxdiff = (e->getWeight() - e->getFlow());
                    average += (e->getWeight() - e->getFlow());
                    e->setSelected(true);
                } else { //if is bidirectional
                    if (e->getFlow() > 0){
                        if (e->getWeight() - e->getFlow() > maxdiff) maxdiff = (e->getWeight() - e->getFlow());
                        average += (e->getWeight() - e->getFlow());
                    }
                    else {
                        if (e->getReverse()->getWeight() - e->getReverse()->getFlow() > maxdiff) maxdiff = (e->getReverse()->getWeight() - e->getReverse()->getFlow());
                        average += (e->getReverse()->getWeight() - e->getReverse()->getFlow());
                    }
                    e->setSelected(true);
                    e->getReverse()->setSelected(true);
                }
                counter++;
            }
        }
    }
    average /= counter;
    for(auto v : s.getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setSelected(false);
        }
    }

    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            if(!e->isSelected()) {
                if (e->getReverse() == nullptr) { //if not bidirectional
                    variance += pow((e->getWeight() - e->getFlow()) - average,2);
                    e->setSelected(true);
                } else { //if is bidirectional
                    if (e->getFlow() > 0){
                        variance += pow((e->getWeight() - e->getFlow()) - average,2);
                    }
                    else {
                        variance += pow((e->getReverse()->getWeight() - e->getReverse()->getFlow()) - average,2);
                    }
                    e->setSelected(true);
                    e->getReverse()->setSelected(true);
                }
            }

        }
    }
    variance /= (counter - 1);
    for(auto v : s.getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setSelected(false);
        }
    }
    cout << "The inicial metrics are: \n";
    cout << "Average:" << fixed << setprecision(2) << average << ' ' << "Variance:"  << fixed << setprecision(2) << variance << ' ' << "Max-Difference:" << maxdiff << '\n';

    Balancing_func(s);
    //final metrics
    variance = 0;
    average = 0;
    maxdiff = 0;
    counter = 0;
    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            if(!e->isSelected()) {
                if (e->getReverse() == nullptr) { //if not bidirectional
                    if (e->getWeight() - e->getFlow() > maxdiff) maxdiff = (e->getWeight() - e->getFlow());
                    average += (e->getWeight() - e->getFlow());
                    e->setSelected(true);
                } else { //if is bidirectional
                    if (e->getFlow() > 0){
                        if (e->getWeight() - e->getFlow() > maxdiff) maxdiff = (e->getWeight() - e->getFlow());
                        average += (e->getWeight() - e->getFlow());
                    }
                    else {
                        if (e->getReverse()->getWeight() - e->getReverse()->getFlow() > maxdiff) maxdiff = (e->getReverse()->getWeight() - e->getReverse()->getFlow());
                        average += (e->getReverse()->getWeight() - e->getReverse()->getFlow());
                    }
                    e->setSelected(true);
                    e->getReverse()->setSelected(true);
                }
                counter++;
            }
        }
    }
    average /= counter;
    for(auto v : s.getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setSelected(false);
        }
    }

    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            if(!e->isSelected()) {
                if (e->getReverse() == nullptr) { //if not bidirectional
                    variance += pow((e->getWeight() - e->getFlow()) - average,2);
                    e->setSelected(true);
                } else { //if is bidirectional
                    if (e->getFlow() > 0){
                        variance += pow((e->getWeight() - e->getFlow()) - average,2);
                    }
                    else {
                        variance += pow((e->getReverse()->getWeight() - e->getReverse()->getFlow()) - average,2);
                    }
                    e->setSelected(true);
                    e->getReverse()->setSelected(true);
                }
            }

        }
    }
    variance /= (counter - 1);
    for(auto v : s.getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setSelected(false);
        }
    }
    cout << "The final metrics are: \n";
    cout << "Average:" << fixed << setprecision(2) << average << ' ' << "Variance:"  << fixed << setprecision(2) << variance << ' ' << "Max-Difference:" << maxdiff << '\n';

}
/**
 * function that removes a specific reservoir from the system and verifies which cities have their supply affected - complexity O(VE^2 + f(n)), where V is the number of vertices, E is the number of edges in the graph s and f(n) the time complexity of the Meet_Costumer_needs() function
 * @param s graph that contain all the information about the water supply management system
 * @param reservoir_code code of the specific reservoir that will be removed from the system
 * @return true if the given reservoir exists and it outputs directly to the console the cities whose supply is affected by this removal and false otherwise
 */

bool Menu::Remove_Water_Reservoir(std::string reservoir_code,Graph<string> s) {
    if(d.getReservoirs().find(reservoir_code) == d.getReservoirs().end()) return false;
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    set<string> cities_affected;
    for(auto p : l) cities_affected.insert(p.first.getCodeCity());
    unordered_map<string,double> temp;
    for(auto p : l) temp[p.first.getCodeCity()] = (p.first.getDemand() - p.second);
    unordered_map<Edge<string>*,double> restore_weights;
    Vertex<string>* v = s.findVertex(reservoir_code);
    for(auto e: v->getIncoming()){
        restore_weights[e] = e->getWeight();
        e->setWeight(0.0);
    }

    for(auto e: v->getAdj()){
        restore_weights[e] = e->getWeight();
        e->setWeight(0.0);
    }

    list<pair<City,double>> r = edmondsKarp(s);
    //restore weights


    for(auto e: v->getIncoming()){
        e->setWeight(restore_weights[e]);
    }

    for (auto e: v->getAdj()) {
        e->setWeight(restore_weights[e]);
    }
    bool flag = false;
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) {
                flag = true;
                break;
            }
        }
    }
    if (!flag) {
        cout << "None of the cities were affected by the removal!\n";
        return true;
    }

    cout << "The affected cities by the removal of the Reservoi are:\n";
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << " m^3 of water in deficit!" << '\n';
        }
    }
    return true;
}

/**
 * function that removes a specific pumping station from the system and verifies which cities have their supply affected  - complexity O(VE^2 + f(n)), where V is the number of vertices, E is the number of edges in the graph b and f(n) the time complexity of the Meet_Costumer_needs() function
 * @param b graph that contain all the information about the water supply management system
 * @param station_code code of the specific pumping station that will be removed from the system
 * @return true if the given pumping station exists and it outputs directly to the console the cities whose supply is affected by this removal and false otherwise
 */
bool Menu::Maintenance_Station(string station_code,const Graph<string> b){
    if(d.getStations().find(station_code) == d.getStations().end()) return false;
    set<string> cities_affected;
    list<pair<City,double>> l = Meet_Costumer_needs(b);
    unordered_map<string,double> temp;
    for(auto p : l) cities_affected.insert(p.first.getCodeCity());
    for(auto p : l) temp[p.first.getCodeCity()] = (p.first.getDemand() - p.second);
    unordered_map<Edge<string>*,double> restore_weights;

    auto v = b.findVertex(station_code);


    for(auto e: v->getIncoming()){
        restore_weights[e] = e->getWeight();
        e->setWeight(0.0);
    }

    for(auto e: v->getAdj()){
        restore_weights[e] = e->getWeight();
        e->setWeight(0.0);
    }

    list<pair<City,double>> r = edmondsKarp(b);
    //restore weights
    for(auto e: v->getIncoming()){
        e->setWeight(restore_weights[e]);
    }

    for(auto e: v->getAdj()){
        e->setWeight(restore_weights[e]);
    }

    bool flag = false;
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) {
                flag = true;
                break;
            }
        }
    }
    if (!flag) {
        cout << "None of the cities were affected by the removal!\n";
        return true;
    }
    cout << "The affected cities by the removal of the Station are:\n";
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << " m^3 of water in deficit!" << '\n';

        }
    }

    return true;
}
/**
 * function that checks which of the pumping stations can be removed without having any effect on the system -  complexity O(n(VE^2 + f(n))), where V is the number of vertices, E is the number of edges in the graph s, f(n) the time complexity of the Meet_Costumer_needs() function and n the number of stations to iterate
 * @param s graph that contain all the information about the water supply management system
 * @return a vector of the Pumping stations codes whose removal doesnt affect the system delivery capacity
 */
vector<string> Menu::Remove_Station_noeffect(Graph<string> s){
    vector<string> caixa;
    for(auto r : d.getStations()) {
        list<pair<City, double>> l = Meet_Costumer_needs(s);
        set<string> cities_affected;
        unordered_map<string,double> temp;
        for(auto p : l) temp[p.first.getCodeCity()] = (p.first.getDemand() - p.second);
        for (auto p: l) cities_affected.insert(p.first.getCodeCity());
        unordered_map<Edge<string> *, double> restore_weights;
        Vertex<string> *v = s.findVertex(r.first);
        for (auto e: v->getIncoming()) {
            restore_weights[e] = e->getWeight();
            e->setWeight(0.0);
        }

        for (auto e: v->getAdj()) {
            restore_weights[e] = e->getWeight();
            e->setWeight(0.0);
        }

        list<pair<City, double>> r1 = edmondsKarp(s);
        //restore weights


        for (auto e: v->getIncoming()) {
            e->setWeight(restore_weights[e]);
        }

        for (auto e: v->getAdj()) {
            e->setWeight(restore_weights[e]);
        }

        bool flag = false;
        for(auto p : r1){
            if(p.second < p.first.getDemand()) {
                if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)){
                    flag = true;
                    break;
                }
            }
        }
        if(!flag) caixa.push_back(r.first);
    }
    return caixa;
}
/**
 * function that restores the capacity of all the removed pipes - complexity O(VE), where V is the number of vertices, E is the number of edges in the graph d
 * @param d graph that contain all the information about the water supply management system
 * @param restore_weights an unordered_map that stores the key value pairs of the removed edges and their corresponding initial capacity
 * @return
 */
void Menu::restore_capacities(Graph<string> d,unordered_map<Edge<string>*,double> restore_weights){
    for(auto v : d.getVertexSet()){
        for(auto e : v->getAdj()){
            if(restore_weights.find(e) != restore_weights.end()){
                e->setWeight(restore_weights[e]);
            }
        }
    }
}

/**
 * function that removes a multiple pipes from the system and verifies which cities have their supply affected - complexity O(VE^2 + f(n)), where V is the number of vertices, E is the number of edges in the graph s and f(n) the time complexity of the Meet_Costumer_needs() function
 * @param s graph that contain all the information about the water supply management system
 * @param t set that contains the multiple pipes to remove, each pair represents the source and target vertexes of each pipe
 * @return true if all the given pipes exist and it outputs directly to the console the cities whose supply is affected by this removal and false otherwise
 */
bool Menu::Remove_Pipe2(Graph<string> s,set<pair<string,string>> t) {
    unordered_map<string,double> temp;
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    for(auto p : l) temp[p.first.getCodeCity()] = (p.first.getDemand() - p.second);
    unordered_map<Edge<string> *, double> restore_weights;
    set<string> cities_affected;
    for (auto p: l) cities_affected.insert(p.first.getCodeCity());
    for(auto final : t) {
        bool bidirectional = false;
        auto v_source = s.findVertex(final.first);
        auto v_target = s.findVertex(final.second);
        bool exits = false;
        if (v_source == nullptr || v_source == v_target || v_target == nullptr) {
            return false;
        }

        for (auto e: v_source->getAdj()) {
            if (e->getDest()->getInfo() == final.second) {
                exits = true;
                break;
            }
        }
        for (auto e: v_target->getAdj()) {
            if (e->getDest()->getInfo() == final.first) {
                bidirectional = true;
                break;
            }
        }


        if (!exits) return false;
        //Check for already existent problems with supply
        //Check for invalid source or target

        if (bidirectional) {
            for (auto e: v_source->getAdj()) {
                if (e->getDest()->getInfo() == final.second) {
                    restore_weights[e] = e->getWeight();
                    e->setWeight(0.0);
                    break;
                }
            }
            for (auto e: v_source->getIncoming()) {
                if (e->getOrig()->getInfo() == final.second) {
                    restore_weights[e] = e->getWeight();
                    e->setWeight(0.0);
                    break;
                }
            }

        } else {
            for (auto e: v_source->getAdj()) {
                if (e->getDest()->getInfo() == final.second) {
                    restore_weights[e] = e->getWeight();
                    e->setWeight(0.0);
                    break;
                }
            }
        }
        //Solve for EdmondKarp
    }
    list<pair<City, double>> r = edmondsKarp(s);
    //Evalute the context
    restore_capacities(s,restore_weights);
    bool flag = false;
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) {
                flag = true;
                break;
            }
        }
    }
    if (!flag) {
        cout << "None of the cities were affected by the removal!\n";
        return true;
    }
    //Print result
    cout << "The affected cities by the removal of the Pipe are:\n";
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << " m^3 of water in deficit!" << '\n';
        }
    }
    return true;
}
/**
 * function that removes a specific pipe from the system by checking first if it exists and if it is bidirectional or unidirectional pipe and verifies which cities have their supply affected - complexity O(VE^2 + f(n)), where V is the number of vertices, E is the number of edges in the graph s and f(n) the time complexity of the Meet_Costumer_needs() function
 * @param s graph that contain all the information about the water supply management system
 * @param source code of the source vertex of the specific pipe
 * @param target code of the target vertex of the specific pipe
 * @return true if the given pipe exists and it outputs directly to the console the cities whose supply is affected by this removal and false otherwise
 */
bool Menu::Remove_Pipe(Graph<string> s,std::string source, std::string target) {
    unordered_map<string,double> temp;
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    for(auto p : l) temp[p.first.getCodeCity()] = (p.first.getDemand() - p.second);
    bool bidirectional = false;
    auto v_source = s.findVertex(source);
    auto v_target = s.findVertex(target);
    bool exits = false;
    if (v_source == nullptr || v_source == v_target || v_target== nullptr) {
        return false;
    }

    for(auto e : v_source->getAdj()){
        if(e->getDest()->getInfo() == target){
            exits = true;
            break;
        }
    }
    for(auto e : v_target->getAdj()){
        if(e->getDest()->getInfo() == source){
            bidirectional = true;
            break;
        }
    }


    if(!exits) return false;
    //Check for already existent problems with supply
    set<string> cities_affected;
    for(auto p : l) cities_affected.insert(p.first.getCodeCity());
    //Check for invalid source or target
    unordered_map<Edge<string>*,double> restore_weights;
    if (bidirectional) {
        for(auto e : v_source->getAdj()){
            if(e->getDest()->getInfo() == target){
                restore_weights[e] = e->getWeight();
                e->setWeight(0.0);
                break;
            }
        }
        for(auto e : v_source->getIncoming()){
            if(e->getOrig()->getInfo() == target){
                restore_weights[e] = e->getWeight();
                e->setWeight(0.0);
                break;
            }
        }

    }
    else {
        for(auto e : v_source->getAdj()){
            if(e->getDest()->getInfo() == target){
                restore_weights[e] = e->getWeight();
                e->setWeight(0.0);
                break;
            }
        }
    }
    //Solve for EdmondKarp

    list<pair<City, double>> r = edmondsKarp(s);
    //Evalute the context
    if (bidirectional) {
        for(auto e : v_source->getAdj()){
            if(e->getDest()->getInfo() == target){
                e->setWeight(restore_weights[e]);
                break;
            }
        }
        for(auto e : v_source->getIncoming()){
            if(e->getOrig()->getInfo() == target){
                e->setWeight(restore_weights[e]);
                break;
            }
        }

    }
    else {
        for(auto e : v_source->getAdj()){
            if(e->getDest()->getInfo() == target){
                e->setWeight(restore_weights[e]);
                break;
            }
        }
    }
    bool flag = false;
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) {
                flag = true;
                break;
            }
        }
    }
    if (!flag) {
        cout << "None of the cities were affected by the removal!\n";
        return true;
    }
    //Print result
    cout << "The affected cities by the removal of the Pipe are:\n";
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << " m^3 of water in deficit!" << '\n';
        }
    }
    return true;
}
/**
 * function that checks for each city the pipes that when removed have an effect on its water supply -  complexity O(VE(VE^2 + f(n))), where V is the number of vertices, E is the number of edges in the graph s, f(n) the time complexity of the Meet_Costumer_needs() function and n the number of stations to iterate
 * @param s graph that contain all the information about the water supply management system
 * @return it outputs directly to the console for each city the pipes that when removed cause an effect on the its supply.
 */
void Menu::Critical_Pipe_allCities(Graph<std::string> s) {
    map<string, vector<string>> all_cities;
    unordered_map<string,double> temp;
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    set<string> cities_affected;

    for(auto p : l) cities_affected.insert(p.first.getCodeCity());
    for(auto p : l) temp[p.first.getCodeCity()] = (p.first.getDemand() - p.second);

    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            if(!e->isSelected()) {
                bool bidirectional = false;
                if (e->getReverse() != nullptr) bidirectional = true;
                unordered_map < Edge < string > *, double > restore_weights;
                if (bidirectional) {

                    restore_weights[e] = e->getWeight();
                    e->setWeight(0.0);


                    restore_weights[e->getReverse()] = e->getReverse()->getWeight();
                    e->getReverse()->setWeight(0.0);



                } else {
                    restore_weights[e] = e->getWeight();
                    e->setWeight(0.0);
                }
                //Solve for EdmondKarp
                list <pair<City, double>> r = edmondsKarp(s);
                //Evalute the context

                if (bidirectional) {
                    e->setWeight(restore_weights[e]);
                    e->getReverse()->setWeight(restore_weights[e->getReverse()]);


                    e->setSelected(true);
                    e->getReverse()->setSelected(true);
                } else {
                    e->setWeight(restore_weights[e]);
                    e->setSelected(true);
                }
                //Print result
                for (auto p: r) {
                    if (p.second < p.first.getDemand()) {
                        if (cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) {
                            all_cities[p.first.getNameCity()].push_back(v->getInfo() + "-->" + e->getDest()->getInfo());
                        } else if (temp[p.first.getCodeCity()] > p.second) {
                            all_cities[p.first.getNameCity()].push_back(v->getInfo() + "-->" + e->getDest()->getInfo());

                        }

                    }
                }

            }
        }
    }

    cout << "The critical pipes for each city are: " << endl;
    for (auto it : all_cities) {
        cout << it.first << ":" << endl;
        for (auto it2 : it.second) {
            cout << it2 << endl;
        }
        cout << endl;
    }
}
/**
 * function that checks for a desired city the pipes that when removed have an effect on its water supply -  complexity O(VE(VE^2 + f(n))), where V is the number of vertices, E is the number of edges in the graph s, f(n) the time complexity of the Meet_Costumer_needs() function and n the number of stations to iterate
 * @param s graph that contain all the information about the water supply management system
 * @param city_code code of the specific city that we want to calculate the critical pipes
 * @return true if the the desired city exits, along with its critical pipes, and false otherwise
 */
bool Menu::Critical_Pipe_City(Graph<string> s,string city_code){
    if(d.getCities().find(city_code) == d.getCities().end()) return false;
    vector<string> all_cities;
    unordered_map<string,double> temp;
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    set<string> cities_affected;

    for(auto p : l) cities_affected.insert(p.first.getCodeCity());
    for(auto p : l) temp[p.first.getCodeCity()] = (p.first.getDemand() - p.second);

    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            if(!e->isSelected()) {
                bool bidirectional = false;
                if (e->getReverse() != nullptr) bidirectional = true;
                unordered_map < Edge < string > *, double > restore_weights;
                if (bidirectional) {

                    restore_weights[e] = e->getWeight();
                    e->setWeight(0.0);

                    restore_weights[e->getReverse()] = e->getReverse()->getWeight();
                    e->getReverse()->setWeight(0.0);

                } else {
                    restore_weights[e] = e->getWeight();
                    e->setWeight(0.0);
                }
                //Solve for EdmondKarp
                list <pair<City, double>> r = edmondsKarp(s);
                //Evalute the context

                if (bidirectional) {
                    e->setWeight(restore_weights[e]);

                    e->getReverse()->setWeight(restore_weights[e->getReverse()]);
                    e->setSelected(true);
                    e->getReverse()->setSelected(true);

                } else {
                    e->setWeight(restore_weights[e]);
                    e->setSelected(true);
                }
                //Print result
                for (auto p: r) {
                    if (p.second < p.first.getDemand() && p.first.getCodeCity() == city_code) {
                        if (cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) {
                            all_cities.push_back(v->getInfo() + "-->" + e->getDest()->getInfo());
                        } else if (temp[p.first.getCodeCity()] > p.second) {
                            all_cities.push_back(v->getInfo() + "-->" + e->getDest()->getInfo());

                        }
                        break;

                    }
                }
            }
        }
    }

    cout << "The critical pipes for " << d.getCities()[city_code].getNameCity() <<" are: " << endl;
    for (auto it : all_cities) {
        cout << it << endl;
    }
    return true;
}
