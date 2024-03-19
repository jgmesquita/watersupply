//
// Created by afdom on 16/03/2024.
//
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
bool Menu::Max_Amount_Water_specific(string city_code){
    if(d.getCities().find(city_code) == d.getCities().end()) return false;
    Graph<string> s = d.getSupply();
    std::ofstream outputfile("Output.txt");
    //first need to create a super source
    string super_source = "SS";
    string super_target = "ST";
    s.addVertex(super_source);
    s.addVertex(super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'R') s.addEdge(super_source,v->getInfo(),DBL_MAX);
    }
    //create a super sink
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C') s.addEdge(v->getInfo(),super_target,DBL_MAX);
    }
    edmondsKarp(&s,super_source,super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo() == city_code){
            double value = 0.0;
            for(auto e : v->getIncoming()){
                value += e->getFlow();
            }
            City temp = d.getCities()[city_code];
            cout << temp.getNameCity() << " - " << city_code << " - " << value << '\n';
            outputfile << temp.getNameCity() << " - " << city_code << " - " << value << endl;
            return true;
        }
    }
    outputfile.close();
    return false;
}
void Menu::Max_Amount_Water() {
    Graph<string> s = d.getSupply();
    std::ofstream outputfile("Output.txt");
    //first need to create a super source
    string super_source = "SS";
    string super_target = "ST";
    s.addVertex(super_source);
    s.addVertex(super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'R') s.addEdge(super_source,v->getInfo(),DBL_MAX);
    }
    //create a super sink
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C') s.addEdge(v->getInfo(),super_target,DBL_MAX);
    }
    edmondsKarp(&s,super_source,super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C'){
            double value = 0.0;
            for(auto e : v->getIncoming()){
                value += e->getFlow();
            }
            City temp = d.getCities()[v->getInfo()];
            cout << temp.getNameCity() << " - " << temp.getCodeCity() << " - " << value << '\n';
            outputfile << temp.getNameCity() << " - " << temp.getCodeCity() << " - " << value << endl;
        }
    }
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
// Return true if a path to the target is found, false otherwise
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


void Menu::edmondsKarp(Graph<string> *g, string source, string target) {

    Vertex<string>* s = g->findVertex(source);
    Vertex<string>* t = g->findVertex(target);

    for (auto v : g->getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
    while( findAugmentingPath(g, s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
}

list<pair<City,double>> Menu::Meet_Costumer_needs(){
    Graph<string> s = d.getSupply();
    list<pair<City,double>> result;
    std::ofstream outputfile("Output.txt");
    //first need to create a super source
    string super_source = "SS";
    string super_target = "ST";
    s.addVertex(super_source);
    s.addVertex(super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'R') s.addEdge(super_source,v->getInfo(),DBL_MAX);
    }
    //create a super sink
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C') s.addEdge(v->getInfo(),super_target,DBL_MAX);
    }
    edmondsKarp(&s,super_source,super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C'){
            double value = 0.0;
            for(auto e : v->getIncoming()){
                value += e->getFlow();
            }
            City temp = d.getCities()[v->getInfo()];
            if(value < temp.getDemand()) result.push_back(make_pair(temp,value));


        }
    }
    return result;
}

void Menu::Balance_Load() {
    //pensar melhor nisto
    Graph<string> s = d.getSupply();
    list<pair<City,double>> result;

    //first need to create a super source
    string super_source = "SS";
    string super_target = "ST";
    s.addVertex(super_source);
    s.addVertex(super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'R') s.addEdge(super_source,v->getInfo(),DBL_MAX);
    }
    //create a super sink
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C') s.addEdge(v->getInfo(),super_target,DBL_MAX);
    }
    edmondsKarp(&s,super_source,super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'R') s.removeEdge(super_source,v->getInfo());
    }
    //create a super sink
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C') s.removeEdge(v->getInfo(),super_target);
    }
    s.removeVertex(super_source);
    s.removeVertex(super_target);
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
    for(auto v : s.getVertexSet()) {
        double total_diff = 0.0;
        double n_edges = 0.0;
        for (auto e: v->getIncoming()) {
            n_edges++;
            total_diff += (e->getWeight() - e->getFlow());
        }
        if(n_edges != 0) {
            total_diff /= n_edges;
            for (auto e: v->getIncoming()) {
                if (e->getWeight() - e->getFlow() > total_diff)
                    e->setFlow(e->getFlow() + ((e->getWeight() - e->getFlow()) - total_diff));
                else if (e->getWeight() - e->getFlow() < total_diff)
                    e->setFlow(e->getFlow() - (total_diff - (e->getWeight() - e->getFlow())));
            }
        }
    }

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
bool Menu::Remove_Water_Reservoir(std::string reservoi_code) {
    if(d.getReservoiers().find(reservoi_code) == d.getReservoiers().end()) return false;
    Graph<string> s = d.getSupply();
    list<pair<City,double>> l = Meet_Costumer_needs();
    list<pair<City,double>> r;
    for(auto v : s.getVertexSet()){

        for(auto e: v->getAdj()){
            if(e->getDest()->getInfo() == reservoi_code)s.removeEdge(e->getDest()->getInfo(),reservoi_code);
        }

    }
    for(auto v : s.getVertexSet()){
        if(v->getInfo() == reservoi_code){
            for(auto e: v->getAdj()){
                s.removeEdge(reservoi_code,e->getDest()->getInfo());
            }
        }
    }
    s.removeVertex(reservoi_code);
    string super_source = "SS";
    string super_target = "ST";
    s.addVertex(super_source);
    s.addVertex(super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'R') s.addEdge(super_source,v->getInfo(),DBL_MAX);
    }
    //create a super sink
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C') s.addEdge(v->getInfo(),super_target,DBL_MAX);
    }
    edmondsKarp(&s,super_source,super_target);
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C'){
            double value = 0.0;
            for(auto e : v->getIncoming()){
                value += e->getFlow();
            }
            City temp = d.getCities()[v->getInfo()];
            if(value < temp.getDemand()) r.push_back(make_pair(temp,value));

        }
    }
    vector<string> t;
    cout << "The affected cities by the removal of the reservoir are:\n";
    for(auto p : r){
        if(std::find(l.begin(), l.end(),p) == l.end()) cout << p.first.getNameCity() << '\n';
    }
    return true;
}

bool Menu::Maintenance_Station(string station_code) {
    if (d.getStations().find(station_code) == d.getStations().end()) return false;
    Graph<string> s = d.getSupply();
    list<pair<City, double>> l = Meet_Costumer_needs();
    list<pair<City, double>> r;
    for (auto v: s.getVertexSet()) {

        for (auto e: v->getAdj()) {
            if (e->getDest()->getInfo() == station_code)
                s.removeEdge(e->getDest()->getInfo(), station_code);
        }

    }
    for (auto v: s.getVertexSet()) {
        if (v->getInfo() == station_code) {
            for (auto e: v->getAdj()) {
                s.removeEdge(station_code, e->getDest()->getInfo());
            }
        }
    }
    s.removeVertex(station_code);
    string super_source = "SS";
    string super_target = "ST";
    s.addVertex(super_source);
    s.addVertex(super_target);
    for (Vertex<string> *v: s.getVertexSet()) {
        if (v->getInfo()[0] == 'R') s.addEdge(super_source, v->getInfo(), DBL_MAX);
    }
    //create a super sink
    for (Vertex<string> *v: s.getVertexSet()) {
        if (v->getInfo()[0] == 'C') s.addEdge(v->getInfo(), super_target, DBL_MAX);
    }
    edmondsKarp(&s, super_source, super_target);
    for (Vertex<string> *v: s.getVertexSet()) {
        if (v->getInfo()[0] == 'C') {
            double value = 0.0;
            for (auto e: v->getIncoming()) {
                value += e->getFlow();
            }
            City temp = d.getCities()[v->getInfo()];
            if (value < temp.getDemand()) r.push_back(make_pair(temp, value));

        }
    }
    vector<string> t;
    cout << "The affected cities by the removal of the Station are:\n";
    for (auto p: r) {
        if (std::find(l.begin(), l.end(), p) == l.end()) cout << p.first.getNameCity() << ' ' << p.second << '\n';
    }
    return true;
}

bool Menu::Remove_Pipe(std::string source, std::string target, bool bidirectional) {
    Graph<string> s = d.getSupply();
    //Check for already existent problems with supply
    list<pair<City, double>> l = Meet_Costumer_needs();
    list<pair<City, double>> r;
    //Check for invalid source or target
    auto v_source = s.findVertex(source);
    auto v_target = s.findVertex(target);
    if (v_source == nullptr || v_target) {
        return false;
    }
    if (bidirectional) {
        s.removeEdge(source, target);
        s.removeEdge(target, source);
    }
    else {
        s.removeEdge(source, target);
    }
    //Solve for EdmondKarp
    string super_source = "SS";
    string super_target = "ST";
    s.addVertex(super_source);
    s.addVertex(super_target);
    //Super Source
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'R') s.addEdge(super_source,v->getInfo(),DBL_MAX);
    }
    //Super Target
    for(Vertex<string>* v : s.getVertexSet()){
        if(v->getInfo()[0] == 'C') s.addEdge(v->getInfo(),super_target,DBL_MAX);
    }
    edmondsKarp(&s,super_source,super_target);
    //Print result
    cout << "The affected cities by the removal of the respective edge are:" << endl;
    for (auto p: r) {
        if (std::find(l.begin(), l.end(), p) == l.end()) cout << p.first.getNameCity() << ' ' << p.second << endl;
    }
    return true;
}

