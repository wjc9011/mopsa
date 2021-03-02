#ifndef MOPSA_GEOMETRY_POLYGON_H
#define MOPSA_GEOMETRY_POLYGON_H

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <mopsa/geometry/point.hpp>
namespace mopsa
{

template<class T>
class Polygon : public boost::geometry::model::polygon<T>
{
  using point = T;

public:

  inline const point& startpoint() const;
  inline const point& endpoint() const;

};

/**************************************
  Inline Function
 **************************************/

template<class T>
const typename Polygon<T>::point &
Polygon<T>::startpoint() const
{
  return this->outer().front();
}

template<class T>
const typename Polygon<T>::point &
Polygon<T>::endpoint() const
{
  return this->outer().back();
}


}

#endif
