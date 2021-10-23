//
// Created by Vladimir Polikushin on 19.10.2021.
//

#ifndef A_DATABASE_H
#define A_DATABASE_H

#endif //A_DATABASE_H

#pragma once

#include "BusInfo.h"
#include "StopInfo.h"
#include "Json.h"

class DataBase {
public:
    void Read(std::istream& input);
    void Perform();
    void Answer(std::ostream& output);
private:
    void ProcessAddBusRequest(const std::map<std::string, Json::Node>& request);
    void ProcessAddStopRequest(const std::map<std::string, Json::Node>& request);
    Json::Node ProcessBusRequest(std::ostream& output, const BusInfo& bus_info, int id);
    Json::Node ProcessStopRequest(std::ostream& output,
                                  const std::unordered_map<std::string, StopInfo>& stops_,
                                  const std::string& stop_name,
                                  int id);
    std::unordered_map<std::string, StopInfo> stops_;
    std::unordered_map<std::string, BusInfo> buses_;
    std::unordered_map<std::string, int> distances_;
    std::vector<Json::Node> stat_requests_;
};

std::vector<std::pair<std::string, int>> ReadDistances(std::istream& input);

std::string ReadName(std::istream& input, char delimeter);

std::string ReadNameTillEnd(std::istream& input);

int ReadNumber(std::istream& input);

void PrintRoot(std::ostream& output, const std::vector<Json::Node>& data);

void Print(std::ostream& output, const std::vector<Json::Node>& data);

void Print(std::ostream& output, const std::map<std::string, Json::Node>& data);

void Print(std::ostream& output, const std::string& data);

void Print(std::ostream& output, const long double data);

void Print(std::ostream& output, const int data);
