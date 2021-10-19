#include <set>
#include <unordered_map>
#include <string>
#include <utility>
#include <vector>
#include <sstream>

#include "Coordinate.h"

using namespace std;

class BusInfo {
public:
    BusInfo() = default;
    BusInfo(vector<string> stops, size_t number_stops, size_t number_unique_stops)
    :stops_(std::move(stops)), number_stops_(number_stops), number_unique_stops_(number_unique_stops) {}
    void ComputeBusInfo(const unordered_map<string, Coordinate>& stops);
    void GetBusInfo() const;
    size_t GetUniqueStops() const;
private:
    vector<string> stops_;
    size_t number_stops_ = 0;
    size_t number_unique_stops_ = 0;
    long double route_length = 0;
};

void BusInfo::ComputeBusInfo(const unordered_map<string, Coordinate>& stops) {
    string prev_stop;
    for(const string& stop: stops_) {
        if(prev_stop.empty()) {
            prev_stop = stop;
            continue;
        }
        const long double dist = ComputeDistance(stops.at(prev_stop), stops.at(stop));
        route_length += dist;
        //cout << "\nDEBUG DIST:\n";
        //cout << "Dist between " << prev_stop << " and " << stop << " is " << dist << endl;
        prev_stop = stop;
    }
}

void BusInfo::GetBusInfo() const {
    cout << number_stops_ << " stops on route, "
         << number_unique_stops_ << " unique stops, "
         << route_length << " route length" << endl;
/*
    cout << "DEBUG: " << endl;
    for(const string& s : stops_) {
        cout << s << endl;
    }
*/
}

size_t BusInfo::GetUniqueStops() const {
    return number_unique_stops_;
}

class DataBase {
public:
    void AddStop(string stop_name, Coordinate stop_coordinates);
    void AddBus(string bus_number, BusInfo bus_info);
    void GetBusInfo(const string& bus_number);
    void Debug_Stops();
private:
    unordered_map<string, Coordinate> stops_;
    unordered_map<string, BusInfo> buses_;
};

void DataBase::AddStop(string stop_name, Coordinate stop_coordinates) {
    stops_[move(stop_name)] = stop_coordinates;
}

void DataBase::AddBus(string bus_number, BusInfo bus_info) {
    buses_[move(bus_number)] = move(bus_info);
}

void DataBase::GetBusInfo(const string& bus_number) {
    cout << "Bus " << bus_number << ": ";
    if(buses_[bus_number].GetUniqueStops()) {
        buses_[bus_number].ComputeBusInfo(stops_);
        buses_[bus_number].GetBusInfo();
    } else {
        cout << "not found" << endl;
    }
}

void DataBase::Debug_Stops() {
    for(const auto& el : stops_) {
        cout << el.first << ": " << el.second;
    }
}

int main() {
    cout.precision(6);
    size_t q = 0;
    cin >> q;
    DataBase db;
    for(size_t i = 0; i < q; ++i) {
        string s;
        cin >> s;
        if(s == "Stop") {
            string stop_name;
            Coordinate stop_coordinates;
            cin.ignore(1);
            getline(cin, stop_name, ':');
            cin >> stop_coordinates;
            db.AddStop(move(stop_name), move(stop_coordinates));
        } else if(s == "Bus") {
            string bus_number;
            cin.ignore(1);
            getline(cin, bus_number, ':');
            cin.ignore(1);
            vector<string> stops;
            set<string> unique_stops;
            string buf;
            getline(cin, buf);
            stringstream ss(buf);
            string stop;
            string current;
            bool is_circle = false;
            while(ss >> current) {
                if(current == ">") {
                    is_circle = true;
                    stop.pop_back();
                    stops.push_back(stop);
                    unique_stops.insert(stop);
                    stop.clear();
                } else if(current == "-") {
                    is_circle = false;
                    stop.pop_back();
                    stops.push_back(stop);
                    unique_stops.insert(stop);
                    stop.clear();
                } else {
                    stop += current + " ";
                }
            }
            stop.pop_back();
            unique_stops.insert(stop);
            stops.push_back(move(stop));
            stop.clear();
            if(!is_circle) {
                for(int j = static_cast<int>(stops.size() - 2); j >= 0; --j) {
                    stops.push_back(stops[j]);
                }
            }
            size_t stops_size = stops.size();
            db.AddBus(move(bus_number), move(BusInfo(move(stops), stops_size, unique_stops.size())));
        }
    }
    /*
    cout << endl;
    db.Debug_Stops();
    cout << endl;
    */
    cin >> q;
    for(size_t i = 0; i < q; ++i) {
        string req;
        cin >> req;
        if(req == "Bus") {
            string bus_number;
            cin.ignore(1);
            getline(cin, bus_number);
            db.GetBusInfo(bus_number);
        }
    }
    return 0;
}
