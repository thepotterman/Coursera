//
// Created by Vladimir Polikushin on 20.10.2021.
//

#ifndef B_STOPINFO_H
#define B_STOPINFO_H

#endif //B_STOPINFO_H

#pragma once

#include <set>

#include "Coordinate.h"

class StopInfo{
public:
    StopInfo() = default;
    explicit StopInfo(Coordinate stop_coordinate)
    : stop_coordinate_(stop_coordinate) {}
    void SetCoordinates(Coordinate stop_coordinate);
    void AddBus(const std::string& bus_name);
    Coordinate GetCoordinates() const;
    std::set<std::string> GetBuses() const;
private:
    std::set<std::string> buses_stops_here_;
    Coordinate stop_coordinate_;
};

std::ostream& operator<<(std::ostream& output, const StopInfo& data);