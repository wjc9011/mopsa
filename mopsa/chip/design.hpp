#ifndef MOPSA_CHIP_DESIGN_H
#define MOPSA_CHIP_DESIGN_H

#include <mopsa/headerdef.hpp>
#include <mopsa/geometry/polygon.hpp>
#include <mopsa/geometry/point.hpp>

#include <filesystem>

namespace mopsa
{

class Design
{

public:

  bool load_design(const std::filesystem::path &path);

  inline int num_obstacles() const;

  inline const point & upper_left() const;

  inline const point & lower_right() const;

private:

  std::vector<polygon> _obstacles;
  // the uppder left and lower right of design
  point _upper_left, _lower_right;

};

/**************************************
  Inline Function
 **************************************/

int
Design::num_obstacles() const
{
  return _obstacles.size();
}

const point &
Design::upper_left() const
{
  return _upper_left;
}

const point &
Design::lower_right() const
{
  return _lower_right;
}

}

#endif
