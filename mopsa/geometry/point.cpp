#include <mopsa/geometry/point.hpp>
#include <mopsa/util/util_funcs.h>

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/detail/equals/interface.hpp>
#include <string>

namespace mopsa
{

bool 
operator == (const point &p1, const point &p2)
{
  return boost::geometry::equals(p1, p2);
}

bool 
operator!=(const point &p1, const point &p2)
{
  return !boost::geometry::equals(p1, p2);
}

point operator+(const point &p1, const point &p2)
{
  return {p1.x() + p2.x(), p1.y() + p2.y()};
}

point operator-(const point &p1, const point &p2)
{
  return {p1.x() - p2.x(), p1.y() - p2.y()};
}

point operator*(const point &p1, double factor)
{
  return {p1.x()*factor, p1.y()*factor};
}

std::string 
to_string(const point &point)
{
  return "(" + to_string(point.x()) + ", " 
    + to_string(point.y()) + ")";
}

}
