//
// Created by Vladimir Polikushin on 19.10.2021.
//

#ifndef A_DATABASE_H
#define A_DATABASE_H

#endif //A_DATABASE_H

#pragma once

#include "BusInfo.h"
#include "StopInfo.h"

class DataBase {
public:
    void Read(std::istream& input);
    void Perform();
    void Answer(std::istream& input, std::ostream& output);
private:
    std::unordered_map<std::string, StopInfo> stops_;
    std::unordered_map<std::string, BusInfo> buses_;
};

std::string ReadName(std::istream& input, char delimeter);

std::string ReadNameTillEnd(std::istream& input);

int ReadNumber(std::istream& input);