#include <mopsa/geometry/point.hpp>

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/detail/equals/interface.hpp>
#include <string>

namespace mopsa
{

bool 
operator==(const point &p1, const point &p2)
{
  return boost::geometry::equals(p1, p2);
}

bool 
operator!=(const point &p1, const point &p2)
{
  return !boost::geometry::equals(p1, p2);
}

std::string 
to_string(const point &point)
{
  return "(" + std::to_string(point.x()) + ", " 
    + std::to_string(point.y()) + ")";
}

}
