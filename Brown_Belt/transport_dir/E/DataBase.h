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
#include "graph.h"
#include "router.h"

struct CustomWeight {
    CustomWeight() = default;
    explicit CustomWeight(int new_weight) : weight(new_weight) {}
    long double weight = 0.0;
    std::string name;
    std::optional<int> span_count = std::nullopt;
};

bool operator<(const CustomWeight& lhs, const CustomWeight& rhs);

bool operator>(const CustomWeight& lhs, const CustomWeight& rhs);

bool operator<(const CustomWeight& lhs, int rhs);

bool operator>(const CustomWeight& lhs, int rhs);

bool operator>=(const CustomWeight& lhs, int rhs);

bool operator==(const CustomWeight& lhs, int rhs);

CustomWeight operator+(const CustomWeight& lhs, const CustomWeight& rhs);

struct RoutingSettings {
    int bus_wait_time = 0;
    long double bus_velocity = 0.0;
};

class DataBase {
public:
    void Read(std::istream& input);
    void Perform();
    void Answer(std::ostream& output);
private:
    size_t stop_id = 0;
    void ProcessAddBusRequest(const std::map<std::string, Json::Node>& request);
    void ProcessAddStopRequest(const std::map<std::string, Json::Node>& request);
    Json::Node ProcessBusRequest(std::ostream& output, const std::map<std::string, Json::Node>& request);
    Json::Node ProcessStopRequest(std::ostream& output, const std::map<std::string, Json::Node>& request);
    Json::Node ProcessRouteRequest(std::ostream& output, const std::map<std::string, Json::Node>& request);
    std::unordered_map<std::string, StopInfo> stops_;
    std::unordered_map<std::string, BusInfo> buses_;
    std::unordered_map<std::string, int> distances_;
    //std::unordered_map<size_t, std::string> stop_id_to_stop_name_;
    std::vector<Json::Node> stat_requests_;
    RoutingSettings routing_settings_;
    std::optional<Graph::DirectedWeightedGraph<CustomWeight>> graph_ = std::nullopt;
    std::optional<Graph::Router<CustomWeight>> router_ = std::nullopt;
};

void PrintRoot(std::ostream& output, const std::vector<Json::Node>& data);

void Print(std::ostream& output, const std::vector<Json::Node>& data);

void Print(std::ostream& output, const std::map<std::string, Json::Node>& data);

void Print(std::ostream& output, const std::string& data);

void Print(std::ostream& output, long double data);

void Print(std::ostream& output, int data);
