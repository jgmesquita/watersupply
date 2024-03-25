//
// Created by afdom on 16/03/2024.
//


//Funçoes remocao de reservatorios, arestas e pipes dar output a cidades que ficaram ainda mais prejudicadas
//alterar a ultima fucnçao para dar os pipes que removidos afetariam cada cidade
//compor erros na ultima funçao
//compor falta de novas cidades afetadas
// pensar naquela funçao de aplicar so uma vex o maxflow <<<< pouco importante ???
//mostrar as cidades que passaram a estar afetadas depois de aplicar o algoritmo de balance
//fazer output para ficheiro
#include "Menu.h"
#include "cfloat"
#include <cmath>
#include <iomanip>
Menu::Menu() {
    this->d = Data();
    d.parseReservoir();
    d.parseStation();
    d.parseCity();
    d.parsePipes();
}
Graph<string> Menu::getSupy(){
    return d.getSupply();
}
bool Menu::Max_Amount_Water_specific(string city_code){
    if(d.getCities().find(city_code) == d.getCities().end()) return false;
    Graph<string> s = d.getSupply();
    std::ofstream outputfile("Output.txt");
    //first need to create s super source
    list<pair<City,double>> r = edmondsKarp(s);
    for (auto p : r) {
        if(p.first.getCodeCity() == city_code)
        cout << p.first.getNameCity() << " - " << p.first.getCodeCity() << " - " << p.second << '\n';
    }
    outputfile.close();
    return false;
}
void Menu::Max_Amount_Water() {
    Graph<string> s = d.getSupply();
    std::ofstream outputfile("Output.txt");
    //first need to create s super source
    list<pair<City,double>> r = edmondsKarp(s);
    double total = 0;
    for (auto p : r) {
        cout << p.first.getNameCity() << " - " << p.first.getCodeCity() << " - " << p.second << '\n';
        total += p.second;
    }
    cout << total << "\n";
    outputfile.close();


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
            g.addEdge(super_source,v->getInfo(),d.getReservoiers()[v->getInfo()].getMaxDelivery());
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
//falta dar outpt a cidades que passaram a estar afetadas
void Menu::Balance_Load(Graph<string> s) {
    //pensar melhor nisto
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    set<string> cities_affected;
    for(auto p : l) cities_affected.insert(p.first.getCodeCity());
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


}


bool Menu::Remove_Water_Reservoir(std::string reservoi_code,Graph<string> s) {
    if(d.getReservoiers().find(reservoi_code) == d.getReservoiers().end()) return false;
    list<pair<City,double>> l = Meet_Costumer_needs(s);
    set<string> cities_affected;
    for(auto p : l) cities_affected.insert(p.first.getCodeCity());
    unordered_map<string,double> temp;
    for(auto p : l) temp[p.first.getCodeCity()] = p.second;
    unordered_map<Edge<string>*,double> restore_weights;
    Vertex<string>* v = s.findVertex(reservoi_code);
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


    cout << "The affected cities by the removal of the Reservoi are:\n";
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << '\n';
        }
    }
    return true;
}


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


    cout << "The affected cities by the removal of the Station are:\n";
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if ((cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) || (temp[p.first.getCodeCity()] > p.second)) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << '\n';

        }
    }


    return true;
}
/*
 * reve esta funçao por favor n tive tempo
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
*/
bool Menu::Remove_Pipe(Graph<string> s,std::string source, std::string target) {
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
    list<pair<City, double>> l = Meet_Costumer_needs(s);
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
    //Print result
    cout << "The affected cities by the removal of the Pipe are:\n";
    for(auto p : r){
        if(p.second < p.first.getDemand()) {
            if (cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << '\n';
        }
    }
    return true;
}
//funçao a falhar nao esta a restaurante corretamente as capacidades das arestas
void Menu::Remove_Pipe2(Graph<string> s){
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

                restore_weights[e] = e->getWeight();
                e->setWeight(0.0);



                for(auto p : e->getDest()->getAdj()){
                    if(p->getDest()->getInfo() == v->getInfo()){
                        restore_weights[p] = p->getWeight();
                        p->setWeight(0.0);
                        break;
                    }
                }

            }
            else {

                restore_weights[e] = e->getWeight();
                e->setWeight(0.0);
            }
            //Solve for EdmondKarp

            list<pair<City, double>> r = edmondsKarp(s);
            //Evalute the context
            if (bidirectional) {
                e->setWeight(restore_weights[e]);



                for(auto p : e->getDest()->getAdj()){
                    if(p->getDest()->getInfo() == v->getInfo()){
                        e->setWeight(restore_weights[e]);
                        break;
                    }
                }

            }
            else {
                e->setWeight(restore_weights[e]);

            }
            //Print result
            cout << "The affected cities by the removal of the Pipe " << v->getInfo() << " --> " << e->getDest()->getInfo() << " are:\n";
            for(auto p : r){
                if(p.second < p.first.getDemand()) {
                    if (cities_affected.find(p.first.getCodeCity()) == cities_affected.end()) cout << p.first.getNameCity() << ' ' << (p.first.getDemand() - p.second) << '\n';
                }
            }
        }
    }
}


