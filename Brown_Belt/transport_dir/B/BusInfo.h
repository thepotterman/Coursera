//
// Created by Vladimir Polikushin on 19.10.2021.
//

#ifndef A_BUSINFO_H
#define A_BUSINFO_H

#endif //A_BUSINFO_H

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <sstream>

#include "StopInfo.h"
#include "Coordinate.h"

class BusInfo {
public:
    BusInfo() = default;
    BusInfo(int number_unique_stops, int number_stops, std::vector<std::string> stops)
    : number_unique_stops_(number_unique_stops),
      number_stops_(number_stops),
      stops_(std::move(stops)) {}

    void SetStops(std::vector<std::string> stops);
    void SetNumberStops(int number_stops);
    void SetNumberUniqueStops(int number_unique_stops);
    void ComputeRouteLength(const std::unordered_map<std::string, StopInfo>& stops);
    int GetNumberUniqueStops() const;
    int GetNumberStops() const;
    long double GetRouteLength() const;
    std::vector<std::string> GetStops() const;
private:
    int number_unique_stops_ = 0.0;
    int number_stops_ = 0.0;
    long double route_length_ = 0.0;
    std::vector<std::string> stops_;
};

std::ostream& operator<<(std::ostream& out, const BusInfo& data);

std::istream& operator>>(std::istream& input, BusInfo& data);

void DeleteSpacesInEndString(std::string& s);