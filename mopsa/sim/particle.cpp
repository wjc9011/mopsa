#include "mopsa/geometry/point.hpp"
#include <boost/geometry/algorithms/detail/distance/interface.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/cos_pi.hpp>
#include <boost/math/special_functions/sin_pi.hpp>

#include <mopsa/logger/debug.hpp>
#include <mopsa/sim/particle.hpp>

#include <omp.h>

#include <iostream>

namespace mopsa
{

Particle::Particle(
  const point & coord, 
  double diameter,
  int num_points
) 
  : _current_coord(coord)
  , _diameter(diameter)
  , _radius(diameter * 0.5)
  ,_num_points(num_points)
{
  for(int i=0; i<_num_points; i++) {
    double degree = i * (2.0*boost::math::constants::pi<double>())/_num_points;

    double x = std::cos(degree) * _radius;
    double y = std::sin(degree) * _radius;
    _polygon.push_back({x, y});
  }
  _polygon.push_back( _polygon.startpoint() );

  FILE *fp = fopen("particle.txt", "w");

  fprintf(fp, "%lu\n", _polygon.size());
  for(const auto &point : _polygon.outer()) {
    fprintf(fp, "%lf %lf\n", point.x(), point.y());
  }
  fclose(fp);
}

void 
Particle::update_coord(const point & new_coord)
{
  _current_coord = new_coord;
}

bool 
Particle::cover(const point & node)
{
  if(  (node.x() - _current_coord.x() > _radius) 
    || (node.y() - _current_coord.y() > _radius))
    return false;

  double dist = boost::geometry::distance(_current_coord, node);

  return dist <= _radius;
}

bool 
Particle::overlap_obstacle(
  const Obstacle & obstacle, 
  std::vector<point> & res,
  bool debug)
{
  bool method_intersection = false;

  if(method_intersection) {
    std::vector<polygon> res;

    res = _polygon.intersection(obstacle.get_polygon());

    if (debug and res.size() >= 1) {
      FILE *fp = fopen("debug_1.txt", "w");

      fprintf(fp, "%lu\n", _polygon.size());
      for(const auto &point : _polygon.outer()) {
  fprintf(fp, "%lf %lf\n", point.x(), point.y());
      }

      fprintf(fp, "%lu\n", obstacle.get_polygon().size());
      for(const auto &point : obstacle.get_polygon().outer()) {
  fprintf(fp, "%lf %lf\n", point.x(), point.y());
      }

      for(const auto & poly : res) {
  fprintf(fp, "%lu\n", poly.size());
  for(const auto &point : poly.outer()) {
    fprintf(fp, "%lf %lf\n", point.x(), point.y());
  }
      }
      fclose(fp);
      mopsa_exit(-1);
    }

    return res.size() >= 1;
  }
  else {
    std::vector<std::pair<int, point>> myres;
    res.clear();
    if(obstacle.get_polygon().startpoint() != 
  obstacle.get_polygon().endpoint()) return false;

    #pragma omp parallel for
    for(size_t i=0; i<_polygon.size(); i++) {
      point x = mopsa::point_add(_polygon.outer()[i], _current_coord);

      bool within = false;
      within = obstacle.get_polygon().within(x);
      if(within) {
  #pragma omp critical
  {
    myres.push_back({i, x});
  }
      }
    }

    std::sort(myres.begin(), myres.end(), 
      [](const auto & p1, const auto & p2) { 
  return p1.first < p2.first;
      } 
    );

    for(const auto & data : myres) res.emplace_back(data.second);

    //double dist = boost::geometry::distance(_current_coord, obstacle.get_polygon().startpoint());

    //if(dist <= 20) debug = true;
    //else debug = false;
    //debug = false;
    if(debug and res.size()) {
      FILE *fp = fopen("debug_overlap.txt", "w");

      fprintf(fp, "%lu\n", _polygon.size());
      for(const auto &p : _polygon.outer()) {
  point p2 = mopsa::point_add(p, _current_coord);
  fprintf(fp, "%lf %lf\n", p2.x(), p2.y());
      }

      fprintf(fp, "%lu\n", obstacle.get_polygon().size());
      for(const auto &point : obstacle.get_polygon().outer()) {
  fprintf(fp, "%lf %lf\n", point.x(), point.y());
      }

      fprintf(fp, "%lu\n", res.size());
      for(const auto & p : res) {
  fprintf(fp, "%lf %lf\n", p.x(), p.y());
      }
      fclose(fp);
      std::cout << "\nPlease cheack" << std::endl;
      std::cout << "Res = " << res.size() << std::endl;
      std::cout << to_string(_current_coord) << " vs " <<
        to_string(obstacle.get_polygon().startpoint()) << std::endl;;
    }
    return res.size() >= 1;
  }
}

}
