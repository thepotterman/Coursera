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

long double BusInfo::GetRouteLength() const {
    return route_length_;
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

void BusInfo::ComputeRouteLength(const std::unordered_map<std::string, StopInfo>& stops) {
    for(int i = 1; i < stops_.size(); ++i) {
        long double current_distance = ComputeDistance(stops.at(stops_[i]).GetCoordinates(),
                                                       stops.at(stops_[i - 1]).GetCoordinates());
        route_length_ += current_distance;
    }
}

std::vector<std::string> BusInfo::GetStops() const {
    return stops_;
}

std::ostream& operator<<(std::ostream& out, const BusInfo& data) {
    if(data.GetNumberUniqueStops() == 0) {
        out << "not found";
        return out;
    }
    out << data.GetNumberStops() << " stops on route, "
        << data.GetNumberUniqueStops() << " unique stops, "
        << data.GetRouteLength() << " route length";
    return out;
}

std::istream& operator>>(std::istream& input, BusInfo& data) {
    bool is_circle = true;
    std::string stop;
    std::string current;
    std::vector<std::string> stops;
    std::set<std::string> unique_stops;
    while(input >> current) {
        if(current == ">") {
            is_circle = true;
            DeleteSpacesInEndString(stop);
            unique_stops.insert(stop);
            stops.push_back(move(stop));
            stop.clear();
        } else if(current == "-") {
            is_circle = false;
            DeleteSpacesInEndString(stop);
            unique_stops.insert(stop);
            stops.push_back(move(stop));
            stop.clear();
        } else {
            stop += current + " ";
        }
    }
    DeleteSpacesInEndString(stop);
    unique_stops.insert(stop);
    stops.push_back(move(stop));
    stop.clear();
    if(!is_circle) {
        for(int i = stops.size() - 2; i >= 0; --i) {
            stops.push_back(stops[i]);
        }
    }
    data.SetNumberStops(stops.size());
    data.SetNumberUniqueStops(unique_stops.size());
    data.SetStops(move(stops));
    return input;
}

void DeleteSpacesInEndString(std::string& s) {
    while(s.back() == ' ') {
        s.pop_back();
    }
}












