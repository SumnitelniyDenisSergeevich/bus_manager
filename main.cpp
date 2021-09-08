#include <cassert>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    string temp = ""s;
    is >> temp;
    if (temp == "NEW_BUS"s) {
        q.type = QueryType::NewBus;
        is >> temp;
        q.bus = temp;
        int i = 0;
        is >> i;
        q.stops.clear();
        for (; i != 0; --i) {
            is >> temp;
            q.stops.push_back(temp);
        }

    }
    else if (temp == "BUSES_FOR_STOP"s) {
        q.type = QueryType::BusesForStop;
        is >> temp;
        q.stop = temp;
    }
    else if (temp == "STOPS_FOR_BUS"s) {
        q.type = QueryType::StopsForBus;
        is >> temp;
        q.bus = temp;
    }
    else if (temp == "ALL_BUSES"s) {
        q.type = QueryType::AllBuses;
    }
    return is;
}

bool operator == (const vector<string>& c1, const vector<string>& c2)
{
    if (c1.size() != c2.size() || c1.size() == 0) {
        return false;
    }

    vector<string>::const_iterator it1, it2;
    it1 = c1.begin();
    it2 = c2.begin();

    while (it1 != c1.end()) {
        if (*it1 != *it2) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}

void TestPointInPutQuery() {                               // проверка ввода Query
    istringstream input;
    input.str("NewBus 32 3 Ti Mi Vi"s);
    Query temp;
    input >> temp;
    assert(temp.type == QueryType::NewBus);
    assert(temp.bus == "32"s);
    vector<string> p = { "Ti"s,"Mi"s,"Vi"s };
    assert(temp.stops == p);

    istringstream input5;
    input5.str("NewBus 32 1 Ti"s);
    Query temp0;
    input5 >> temp0;
    assert(temp0.type == QueryType::NewBus);
    assert(temp0.bus == "32"s);
    vector<string> p1 = { "Ti"s };
    assert(temp0.stops == p1);

    istringstream input2;
    input2.str("BusesForStop Mar"s);
    Query temp1;
    input2 >> temp1;
    assert(temp1.type == QueryType::BusesForStop);
    assert(temp1.stop == "Mar"s);

    istringstream input3;
    input3.str("StopsForBus 33"s);
    Query temp2;
    input3 >> temp2;
    assert(temp2.type == QueryType::StopsForBus);
    assert(temp2.bus == "33"s);

    istringstream input4;
    input4.str("AllBuses"s);
    Query temp3;
    input4 >> temp3;
    assert(temp3.type == QueryType::AllBuses);

    //cout<<"InPutQuery ок"s << endl;
}

struct BusesForStopResponse {
    vector<string> buses;       // автобусы, останавливающиеся на этой остановке
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    bool b = false;
    if (r.buses.size() == 0) {
        os << "No stop"s;
    }
    else {
        for (const string& bus : r.buses) {
            if(b){
                os << " "s;
            }
            b = true;
            os << bus;
        }
    }

    return os;
}

void TestPointOutPutBusesForStopResponse() {                               // проверка вывода BusesForStop
    BusesForStopResponse temp;
    temp.buses = { "32"s, "32k"s, "950", "980"s };
    ostringstream output;
    output << temp;
    assert(output.str() == "32 32k 950 980"s);

    temp.buses = {};
    ostringstream output2;
    output2 << temp;
    assert(output2.str() == "No stop"s);

    temp.buses = { "32"s };
    ostringstream output3;
    output3 << temp;
    assert(output3.str() == "32"s);

    // cout << "TestPointOutPutBusesForStopResponse = Ok"s<< endl;
}

struct StopsForBusResponse { // oshibka
   vector<pair<string,vector<string>>> stops_to_buses;  // остановки - австобусы 
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    bool b = false;
    if (r.stops_to_buses.size() == 0) {
        os << "No bus"s ;
    }
    else {
        for (const auto& [stop, buses] : r.stops_to_buses) {
            if(b) {
                os << endl;
            }
            b = true;
            os << "Stop "s << stop << ": "s;
            if (buses.size() == 0) {
                os << "no interchange"s;
            }
            else {
                bool a = false;
                for (const string& bus : buses) {
                    if(a){
                        os << " "s;
                    }
                    a = true;
                    os << bus;
                }
            }
        }
    }
    return os;
}

void TestPointOutPutStopsForBusResponse() {                               // проверка вывода StopsForBus
    StopsForBusResponse temp;
    pair<string,vector<string>> p = {"Mar"s, {"32"s,"32k"s}};
    temp.stops_to_buses.push_back(p);
    p = {"Ter"s, {"33"s, "350"s, "32"s}};
    temp.stops_to_buses.push_back(p);
    ostringstream output;
    output << temp;
    assert(output.str() == "Stop Mar: 32 32k\nStop Ter: 33 350 32"s);
    
    StopsForBusResponse temp4;
    p = {"Mar"s, {}};
    temp4.stops_to_buses.push_back(p);
    p = {"Ter"s, { "33"s, "350"s, "32"s }};
    temp4.stops_to_buses.push_back(p);
    ostringstream output4;
    output4 << temp4;
    assert(output4.str() == "Stop Mar: no interchange\nStop Ter: 33 350 32"s);

    StopsForBusResponse temp1;
    ostringstream output1;
    output1 << temp1;
    assert(output1.str() == "No bus"s);

    StopsForBusResponse temp2;
    p = {"Mar"s, { "32"s, "32k"s}};
    temp2.stops_to_buses.push_back(p);
    ostringstream output2;
    output2 << temp2;
    assert(output2.str() == "Stop Mar: 32 32k"s);

    StopsForBusResponse temp3;
    p = {"Mar"s, { "32"s}};
    temp3.stops_to_buses.push_back(p);
    ostringstream output3;
    output3 << temp3;
    assert(output3.str() == "Stop Mar: 32"s);

    //cout << "TestPointOutPutStopsForBusResponse = Ok"s<< endl;
}

struct AllBusesResponse {
    map<string, vector<string>> buses_to_stops;   // автобус - остановки
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    bool b = false;
    if (r.buses_to_stops.size() == 0) {
        os << "No buses"s;
    }
    else {
        for (const auto& bus_item : r.buses_to_stops) {
            if(b) {
                os << endl;
            }
            b = true;
            os << "Bus "s << bus_item.first << ": "s;
            bool a = false;
            for (const string& stop : bus_item.second) {
                if(a){
                    os << " "s;
                }
                a = true;
                os << stop;
            }
        }
    }
    return os;
}

void TestPointOutPutAllBusesResponse() {                               // проверка вывода AllBusesResponse
    AllBusesResponse temp;
    temp.buses_to_stops["32"] = { "Mar"s, "Ter"s };
    temp.buses_to_stops["32K"] = { "Mor"s, "MEr"s, "Ter"s };
    ostringstream output;
    output << temp;
    assert(output.str() == "Bus 32: Mar Ter\nBus 32K: Mor MEr Ter"s);

    AllBusesResponse temp1;
    temp1.buses_to_stops["32"] = { "Mar"s, "Ter"s };
    ostringstream output1;
    output1 << temp1;
    assert(output1.str() == "Bus 32: Mar Ter"s);

    AllBusesResponse temp3;
    temp3.buses_to_stops["32"] = { "Mar"s };
    ostringstream output3;
    output3 << temp3;
    assert(output3.str() == "Bus 32: Mar"s);

    AllBusesResponse temp2;
    ostringstream output2;
    output2 << temp2;
    assert(output2.str() == "No buses"s);
    //cout << "TestPointOutPutAllBusesResponse = Ok"s<< endl;
}

class BusManager {                                                    //class
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        buses_to_stops[bus] = stops;
        for (const string& stop : stops) {
            stops_to_buses[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse buses_on_stop;
        if (stops_to_buses.count(stop)) {
            for (const string& bus : stops_to_buses.at(stop)) {
                buses_on_stop.buses.push_back(bus);
            }
        }
        return buses_on_stop;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse stops_buses_needs;     // остановка- автобусы нужные
        if(buses_to_stops.count(bus)){
            pair<string,vector<string>> p;
            for (const string& stop : buses_to_stops.at(bus)) {
              
                p.first = stop;
                p.second.clear();
                for (const string& other_bus : stops_to_buses.at(stop)) {
                    if (bus != other_bus) {
                        p.second.push_back(other_bus);
                    }
                }
                stops_buses_needs.stops_to_buses.push_back(p);
            }
        }
        return stops_buses_needs;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse all_buses;
        all_buses.buses_to_stops = buses_to_stops;
        return all_buses;
    }
    
private:
    map<string, vector<string>> buses_to_stops, stops_to_buses;
};

void TestGetBusesForStop() {
    BusManager temp1;
    BusesForStopResponse p;
    p=temp1.GetBusesForStop("Mer"s);
    assert(p.buses.size() == 0);

    temp1.AddBus("32", { "Mer","Ter" });
    p = temp1.GetBusesForStop("Mer"s);
    assert(p.buses.size() == 1);

    temp1.AddBus("33", { "Mir","Tir" });
    p = temp1.GetBusesForStop("Mer"s);
    assert(p.buses.size() == 1);
    temp1.AddBus("34", { "Mir","Mer" });
    p = temp1.GetBusesForStop("Mer"s);
    assert(p.buses.size() == 2);

    //cout << "TestGetBusesForStop = Ok" << endl;
}

void TestGetStopsForBus() {
    BusManager temp1;
    StopsForBusResponse p;
    p = temp1.GetStopsForBus("32"s);
    assert(p.stops_to_buses.size() == 0);

    

    //cout << "TestGetStopsForBus = Ok" << endl;
}

void TestGetAllBuses() {
    BusManager temp1;
    AllBusesResponse p;
    p = temp1.GetAllBuses();
    assert(p.buses_to_stops.size() == 0);

    temp1.AddBus("32", { "Ti"s,"Li"s,"Mi"s });
    p = temp1.GetAllBuses();
    assert(p.buses_to_stops.size() == 1);

    temp1.AddBus("33", { "Te"s });
    p = temp1.GetAllBuses();
    assert(p.buses_to_stops.size() == 2);

    temp1.AddBus("950", { "Tra"s });
    p = temp1.GetAllBuses();
    assert(p.buses_to_stops.size() == 3);

    //cout << "TestGetAllBuses = Ok" << endl;
}




// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }
}
