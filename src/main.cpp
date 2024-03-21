#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "Graph.h"
#include "Data.h"
#include "Menu.h"
using namespace std;




int main() {
    int choice;
    Menu m = Menu();
    Graph<string> s = m.getSupy();
    while (true) {
        cout << " _________________________________________ " << '\n';
        cout << "|      Water Supply Management System     |" << '\n';
        cout << "|                                         |" << '\n';
        cout << "|1.  Maximum flow                         |" << '\n';
        cout << "|2.  Costumer water needs                 |" << '\n';
        cout << "|3.  Balance Load across network          |" << '\n';
        cout << "|4.  Remove Water Reservoir               |" << '\n';
        cout << "|5.  Pumping Station Maintenance          |" << '\n';
        cout << "|6.  Remove Pipe                          |" << '\n';
        cout << "|7.  Remove each pipe                     |" << '\n';
        cout << "|_________________________________________|" << "\n";
        cout << "Please enter your choice:";
        cin >> choice;

        switch (choice) {
            case 1: {
                int opcao;
                cout << " _________________________________________ " << '\n';
                cout << "|      Water Supply Management System     |" << '\n';
                cout << "|                                         |" << '\n';
                cout << "|1.  Maximum flow of each city            |" << '\n';
                cout << "|2.  Maximum flow of desired city         |" << '\n';
                cout << "|_________________________________________|" << "\n";
                cout << "Please choose the desired option:";
                cin >> opcao;
                if(opcao ==1) m.Max_Amount_Water();
                else{
                    string code_city;
                    cout << "Please insert the desired city code:";
                    cin >> code_city;
                    if(!m.Max_Amount_Water_specific(code_city)) cout << "Not a valid city code!" << '\n';

                }
                break;
            }
            case 2: {
                list<pair<City,double>> r = m.Meet_Costumer_needs(s);
                for(auto temp : r)
                cout << temp.first.getNameCity() << " - " << temp.first.getCodeCity() << " - " << temp.second << '\n';
                break;
            }
            case 3: {
                //m.Balance_Load();
                break;
            }
            case 4: {
                string reservoi_code;
                cout << "Please insert a valid Reservoi code:";
                cin >> reservoi_code;
                if(!m.Remove_Water_Reservoir(reservoi_code,s)) cout << "Not a valid Reservoi code!" << '\n';
                break;
            }
            case 5: {
                string station_code;
                cout << "Please insert a valid Station code:";
                cin >> station_code;
                if(!m.Maintenance_Station(station_code,s)) cout << "Not a valid Station code!" << '\n';
                break;
            }
            case 6: {
                string source;
                cout << "Please insert a valid Source code (Station or Reservoir):";
                cin >> source;
                string target;
                cout << "Please insert a valid Target code (Station or City):";
                cin >> target;
                if(!m.Remove_Pipe(s,source,target)) cout << "Not a valid Pipe!" << '\n';
                break;
            }
            case 7: {
                m.Remove_Pipe2(s);

                break;
            }
            default:
                break;
        }
    }
    return 0;
}