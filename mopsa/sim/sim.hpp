#ifndef MOPSA_SIM_SIM_H
#define MOPSA_SIM_SIM_H

#include <mopsa/headerdef.hpp>
#include <mopsa/chip/chip.hpp>
#include <mopsa/util/reader.hpp>
#include <mopsa/sim/particle.hpp>
#include <mopsa/sim/sim_flowgrid.hpp>
#include <vector>

namespace mopsa
{

class Simulate
{

public:

  struct Setting : public Reader
  {
    Setting() : 
      Reader("%,;[]=\'\"", " \r\t\n"),

      time_resolution(1), 
      alpha(1), 
      beta(1.45), 

      init_position_shift(0, 0),
      init_v(0, 0),

      max_timestep(100000),
      sim_boundary_x_ratio(0),

      chip_name("chip_name"),
      output_folder("."),

      dump_debug_file(false),

      use_sim_gridflow(true),

      adv_wall_effect_enabled(true),
      adv_wall_effect_collision_threshold(0.01),
      adv_wall_effect_candidate_threshold(1),
      adv_wall_effect_extra_shift(0.01),
      debug_adv_wall_effect_consistency(false)
    {}

    // simulation parameter
    double time_resolution;
    double alpha;
    double beta;

    // particle information
    point init_position_shift;
    velocity init_v;
    std::vector<double> dPs;

    // boundary
    int max_timestep;
    double sim_boundary_x_ratio;

    // dumping setting
    std::string chip_name;
    std::filesystem::path output_folder;

    std::set<int> debug_step;

    bool dump_debug_file;

    bool use_sim_gridflow;

    // advv wall effect
    bool adv_wall_effect_enabled;
    double adv_wall_effect_collision_threshold;
    double adv_wall_effect_candidate_threshold;
    double adv_wall_effect_extra_shift;
    bool debug_adv_wall_effect_consistency;

  public:

    bool read(const std::filesystem::path &path);

    void dump(std::ostream &os);

  private:

    bool _is_comment_prefix() override;

    std::string read_comment() override;

    /* read string (braced by " or ' (e.g.,  "context" or 'context') ) */
    bool read_string(std::string&) override;
  };

  Simulate(Chip *chip, Setting *setting);

  ~Simulate();

  bool simulate();

private:

  bool _build_flow_grids();

  bool _simulate_low();

  velocity _cal_particle_velocity(std::vector<int> *covered_nodes_id);

  point _apply_velocity(Particle *particle, const velocity & vel);

  point _apply_wall_effect(Particle *particle);

  bool _apply_wall_effect_low(
    Particle *particle,
    std::pair<point, point> &two_points,
    double &portion
  );

  bool _apply_wall_effect_low_adv(
    Particle *particle,
    point & new_coord
  );

  //double _cal_adv_wall_effect_portion(
    //Particle *particle,
    //point A,
    //point B
  //);

  point _cal_wall_effect_position_based_on_A_B_porition(
    Particle *particle,
    point A,
    point B,
    double portion
  );

  bool _is_large_shift(point new_coord, point old_coord);

  bool _is_debug_step(int cur_step);

  void _dump(const std::vector<point> &trajectory, 
    const std::filesystem::path &path);

private:

  Chip *_chip;

  Setting *_setting;

  Particle *_particle;

  SimFlowGrid *_flowgrids;

  bool _debug_wall_effect;

  int _current_sim_step;
};

}

#endif
