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
            buses_[std::move(bus_name)] = std::move(bus_info);
        } else if(request_type == "Stop") {
            std::string stop_name = ReadName(request_input, ':');
            Coordinate stop_coordinate;
            request_input >> stop_coordinate;
            stops_[std::move(stop_name)] = stop_coordinate;
        }
    }
}

void DataBase::Perform() {
    for(auto& bus_with_info : buses_) {
        if(bus_with_info.second.GetNumberUniqueStops() > 1) {
            bus_with_info.second.ComputeRouteLength(stops_);
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
        }
    }
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