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

}
#endif
