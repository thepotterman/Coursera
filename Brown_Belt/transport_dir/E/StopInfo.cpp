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

void StopInfo::SetStopId(size_t stop_id) {
    stop_id_ = stop_id;
}

size_t StopInfo::GetStopId() const {
    return stop_id_;
}
