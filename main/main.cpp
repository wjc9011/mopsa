#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <mopsa/chip/chip.hpp>
#include <mopsa/sim/particle.hpp>
#include <mopsa/sim/sim.hpp>
#include <mopsa/geometry/point.hpp>

#include <iostream>
#include <deque>

int main()
{
  //using polygon = boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>>;

  ////polygon a{ {{0.4, 0}, {0.4, 2}, {01.2, -450}, {0.4, 0}} };
  //polygon a{ {{0, 0}, {0, 2}, {2, 0}, {0, 0}} };
  //polygon b{ {{0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0}} };

  //std::deque<polygon> c;

  //boost::geometry::intersection(a, b, c);

  //std::cout << boost::geometry::area(c[0]) << std::endl;

  mopsa::Chip chip;
  std::filesystem::path path = "../design_geometry_plain/80um.txt";
  chip.load_design(path);

  std::filesystem::path path_x = "../data/80um-x.csv";
  std::filesystem::path path_y = "../data/80um-y.csv";
  chip.load_flow(path_x, path_y);

  mopsa::point p = {chip.design().min_x(), 
    (chip.design().min_y() + chip.design().max_y())/2};

  p = mopsa::point_add(p, mopsa::point(40, 0));

  mopsa::Particle particle(p, 5, 200);

  mopsa::Simulate::Setting setting;
  setting.time_resolution = 10;
  setting.sim_boundary_x = chip.design().max_x() * 0.95;
  //setting.max_timestep = 100000;
  setting.max_timestep = 10;
  setting.output_folder = "../sim_output/80um/cpp/";
  setting.chip_name = "80um";

  mopsa::Simulate(&chip, &particle, &setting).simulate();
}
