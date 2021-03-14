#ifndef MOPSA_HEADERDEF_H
#define MOPSA_HEADERDEF_H

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <mopsa/geometry/polygon.hpp>
#include <mopsa/geometry/point.hpp>

namespace mopsa
{

using polygon = Polygon<point>;

template<class T>
struct Velocity {
  T vx, vy;

  Velocity() : vx(0), vy(0) {}

  Velocity(T vx, T vy) : vx(vx), vy(vy) {}

};

using velocity = Velocity<double>;

}
#endif
