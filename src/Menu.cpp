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
    if(d.getCities().find(city_code) == d.getCities().end()) return false;
    list<pair<City,double>> r = edmondsKarp(s);
    for (auto p : r) {
        if(p.first.getCodeCity() == city_code)
            cout << p.first.getNameCity() << " - " << p.first.getCodeCity() << " - " << p.second << '\n';
    }
    return true;
}
/**
 * calculates the max amount of water that can reach any city - complexity O(VE^2), where V is the number of vertices and E is the number of edges in the graph s
 * @param s graph that contain all the information about the water supply management system
 * @return it outputs directly to the console the max amount of water for each city
 */
void Menu::Max_Amount_Water(Graph<string> s) {
    ofstream of;
    of.open("../../output.txt", ios::app);
    list<pair<City,double>> r = edmondsKarp(s);
    double total = 0;
    for (auto p : r) {
        cout << p.first.getNameCity() << " - " << p.first.getCodeCity() << " - " << p.second << '\n';
        of << p.first.getNameCity() << " - " << p.first.getCodeCity() << " - " << p.second << '\n';
        total += p.second;
        of.flush();
    }
    cout << "The maxflow for the virtual super sink is: " << total << '\n';
    of << "The maxflow for the virtual super sink is: " << total << '\n';
    of.close();

}


void Menu::testAndVisit(std::queue< Vertex<string>*> &q, Edge<string> *e, Vertex<string> *w, double residual) {
// Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
// Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

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
 * function that retrieves the cities where there is a deficit in its water demand - complexity O(VE^2), where V is the number of vertices and E is the number of edges in the graph s
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
 * before and after applying a balancing function. This balancing function calculates the average difference between the capacity and flow that
 * passes through the adjacent pipes of each vertex and tries distribute in a way that in each adjacent pipe the difference corresponds to
 * the average of the difference - complexity O(VE^2 + f(n)), where V is the number of vertices, E is the number of edges in the graph s and f(n) the time complexity of the Meet_Costumer_needs() function
 * @param s graph that contain all the information about the water supply management system
 * @return it outputs directly to the console the before mentioned metrics before and after applying the balancing
 * algorithm and the cities which water supply  ended up being affect by this balancing algorithm.
 */
void Menu::Balance_Load(Graph<string> s) {
    //pensar melhor nisto
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    set<string> cities_affected;
    for(auto p : l) cities_affected.insert(p.first.getCodeCity());
    unordered_map<string,double> temp;
    for(auto p : l) temp[p.first.getCodeCity()] = p.second;
    list<pair<City,double>> result;
    result = edmondsKarp(s);
    double counter = 0.0;
    double average = 0, variance = 0, maxdiff=0;
    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            if(e->getWeight() - e->getFlow() > maxdiff) maxdiff = (e->getWeight() - e->getFlow());
            average += (e->getWeight() - e->getFlow());
            counter++;
        }
    }
    average /= counter;
    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            variance += pow((e->getWeight() - e->getFlow()) - average,2);

        }
    }
    variance /= (counter - 1);
    cout << "The inicial metrics are: \n";
    cout << "Average:" << fixed << setprecision(2) << average << ' ' << "Variance:"  << fixed << setprecision(2) << variance << ' ' << "Max-Difference:" << maxdiff << '\n';
    //balancing functioon
    for(auto v : s.getVertexSet()) {
        double total_diff = 0.0;
        double n_edges = 0.0;
        for (auto e: v->getAdj()) {
            n_edges++;
            total_diff += (e->getWeight() - e->getFlow());
        }
        if (n_edges != 0) {
            total_diff /= n_edges;
            for (auto e: v->getAdj()) {
                if (e->getWeight() - e->getFlow() > total_diff)
                    e->setFlow(e->getFlow() + ((e->getWeight() - e->getFlow()) - total_diff));
                else if (e->getWeight() - e->getFlow() < total_diff)
                    e->setFlow(e->getFlow() - (total_diff - (e->getWeight() - e->getFlow())));
            }
        }

    }
    //final metrics
    variance = 0;
    average = 0;
    maxdiff = 0;

    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            if(e->getWeight() - e->getFlow() > maxdiff) maxdiff = (e->getWeight() - e->getFlow());
            average += (e->getWeight() - e->getFlow());
            counter++;
        }
    }
    average /= counter;
    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            variance += pow((e->getWeight() - e->getFlow()) - average,2);

        }
    }
    variance /= (counter - 1);
    cout << "The final metrics are: \n";
    cout << "Average:" << fixed << setprecision(2) << average << ' ' << "Variance:"  << fixed << setprecision(2) << variance << ' ' << "Max-Difference:" << maxdiff << '\n';
    list<pair<City,double>> r;
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C'){
            double value = 0.0;
            for(auto e : v->getIncoming()){
                value += e->getFlow();
            }
            City temp = d.getCities()[v->getInfo()];
            r.push_back(make_pair(temp,value));

        }
    }
    cout << "The affected cities by the balancing of the network are:\n";
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << '\n';
        }
    }

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
    for(auto p : l) temp[p.first.getCodeCity()] = p.second;
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
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << '\n';
        }
    }
    return true;
}

/**
 * function that removes a specific pumping station from the system and verifies which cities have their supply affected  - complexity O(VE^2 + f(n)), where V is the number of vertices, E is the number of edges in the graph s and f(n) the time complexity of the Meet_Costumer_needs() function
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
    for(auto p : l) temp[p.first.getCodeCity()] = p.second;
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
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << '\n';

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
        for(auto p : l) temp[p.first.getCodeCity()] = p.second;
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
 * function that removes a specific pipe from the system by checking first if it exists and if it is bidirectional or unidirectional pipe and verifies which cities have their supply affected - complexity O(VE^2 + f(n)), where V is the number of vertices, E is the number of edges in the graph s and f(n) the time complexity of the Meet_Costumer_needs() function
 * @param s graph that contain all the information about the water supply management system
 * @param source code of the source vertex of the specific pipe
 * @param target code of the target vertex of the specific pipe
 * @return true if the given pipe exists and it outputs directly to the console the cities whose supply is affected by this removal and false otherwise
 */
bool Menu::Remove_Pipe(Graph<string> s,std::string source, std::string target) {
    unordered_map<string,double> temp;
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    for(auto p : l) temp[p.first.getCodeCity()] = p.second;
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
        for(auto e : v_target->getAdj()){
            if(e->getDest()->getInfo() == target){
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
        for(auto e : v_target->getAdj()){
            if(e->getDest()->getInfo() == target){
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
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << '\n';
        }
    }
    return true;
}
/**
 * function that checks for each city the pipes that when removed have an effect on its water supply -  complexity O(VE(VE^2 + f(n))), where V is the number of vertices, E is the number of edges in the graph s, f(n) the time complexity of the Meet_Costumer_needs() function and n the number of stations to iterate
 * @param s graph that contain all the information about the water supply management system
 * @return it outputs directly to the console for each city the pipes that when removed cause an effect on the its supply.
 */
void Menu::Remove_Pipe2(Graph<string> s){
    map<string, vector<string>> all_cities{};
    unordered_map<string,double> temp;
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    for(auto p : l) temp[p.first.getCodeCity()] = p.second;
    for(auto v : s.getVertexSet()){
        for(auto e : v->getAdj()){
            bool bidirectional = false;
            for(auto p : e->getDest()->getAdj()){
                if(p->getDest()->getInfo() == v->getInfo()){
                    bidirectional = true;
                    break;
                }
            }
            list<pair<City, double>> l = Meet_Costumer_needs(s);
            set<string> cities_affected;
            for(auto p : l) cities_affected.insert(p.first.getCodeCity());
            //Check for invalid source or target
            unordered_map<Edge<string>*,double> restore_weights;
            if (bidirectional) {
                for(auto p : v->getAdj()){
                    if(p->getDest()->getInfo() == e->getDest()->getInfo()){
                        restore_weights[p] = p->getWeight();
                        p->setWeight(0.0);
                        break;
                    }
                }
                for(auto p : e->getDest()->getAdj()){
                    if(p->getDest()->getInfo() == e->getDest()->getInfo()){
                        restore_weights[p] = p->getWeight();
                        p->setWeight(0.0);
                        break;
                    }
                }

            }
            else {
                for(auto p : v->getAdj()){
                    if(p->getDest()->getInfo() == e->getDest()->getInfo()){
                        restore_weights[p] = p->getWeight();
                        p->setWeight(0.0);
                        break;
                    }
                }
            }
            //Solve for EdmondKarp

            list<pair<City, double>> r = edmondsKarp(s);
            //Evalute the context
            if (bidirectional) {
                for(auto p : v->getAdj()){
                    if(p->getDest()->getInfo() == e->getDest()->getInfo()){
                        p->setWeight(restore_weights[p]);
                        break;
                    }
                }
                for(auto p : e->getDest()->getAdj()){
                    if(p->getDest()->getInfo() == e->getDest()->getInfo()){
                        p->setWeight(restore_weights[p]);
                        break;
                    }
                }

            }
            else {
                for(auto p : v->getAdj()){
                    if(p->getDest()->getInfo() == e->getDest()->getInfo()){
                        p->setWeight(restore_weights[p]);
                        break;
                    }
                }
            }
            //Print result
            for(auto p : r){
                if(p.second < p.first.getDemand()) {
                    if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) {
                        all_cities[p.first.getNameCity()].push_back(v->getInfo() + "-->" + e->getDest()->getInfo());
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