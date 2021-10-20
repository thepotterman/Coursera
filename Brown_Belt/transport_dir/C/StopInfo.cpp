//
// Created by Vladimir Polikushin on 20.10.2021.
//


#include "StopInfo.h"

Coordinate StopInfo::GetCoordinates() const {
    return stop_coordinate_;
}

std::set<std::string> StopInfo::GetBuses() const{
    return buses_stops_here_;
}

void StopInfo::SetCoordinates(Coordinate stop_coordinate) {
    stop_coordinate_ = stop_coordinate;
}

void StopInfo::AddBus(const std::string& bus_name) {
    buses_stops_here_.insert(bus_name);
}

std::ostream &operator<<(std::ostream& output, const StopInfo& data) {
    const auto buses = data.GetBuses();
    if(buses.empty()) {
        output << "no buses";
    } else {
        output << "buses";
        for(const std::string& bus : buses) {
            output << " " << bus;
        }
    }
    return output;
}
