//
// Created by Vladimir Polikushin on 19.10.2021.
//

#ifndef A_COORDINATE_H
#define A_COORDINATE_H

#endif //A_COORDINATE_H

#include <iostream>

#pragma once

class Coordinate {
public:
    Coordinate() = default;
    Coordinate(long double lat, long double lon) : lat_(lat), lon_(lon) {}
    long double GetLat() const;
    long double GetLon() const;
    void SetLat(long double lat);
    void SetLon(long double lon);
private:
    long double lat_ = 0.0; //Широта
    long double lon_ = 0.0; //Долгота
};

std::istream& operator>>(std::istream& in, Coordinate &data);

std::ostream& operator<<(std::ostream& out, const Coordinate &data);

long double ComputeDistance(const Coordinate& lhs, const Coordinate& rhs) ;