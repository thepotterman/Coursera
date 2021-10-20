//
// Created by Vladimir Polikushin on 19.10.2021.
//

#include "DataBase.h"

void DataBase::Read(std::istream &input) {
    int q = ReadNumber(input);
    for(int i = 0; i < q; ++i) {
        std::string request;
        std::getline(input, request);
        std::stringstream request_input(request);
        std::string request_type;
        request_input >> request_type;
        if(request_type == "Bus") {
            std::string bus_name = ReadName(request_input, ':');
            BusInfo bus_info;
            request_input >> bus_info;
            for(const auto& stop : bus_info.GetStops()) {
                stops_[stop].AddBus(bus_name);
            }
            buses_[std::move(bus_name)] = std::move(bus_info);
        } else if(request_type == "Stop") {
            std::string stop_name = ReadName(request_input, ':');
            Coordinate stop_coordinate;
            request_input >> stop_coordinate;
            request_input.ignore(1);
            auto stops_with_distances = ReadDistances(request_input);
            for(auto& stop_with_distance : stops_with_distances) {
                std::string temp = stop_name + "_" + stop_with_distance.first;
                //std::cout << "\nDEBUG\n" << temp << "\nDEBUG\n";
                distances_[std::move(temp)] = stop_with_distance.second;
            }
            stops_[std::move(stop_name)].SetCoordinates(stop_coordinate);
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

void DataBase::Answer(std::istream& input, std::ostream &output) {
    int q = ReadNumber(input);
    for(int i = 0; i < q; ++i) {
        std::string request;
        std::getline(input, request);
        std::stringstream request_input(request);
        std::string request_type;
        request_input >> request_type;
        if(request_type == "Bus") {
            std::string bus_name = ReadNameTillEnd(request_input);
            output << "Bus " << bus_name << ": " << buses_[bus_name] << std::endl;
        } else if(request_type == "Stop") {
            std::string stop_name = ReadNameTillEnd(request_input);
            if(stops_.find(stop_name) != stops_.end()) {
                output << "Stop " << stop_name << ": " << stops_.at(stop_name) << std::endl;
            } else {
                output << "Stop " << stop_name << ": not found" << std::endl;
            }
        }
    }
}

std::vector<std::pair<std::string, int>> ReadDistances(std::istream& input) {
    std::vector<std::pair<std::string, int>> stops_with_distances;
    std::string current;
    std::string previous;
    std::string stop_name;
    int distance = 0;
    while(input >> current) {
        if(current == "to") {
            if(!stop_name.empty()) {
                stop_name.pop_back();
                stop_name.pop_back();
                stops_with_distances.emplace_back(std::move(std::make_pair(std::move(stop_name), distance)));
            }
            previous.pop_back();
            distance = std::stoi(previous);
            previous.clear();
            stop_name.clear();
            current.clear();
        } else {
            if(!previous.empty()) {
                stop_name += previous + " ";
                previous.clear();
            }
            previous = std::move(current);
            current.clear();
        }
    }
    stop_name += previous;
    stops_with_distances.emplace_back(std::move(std::make_pair(std::move(stop_name), distance)));
    return std::move(stops_with_distances);
}

std::string ReadName(std::istream& input, char delimeter) {
    std::string current;
    std::string name;
    input >> current;
    while(current.back() != delimeter) {
        name += current + " ";
        input >> current;
    }
    name += current;
    name.pop_back();
    return std::move(name);
}

std::string ReadNameTillEnd(std::istream& input) {
    std::string current;
    std::string name;
    while(input >> current) {
        name += current + " ";
    }
    name.pop_back();
    return std::move(name);
}

int ReadNumber(std::istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}