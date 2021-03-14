#ifndef MOPSA_SIM_SIM_H
#define MOPSA_SIM_SIM_H

#include <mopsa/headerdef.hpp>
#include <mopsa/chip/chip.hpp>
#include <mopsa/sim/particle.hpp>

namespace mopsa
{

class Simulate
{

public:

  struct Setting
  {
    Setting() : 
      time_resolution(1), 
      alpha(1), 
      beta(1.45), 
      init_position_shift(0, 0),
      init_v(0, 0),
      max_timestep(100000),
      sim_boundary_x(0),
      output_folder("."),
      chip_name("chip_name")
    {}

    double time_resolution;

    double alpha, beta;

    point init_position_shift;
    velocity init_v;

    int max_timestep;
    double sim_boundary_x;

    std::filesystem::path output_folder;

    std::string chip_name;

    std::set<int> debug_step;
  };

  Simulate(Chip *chip, Particle * particle, Setting *setting);

  bool simulate();

private:

  velocity _cal_particle_velocity();

  point _apply_velocity(Particle *particle, const velocity & vel);

  point _apply_well_effect(Particle *particle);

  bool _is_debug_step(int cur_step);

  void _dump(const std::vector<point> &trajectory, 
    const std::filesystem::path &path);

private:

  Chip *_chip;

  Particle *_particle;

  Setting *_setting;

  bool _debug_wall_effect;
};

}

#endif
