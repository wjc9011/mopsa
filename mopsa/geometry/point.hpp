#ifndef MOPSA_GEOMETRY_POINT_H
#define MOPSA_GEOMETRY_POINT_H

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

namespace mopsa
{

using point = boost::geometry::model::d2::point_xy<double>;

extern bool operator==(const point &p1, const point &p2);
extern bool operator!=(const point &p1, const point &p2);
extern point operator+(const point &p1, const point &p2);
extern point operator-(const point &p1, const point &p2);
extern point operator*(const point &p1, double factor);

extern std::string to_string(const point &point);

}

#endif
