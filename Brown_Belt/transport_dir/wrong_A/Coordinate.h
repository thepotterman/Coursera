#ifndef A_COORDINATE_H
#define A_COORDINATE_H
#include <iostream>
#include <cmath>

#endif //A_COORDINATE_H

class Coordinate {
public:
    Coordinate() = default;
    Coordinate(long double lat, long double lon) : lat_(lat), lon_(lon) {}
    long double GetLon() const;
    long double GetLat() const;
    void SetLat(long double lat);
    void SetLon(long double lon);
private:
    long double lat_ = 0.0;
    long double lon_ = 0.0;
};

long double Coordinate::GetLat() const {
    return lat_;
}

long double Coordinate::GetLon() const {
    return lon_;
}

void Coordinate::SetLat(long double lat) {
    lat_ = lat;
}

void Coordinate::SetLon(long double lon) {
    lon_ = lon;
}

std::istream& operator>>(std::istream& in, Coordinate& data) {
    long double lat = 0;
    long double lon = 0;
    in >> lat;
    in.ignore(1);
    in >> lon;
    data.SetLat(lat);
    data.SetLon(lon);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Coordinate& data) {
    out << "lat: " << data.GetLat() << ", lon: " << data.GetLon() << std::endl;
    return out;
}

Coordinate operator+(const Coordinate& lhs, const Coordinate& rhs) {
    return {lhs.GetLat() + rhs.GetLat(), lhs.GetLon() + rhs.GetLon()};
}

long double ComputeDistance(const Coordinate& lhs, const Coordinate& rhs) {
    static const long double EARTH_RADIUS = 6371000;
    static const long double PI = 3.1415926535;
    long double phi1 = lhs.GetLon() * PI / 180.0;
    long double phi2 = rhs.GetLon() * PI / 180.0;
    long double lambda1 = lhs.GetLat() * PI / 180.0;
    long double lambda2 = rhs.GetLat() * PI / 180.0;
    long double sigma = std::acos(
            //std::cos(phi1) * std::cos(phi2) * std::cos(lambda1) * std::cos(lambda2) +
            //std::sin(phi1) * std::sin(phi2) * std::cos(lambda1) * std::cos(lambda2) +
            //std::sin(lambda1) * std::sin(lambda2)
            std::sin(lambda1) * std::sin(lambda2) +
            std::cos(lambda1) * std::cos(lambda2) * std::cos(std::abs(phi1 - phi2))
    );
    //std::cout << "DEBUG_ACOS: " << sigma << std::endl;
    return sigma * EARTH_RADIUS;
}