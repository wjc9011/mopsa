#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <iostream>
#include <deque>

#include <mopsa/chip/chip.hpp>

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
}
