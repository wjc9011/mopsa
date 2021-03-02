#ifndef MOPSA_CHIP_CHIP_H
#define MOPSA_CHIP_CHIP_H

#include <mopsa/headerdef.hpp>
#include <mopsa/geometry/polygon.hpp>
#include <mopsa/chip/design.hpp>
#include <mopsa/chip/flow.hpp>
#include <filesystem>

namespace mopsa
{

class Chip
{

public:
  Chip();

  bool load_design(const std::filesystem::path &design_path);
  bool load_flow(const std::filesystem::path &flow_x_path, 
      const std::filesystem::path &flow_y_path);

private:

  Design _design;

  Flow _flow;
};

}

#endif

