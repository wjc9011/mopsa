#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <mopsa/logger/debug.hpp>
#include <mopsa/util/reader.hpp>
#include <mopsa/chip/chip.hpp>
#include <mopsa/sim/particle.hpp>
#include <mopsa/sim/sim.hpp>
#include <mopsa/geometry/point.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
  std::filesystem::path design_path  = argv[1];
  std::filesystem::path flow_x_path  = argv[2];
  std::filesystem::path flow_y_path  = argv[3];
  std::filesystem::path setting_path = argv[4];

  mopsa::Chip chip;
  mopsa::Simulate::Setting setting;

  if(setting.read(setting_path)) setting.dump(std::cout);
  else mopsa::mopsa_exit(-1);

  chip.load_design(design_path);
  chip.load_flow(flow_x_path, flow_y_path);

  mopsa::Simulate(&chip, &setting).simulate();
}
