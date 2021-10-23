//
// Created by Vladimir Polikushin on 19.10.2021.
//

#include "DataBase.h"

void DataBase::Read(std::istream &input) {
    auto full_input = Json::Load(input).GetRoot().AsMap();
    stat_requests_ = full_input.at("stat_requests").AsArray();
    for(auto& raw_request : full_input.at("base_requests").AsArray()) {
        auto request = raw_request.AsMap();
        if(request.at("type").AsString() == "Bus") {
            this->ProcessAddBusRequest(request);
        } else if(request.at("type").AsString() == "Stop") {
            this->ProcessAddStopRequest(request);
        }
    }
}

void DataBase::Perform() {
    for(auto& bus_with_info : buses_) {
        if(bus_with_info.second.GetNumberUniqueStops() > 1) {
            bus_with_info.second.ComputeStraightRouteLength(stops_);
            bus_with_info.second.ComputeRealRouteLength(distances_);
            bus_with_info.second.ComputeCurvature();
        }
    }
}

void DataBase::Answer(std::ostream &output) {
    std::vector<Json::Node> answer;
    for(const auto& raw_request : stat_requests_) {
        auto request = raw_request.AsMap();
        std::string type = request.at("type").AsString();
        std::string name = request.at("name").AsString();
        int id = request.at("id").AsInt();
        if(type == "Bus") {
            answer.push_back(ProcessBusRequest(output, buses_[name], id));
        } else if(type == "Stop") {
            answer.push_back(ProcessStopRequest(output, stops_, name, id));
        }
    }
    PrintRoot(output, answer);
}

void DataBase::ProcessAddStopRequest(const std::map<std::string, Json::Node>& request) {
    auto stop_name = request.at("name").AsString();
    long double latitude = request.at("latitude").AsLongDouble();
    long double longitude = request.at("longitude").AsLongDouble();
    Coordinate stop_coordinate(latitude, longitude);
    auto stops_with_distances = request.at("road_distances").AsMap();
    for(const auto& stop_with_distance : stops_with_distances) {
        std::string name = stop_name + "_" + stop_with_distance.first;
        int distance = stop_with_distance.second.AsInt();
        distances_[std::move(name)] = distance;
    }
    //std::cout << "\nDEBUG: for stop " << stop_name << " was set Coordinates: " << stop_coordinate << "\n";
    stops_[std::move(stop_name)].SetCoordinates(stop_coordinate);
}

void DataBase::ProcessAddBusRequest(const std::map<std::string, Json::Node>& request) {
    auto bus_name = request.at("name").AsString();
    std::vector<std::string> stops;
    std::set<std::string> unique_stops;
    for(const auto& raw_stop : request.at("stops").AsArray()) {
        auto stop = raw_stop.AsString();
        unique_stops.insert(stop);
        stops.emplace_back(std::move(stop));
    }
    for(const auto& stop : unique_stops) {
        stops_[stop].AddBus(bus_name);
    }
    if(!request.at("is_roundtrip").AsBool()) {
        for(int i = stops.size() - 2; i >= 0; --i) {
            stops.emplace_back(stops[i]);
        }
    }
    buses_[bus_name].SetNumberUniqueStops(unique_stops.size());
    buses_[bus_name].SetNumberStops(stops.size());
    buses_[bus_name].SetStops(std::move(stops));
}

Json::Node DataBase::ProcessBusRequest(std::ostream &output, const BusInfo& bus_info, int id) {
    std::map<std::string, Json::Node> answer;
    answer["request_id"] = Json::Node(id);
    if(bus_info.GetNumberUniqueStops() == 0) {
        answer["error_message"] = Json::Node(std::string("not found"));
        return answer;
    }
    answer["stop_count"] = Json::Node(bus_info.GetNumberStops());
    answer["unique_stop_count"] = Json::Node(bus_info.GetNumberUniqueStops());
    answer["route_length"] = Json::Node(bus_info.GetRealRouteLength());
    answer["curvature"] = Json::Node(bus_info.GetCurvature());
    return answer;
}

Json::Node DataBase::ProcessStopRequest(std::ostream &output,
                                        const std::unordered_map<std::string, StopInfo>& stops_,
                                        const std::string& stop_name,
                                        int id) {
    std::map<std::string, Json::Node> answer;
    answer["request_id"] = Json::Node(id);
    if(stops_.find(stop_name) == stops_.end()) {
        answer["error_message"] = Json::Node(std::string("not found"));
        return answer;
    }
    const auto buses_set = stops_.at(stop_name).GetBuses();
    std::vector<Json::Node> buses;
    for(const auto& bus : buses_set) {
        buses.emplace_back(Json::Node(bus));
    }
    answer["buses"] = std::move(buses);
    return answer;
}

void PrintRoot(std::ostream& output, const std::vector<Json::Node>& data) {
    output << '[';
    for(auto it = data.begin(); it != data.end(); it++) {
        if(it == data.begin()) {
            output << std::endl;
        }
        Print(output, it->AsMap());
        if(next(it) != data.end()) {
            output << ',';
        }
        output << std::endl;
    }
    output << ']';
}

void Print(std::ostream& output, const std::vector<Json::Node>& data) {
    output << '[';
    for(auto it = data.begin(); it != data.end(); it++) {
        if(it == data.begin()) {
            output << std::endl;
        }
        Print(output, it->AsString());
        if(next(it) != data.end()) {
            output << ',';
        }
        output << std::endl;
    }
    output << ']';
}

void Print(std::ostream& output, const std::map<std::string, Json::Node>& data) {
    output << '{';
    for(auto it = data.begin(); it != data.end(); it++) {
        output << std::endl;
        Print(output, it->first);
        output << ": ";
        if(it->first == "route_length" ||
           it->first == "request_id" ||
           it->first == "stop_count" ||
           it->first == "unique_stop_count") {
            Print(output, it->second.AsInt());
        } else if(it->first == "curvature") {
            Print(output, it->second.AsLongDouble());
        } else if(it->first == "error_message") {
            Print(output, it->second.AsString());
        } else if(it->first == "buses") {
            Print(output, it->second.AsArray());
        }
        if(next(it) != data.end()) {
            output << ',';
        }
        output << std::endl;
    }
    output << '}';
}

void Print(std::ostream& output, const std::string& data) {
    output << '"' << data << '"';
}

void Print(std::ostream& output, const long double data) {
    output << data;
}

void Print(std::ostream& output, const int data) {
    output << data;
}








