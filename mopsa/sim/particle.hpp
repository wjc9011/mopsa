#ifndef CHIP_PARTICLE_H
#define CHIP_PARTICLE_H

#include <mopsa/headerdef.hpp>
#include <mopsa/chip/design.hpp>

#include <vector>

namespace mopsa
{

class Particle
{

public:

  Particle(const point & coord, double diameter, int num_points);

  inline const point & coord() const;

  inline double diameter() const;

  inline double radius() const;

  inline const velocity vel() const;

  void update_coord(const point & new_coord);

  bool cover(const point & node);

  bool overlap_obstacle(const Obstacle & obstacle, 
      std::vector<point> &res, bool debug);

private:

  point _current_coord;

  double _diameter;

  int _num_points; // # of points to describe the circle
  polygon _polygon;
};

/**************************************
  Inline Function
 **************************************/

inline const point & 
Particle::coord() const
{
  return _current_coord;
}

inline double 
Particle::diameter() const
{
  return _diameter;
}

inline double 
Particle::radius() const
{
  return _diameter * 0.5;
}

}
#endif
