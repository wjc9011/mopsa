
#include <mopsa/chip/design.hpp>
#include <mopsa/logger/logger.hpp>

#include <iostream>

namespace mopsa
{

bool 
Design::load_design(const std::filesystem::path &design_path)
{
  FILE* fp = fopen(design_path.c_str(), "r");

  if(!fp) {
    LOG(ERROR) << "Cannot open " << design_path << " when load design.\n";
    return false;
  }

  int res = 0, num_polygon = 0;
  res = fscanf(fp, "%d", &num_polygon);
  if(res != 1) {
    LOG(ERROR) << "Cannnot read " << design_path << "\n"; 
    return false;
  }

  std::vector<int> a;
  std::vector<double> xs, ys;
  _obstacles.resize(num_polygon);
  for(int i=0; i<num_polygon; i++) 
  {
    int num_points = 0;
    res = fscanf(fp, "%d", &num_points);
    if(res != 1) {
      LOG(ERROR) << "Cannnot read " << design_path << "\n"; 
      return false;
    }

    _obstacles[i].outer().reserve(num_points);
    for(int j=0; j<num_points; j++) 
    {
      double x, y;
      res = fscanf(fp, "%lf %lf", &x, &y);
      if(res != 2) {
        LOG(ERROR) << "Cannnot read " << design_path << "\n"; 
        return false;
      }
      _obstacles[i].outer().emplace_back(x, y);
      xs.push_back(x);
      ys.push_back(y);
    }

    if(num_points == 1)
    {
      LOG(WARNING) << "Obstacle " << i << " is a point.\n";
    }
    else if(num_points == 2) 
    {
      LOG(WARNING) << "Obstacle " << i << " is a line segment.\n";
    }
    else if(_obstacles[i].startpoint() != _obstacles[i].endpoint()) 
    {
      LOG(WARNING) << "Obstacle " << i << " is not a ring. " 
        << to_string(_obstacles[i].startpoint())
        << " <-> "
        << to_string(_obstacles[i].endpoint()) << '\n';
    }
  }
  std::sort(xs.begin(), xs.end());
  std::sort(ys.begin(), ys.end());

  _upper_left  = {xs.front(), ys.front()};
  _lower_right = {xs.back(), ys.back()};

  return true;
}

}
