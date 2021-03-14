#include "mopsa/geometry/point.hpp"
#include <cstdio>
#include <mopsa/sim/sim.hpp>
#include <mopsa/logger/logger.hpp>

#include <omp.h>

#include <iostream>
#include <string>

namespace mopsa
{

Simulate::Simulate(
  Chip *chip, 
  Particle * particle, 
  Setting * setting)
  : 
    _chip(chip)
  , _particle(particle)
  , _setting(setting)
  , _debug_wall_effect(false)
{

}

bool 
Simulate::simulate()
{
  int step_count = 0;

  LOG(INFO) << "Particle starts at " << to_string(_particle->coord()) 
    << ", diameter = " << _particle->diameter() << '\n';

  std::vector<point> trajectory;
  trajectory.push_back(_particle->coord());
  while(true) 
  {
    step_count += 1;
    bool debug = _is_debug_step(step_count);

    _debug_wall_effect = debug;

    velocity vel = _cal_particle_velocity();
    vel.vy *= 1.45;

    point new_coord = _apply_velocity(_particle, vel);
    (void)new_coord;

    point after_wall = _apply_well_effect(_particle); 
    (void)after_wall;

    trajectory.push_back(_particle->coord());
    
    if(step_count > _setting->max_timestep) {
      LOG(INFO) << "\nStop due to simulation step > " 
        << _setting->max_timestep << '\n';
      break;
    }

    if(_particle->coord().x() >= _setting->sim_boundary_x) {
      LOG(INFO) << "\nStop due to x >= bondary: " 
        << _setting->sim_boundary_x << '\n';
      break;
    }

    printf("step = %d, coord = %s\r", step_count, 
        to_string(_particle->coord()).c_str());
    std::fflush(stdout);

    if(debug) { 
      std::filesystem::path debug_file = 
        "./debug_" + std::to_string(step_count) + ".txt";

      _dump(trajectory, debug_file);

      std::cout << "\nPlease check" << std::endl;
      int a; std::cin >> a;
    }
  }
  printf("\n");

  LOG(INFO) << "Particle ends at " << to_string(_particle->coord()) << " "
    << " diameter = " << _particle->diameter() << '\n';

  // Dump output
  char buf[200];
  sprintf(buf, "%f", _particle->diameter());
  std::cout << buf << std::endl;
  std::filesystem::path output_path = _setting->output_folder
    / (_setting->chip_name + buf + ".txt");

  _dump(trajectory, output_path);

  return true;
}

velocity 
Simulate::_cal_particle_velocity()
{
  velocity vel(0, 0);
  int cnt = 0;

  #pragma omp parallel for
  for(size_t i = 0; i<_chip->flow().nodes().size(); i++) {
    const auto & node = _chip->flow().nodes()[i];
    bool covered = _particle->cover(node.coord);

    if(covered) {
      #pragma omp critical
      {
        cnt += 1;
        vel.vx += node.vx;
        vel.vy += node.vy;
      }
    }
  }

  if(cnt > 0) {
    vel.vx /= cnt;
    vel.vy /= cnt;
  }

  return vel;
}

point 
Simulate::_apply_velocity(
  Particle * particle,
  const velocity & vel
)
{
  point coord = particle->coord();
  point new_coord(0, 0);

  new_coord.x( coord.x() + vel.vx * _setting->time_resolution );
  new_coord.y( coord.y() + vel.vy * _setting->time_resolution );

  particle->update_coord(new_coord);
  return new_coord;
}

point 
Simulate::_apply_well_effect(Particle *particle)
{
  point coord = particle->coord();

  for(const auto & obst : _chip->design().obstacles()) {
    std::vector<point> res;
    if(particle->overlap_obstacle(obst, res, _debug_wall_effect)) {

      point middle = mopsa::point_add(res.front(), res.back());
      middle = mopsa::point_scaling(middle, 0.5);

      point direction = mopsa::point_minus(particle->coord(), middle);
      point dist = mopsa::point_scaling(direction, 
          (res.size()/201.0) * _particle->radius());

      point new_position = mopsa::point_add(particle->coord(), dist);

      if(_debug_wall_effect) {
        std::cout << "!!!!" << res.size() << std::endl;
        std::cout << "Cov front = " << to_string(res.front()) << 
          " back = " << to_string(res.back()) << std::endl;
        std::cout << "Middle = " << to_string(middle) << std::endl;
        std::cout << "Direction = " << to_string(direction) << std::endl;

        std::cout << to_string(coord) << " ===> " 
          << to_string(new_position) << std::endl;
        int a;
        std::cin >> a;
      }
      particle->update_coord(new_position);
      return new_position;
    }
  }

  return coord;
}

bool 
Simulate::_is_debug_step(int cur_step)
{
  if(_setting->debug_step.size() == 0) return false;
  else return _setting->debug_step.find(cur_step) != _setting->debug_step.end();
}

void 
Simulate::_dump(
  const std::vector<point> &trajectory,
  const std::filesystem::path &output_path
)
{
  LOG(INFO) << "Dump result to " << output_path << "\n";
  FILE *fp = fopen(output_path.c_str(), "w");
  fprintf(fp, "%ld %f\n", trajectory.size(), _particle->diameter());
  for(const auto & point : trajectory) {
    fprintf(fp, "%lf %lf\n", point.x(), point.y());
  }
  fclose(fp);
}

}
