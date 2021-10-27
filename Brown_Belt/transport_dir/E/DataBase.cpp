//
// Created by Vladimir Polikushin on 19.10.2021.
//

#include "DataBase.h"

bool operator<(const CustomWeight& lhs, const CustomWeight& rhs) {
    return lhs.weight < rhs.weight;
}

bool operator>(const CustomWeight& lhs, const CustomWeight& rhs) {
    return lhs.weight > rhs.weight;
}

bool operator<(const CustomWeight& lhs, int rhs) {
    return lhs.weight < rhs;
}

bool operator>(const CustomWeight& lhs, int rhs) {
    return lhs.weight > rhs;
}

bool operator>=(const CustomWeight& lhs, int rhs) {
    return lhs.weight >= rhs;
}

bool operator==(const CustomWeight& lhs, int rhs) {
    return lhs.weight == rhs;
}

CustomWeight operator+(const CustomWeight& lhs, const CustomWeight& rhs) {
    CustomWeight answer;
    answer.weight = lhs.weight + rhs.weight;
    return answer;
}

void DataBase::Read(std::istream &input) {
    auto full_input = Json::Load(input).GetRoot().AsMap();
    stat_requests_ = full_input.at("stat_requests").AsArray();
    routing_settings_.bus_velocity =
            full_input.at("routing_settings").AsMap().at("bus_velocity").AsLongDouble() * 1000.0 / 60.0;
    routing_settings_.bus_wait_time =
            full_input.at("routing_settings").AsMap().at("bus_wait_time").AsInt();
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
    Graph::DirectedWeightedGraph<CustomWeight> graph(stop_id * 3);
    for(auto& bus_with_info : buses_) {
        BusInfo& bus_info = bus_with_info.second;
        if(bus_info.GetNumberUniqueStops() > 1) {
            bus_info.ComputeStraightRouteLength(stops_);
            bus_info.ComputeRealRouteLength(distances_);
            bus_info.ComputeCurvature();
            const std::vector<std::string>& stops = bus_info.GetStops();
            long double real_sum = bus_info.GetRealRouteLength();
            long double left_subsum = 0.0;
            long double right_subsum = real_sum;
            Graph::Edge<CustomWeight> current{};
            for(int i = 0; i < stops.size() - 1; ++i) {
                current.from = stops_.at(stops[i]).GetStopId() * 2;
                current.to = stops_.at(stops[i]).GetStopId() * 2 + 1;
                CustomWeight weight;
                weight.weight = routing_settings_.bus_wait_time;
                weight.name = stops[i];
                current.weight = std::move(weight);
                weight = {};
                graph.AddEdge(current);
                current = {};
                for (int j = i + 1; j < stops.size(); ++j) {
                    right_subsum -= GetRealDistance(distances_, stops[j - 1], stops[j]);
                    current.from = stops_.at(stops[i]).GetStopId() * 2 + 1;
                    current.to = stops_.at(stops[j]).GetStopId() * 2;
                    weight.weight = (real_sum - left_subsum - right_subsum) / routing_settings_.bus_velocity;
                    weight.name = bus_with_info.first;
                    weight.span_count.emplace(j - i);
                    current.weight = std::move(weight);
                    weight = {};
                    graph.AddEdge(current);
                }
                left_subsum += GetRealDistance(distances_, stops[i], stops[i + 1]);
                right_subsum = real_sum - left_subsum;
            }
        }
    }
    graph_ = graph;
    if(graph_) {
        Graph::Router<CustomWeight> router(graph_.value());
        router_.emplace(std::move(router));
    }
}

void DataBase::Answer(std::ostream &output) {
    std::vector<Json::Node> answer;
    for(const auto& raw_request : stat_requests_) {
        auto request = raw_request.AsMap();
        const std::string& type = request.at("type").AsString();
        if(type == "Bus") {
            answer.push_back(this->ProcessBusRequest(output, request));
        } else if(type == "Stop") {
            answer.push_back(this->ProcessStopRequest(output, request));
        } else if(type == "Route") {
            answer.push_back(this->ProcessRouteRequest(output, request));
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
    stops_[stop_name].SetCoordinates(stop_coordinate);
    stops_[stop_name].SetStopId(stop_id++);
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

Json::Node DataBase::ProcessBusRequest(std::ostream& output, const std::map<std::string, Json::Node>& request) {
    std::map<std::string, Json::Node> answer;
    int id = request.at("id").AsInt();
    const auto& bus_name = request.at("name").AsString();
    answer["request_id"] = Json::Node(id);
    if(buses_.find(bus_name) == buses_.end()) {
        answer["error_message"] = Json::Node(std::string("not found"));
        return answer;
    }
    const BusInfo& bus_info = buses_[bus_name];
    answer["stop_count"] = Json::Node(bus_info.GetNumberStops());
    answer["unique_stop_count"] = Json::Node(bus_info.GetNumberUniqueStops());
    answer["route_length"] = Json::Node(bus_info.GetRealRouteLength());
    answer["curvature"] = Json::Node(bus_info.GetCurvature());
    return answer;
}

Json::Node DataBase::ProcessStopRequest(std::ostream& output, const std::map<std::string, Json::Node>& request) {
    int id = request.at("id").AsInt();
    const auto& stop_name = request.at("name").AsString();
    std::map<std::string, Json::Node> answer;
    answer["request_id"] = Json::Node(id);
    if(stops_.find(stop_name) == stops_.end()) {
        answer["error_message"] = Json::Node(std::string("not found"));
        return answer;
    }
    const std::set<std::string>& buses_set = stops_.at(stop_name).GetBuses();
    std::vector<Json::Node> buses;
    buses.reserve(buses_set.size());
for(const std::string& bus : buses_set) {
        buses.emplace_back(Json::Node(bus));
    }
    answer["buses"] = std::move(buses);
    return answer;
}

Json::Node DataBase::ProcessRouteRequest(std::ostream& output, const std::map<std::string, Json::Node>& request) {
    std::map<std::string, Json::Node> answer;
    int id = request.at("id").AsInt();
    const auto& from = request.at("from").AsString();
    const auto& to = request.at("to").AsString();
    answer["request_id"] = Json::Node(id);
    std::optional<Graph::Router<CustomWeight>::RouteInfo> route;
    if(router_) {
        route = router_.value().BuildRoute(stops_.at(from).GetStopId() * 2, stops_.at(to).GetStopId() * 2);
    }
    if(!route) {
        answer["error_message"] = Json::Node(std::string("not found"));
    } else {
        answer["total_time"] = route->weight.weight;
        std::vector<Json::Node> items;
        for(size_t i = 0; i < route->edge_count; ++i) {
            std::map<std::string, Json::Node> item;
            const auto& edge = graph_->GetEdge(router_.value().GetRouteEdge(route->id, i));
            item["time"] = edge.weight.weight;
            //const std::string& local_from = stop_id_to_stop_name_[edge.from];
            //const std::string& local_to = stop_id_to_stop_name_[edge.to];
            if(edge.weight.span_count) {
                item["type"] = Json::Node(std::string("Bus"));
                item["bus"] = Json::Node(edge.weight.name);
                item["span_count"] = Json::Node(edge.weight.span_count.value_or(0));
                items.emplace_back(std::move(item));
            } else {
                item["type"] = Json::Node(std::string("Wait"));
                item["stop_name"] = Json::Node(edge.weight.name);
                items.emplace_back(std::move(item));
            }
        }
        answer["items"] = Json::Node(std::move(items));
    }
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
           it->first == "span_count" ||
           it->first == "unique_stop_count") {
            Print(output, it->second.AsInt());
        } else if(it->first == "curvature" ||
                  it->first == "time" ||
                  it->first == "total_time") {
            Print(output, it->second.AsLongDouble());
        } else if(it->first == "error_message" ||
                  it->first == "type" ||
                  it->first == "bus" ||
                  it->first == "stop_name") {
            Print(output, it->second.AsString());
        } else if(it->first == "buses") {
            Print(output, it->second.AsArray());
        } else if(it->first == "items") {
            PrintRoot(output, it->second.AsArray());
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
