//
// Created by Vladimir Polikushin on 19.10.2021.
//

#include "BusInfo.h"

int BusInfo::GetNumberUniqueStops() const {
    return number_unique_stops_;
}

int BusInfo::GetNumberStops() const {
    return number_stops_;
}

long double BusInfo::GetStraightRouteLength() const {
    return straight_route_length_;
}

int BusInfo::GetRealRouteLength() const {
    return real_route_length_;
}

long double BusInfo::GetCurvature() const {
    return curvature_;
}

void BusInfo::SetStops(std::vector<std::string> stops) {
    stops_ = std::move(stops);
}

void BusInfo::SetNumberStops(int number_stops) {
    number_stops_ = number_stops;
}

void BusInfo::SetNumberUniqueStops(int number_unique_stops) {
    number_unique_stops_ = number_unique_stops;
}

void BusInfo::ComputeStraightRouteLength(const std::unordered_map<std::string, StopInfo>& stops) {
    for(int i = 1; i < stops_.size(); ++i) {
        long double current_distance = ComputeDistance(stops.at(stops_[i]).GetCoordinates(),
                                                       stops.at(stops_[i - 1]).GetCoordinates());
        straight_route_length_ += current_distance;
    }
}

void BusInfo::ComputeRealRouteLength(const std::unordered_map<std::string, int>& distances) {
    for(int i = 1; i < stops_.size(); ++i) {
        const std::string& stop_from = stops_[i - 1];
        const std::string& stop_to = stops_[i];
        int current_distance = GetRealDistance(distances, stop_from, stop_to);
        real_route_length_ += current_distance;
    }
}

int GetRealDistance(const std::unordered_map<std::string, int>& distances,
                    const std::string& from, const std::string& to) {

    std::string variant1 = from + "_" + to;
    std::string variant2 = to + "_" + from;
    if(distances.find(variant1) != distances.end()) {
        return distances.at(variant1);
    } else if(distances.find(variant2) != distances.end()){
        return distances.at(variant2);
    } else {
        return 0;
    }
}

void BusInfo::ComputeCurvature() {
    curvature_ = static_cast<long double>(real_route_length_) / straight_route_length_;
}

std::vector<std::string> BusInfo::GetStops() const {
    return stops_;
}