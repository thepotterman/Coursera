//
// Created by Vladimir Polikushin on 19.10.2021.
//

#include <cmath>

#include "Coordinate.h"

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

std::istream& operator>>(std::istream& in, Coordinate &data) {
    long double lat = 0;
    long double lon = 0;
    in >> lat;
    in.ignore(1);
    in >> lon;
    data.SetLat(lat);
    data.SetLon(lon);
    return in;
}
std::ostream& operator<<(std::ostream& out, const Coordinate &data) {
    out << data.GetLat() << ", " << data.GetLon();
    return out;
}

long double ComputeDistance(const Coordinate &lhs, const Coordinate &rhs)  {
    using std::acos;
    using std::sin;
    using std::cos;
    using std::fabs;
    const long double EARTH_RADIUS = 6371000;
    const long double PI = 3.1415926535;
    long double lat1 = lhs.GetLat() * PI / 180.0;
    long double lat2 = rhs.GetLat() * PI / 180.0;
    long double lon1 = lhs.GetLon() * PI / 180.0;
    long double lon2 = rhs.GetLon() * PI / 180.0;
    long double sigma = acos(
        sin(lat1) * sin(lat2) +
        cos(lat1) * cos(lat2) * cos(fabs(lon1 - lon2))
    );
    return sigma * EARTH_RADIUS;
}
