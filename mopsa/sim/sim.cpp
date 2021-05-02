#include <boost/math/special_functions/math_fwd.hpp>
#include <boost/geometry/algorithms/detail/distance/interface.hpp>

#include <mopsa/util/util_funcs.h>
#include <mopsa/geometry/point.hpp>
#include <mopsa/sim/sim.hpp>
#include <mopsa/logger/logger.hpp>

#include <omp.h>

#include <cmath>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <cstdio>
#include <thread>

namespace mopsa
{

/**************************************
  Struct Simulate::Setting
 **************************************/
bool
Simulate::Setting::read(const std::filesystem::path &path)
{
  LOG(INFO) << "Read setting from " << path << '\n';
  if(!open_file(path)) {
    LOG(ERROR) << "Cannot open " << path << '\n';
    return false;
  }

  bool ok = true;
  std::string token;
  while(!_is_end()) {
    token = read_token();
    if(token == "") break;

    else if(token == "resolution") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      time_resolution = val;
      _expect_token(read_token(), ";");
    }

    else if(token == "alpha") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      alpha = val;
      _expect_token(read_token(), ";");
    }

    else if(token == "beta") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      beta = val;
      _expect_token(read_token(), ";");
    }

    else if(token == "init_x_shift") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      init_position_shift.x(val);
      _expect_token(read_token(), ";");
    }

    else if(token == "init_y_shift") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      init_position_shift.y(val);
      _expect_token(read_token(), ";");
    }

    else if(token == "start_vx") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      init_v.vx = val;
      _expect_token(read_token(), ";");
    }

    else if(token == "start_vy") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      init_v.vy = val;
      _expect_token(read_token(), ";");
    }
    
    else if(token == "dPs") {
      _expect_token(read_token(), "=");
      _expect_token(read_token(), "[");

      float val;
      while(!_is_end()) {
        if(!read_float(val)) {
          ok = false;
          break;
        }
        dPs.push_back(val);
        token = read_token();
        if(token == "]") break;
        else _expect_token(token, ",");
      }
      _expect_token(read_token(), ";");
    }

    else if(token == "boundary_x_ratio") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      sim_boundary_x_ratio = val;
      _expect_token(read_token(), ";");
    }

    else if(token == "boundary_max_timestep") {
      _expect_token(read_token(), "=");
      if(!read_int(max_timestep)) {
        ok = false;
        break;
      }
      _expect_token(read_token(), ";");
    }

    else if(token == "chip_name") {
      _expect_token(read_token(), "=");
      if(!read_string(chip_name)) {
        ok = false;
        break;
      }
      _expect_token(read_token(), ";");
    }

    else if(token == "output_folder") {
      _expect_token(read_token(), "=");
      std::string val;
      if(!read_string(val)) {
        ok = false;
        break;
      }
      output_folder = val;
      _expect_token(read_token(), ";");
    }

    else if(token == "dump_debug_file") {
      _expect_token(read_token(), "=");
      std::string val = read_token();
      dump_debug_file = (val=="1" or val=="true");
      _expect_token(read_token(), ";");
    }

    else if(token == "use_sim_gridflow") {
      _expect_token(read_token(), "=");
      std::string val = read_token();
      use_sim_gridflow = (val=="1" or val=="true");
      _expect_token(read_token(), ";");
    }

  /* Wall effect */
    else if(token == "adv_wall_effect_enabled") {
      _expect_token(read_token(), "=");
      std::string val = read_token();
      adv_wall_effect_enabled = (val=="1" or val=="true");
      _expect_token(read_token(), ";");
    }

    else if(token == "adv_wall_effect_collision_threshold") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      adv_wall_effect_collision_threshold = val;
    }

    else if(token == "adv_wall_effect_candidate_threshold") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      adv_wall_effect_candidate_threshold = val;
    }

    else if(token == "adv_wall_effec_extra_shift") {
      _expect_token(read_token(), "=");
      float val = 0;
      if(!read_float(val)) {
        ok = false;
        break;
      }
      adv_wall_effect_extra_shift = val;
    }

    else if(token == "debug_adv_wall_effect_consistency") {
      _expect_token(read_token(), "=");
      std::string val = read_token();
      debug_adv_wall_effect_consistency = (val=="1" or val=="true");
      _expect_token(read_token(), ";");
    }

    else if(token == "debug_step") {
      _expect_token(read_token(), "=");
      _expect_token(read_token(), "[");

      int val;
      while(!_is_end()) {
        if(!read_int(val)) {
          ok = false;
          break;
        }
        debug_step.insert(val);
        token = read_token();
        if(token == "]") break;
        else _expect_token(token, ",");
      }
      _expect_token(read_token(), ";");

    }

    else {
      LOG(WARNING) << "Unknown variable: " << token << '\n';
      while(!_is_end()) {
        token = read_token();
        LOG(WARNING) << "Ignore " << token << '\n';
        if(token == ";") break;
      }
    }
  }

  if(!ok) {
    LOG(ERROR) << "Cannot parse setting: " << path << '\n';
    return false;
  }
  return true;
}

bool 
Simulate::Setting::_is_comment_prefix()
{
  if(_read_cur < _length) {
    return ( _buffer[_read_cur] == '%');
  }
  return false;
};

std::string 
Simulate::Setting::read_comment() 
{
  char c;
  std::string token;
  if(_read_cur < _length) {
    // %
    if(_buffer[_read_cur] == '%') {
      _get_char();
      while(!_is_end()) {
        c = _get_char();
        if(c=='\n') break;
        token.push_back(c);
      }
    } 
  }

  return token;
}

bool 
Simulate::Setting::read_string(std::string &res)
{
  while(!_is_end() and _is_sep_char()) _get_char();
  res = "";
  char c = _cur_char();
  if(c != '\"' and c != '\'') {
    LOG(WARNING) << "Cannot read string at " <<
      _filename << ":" << _line_no << '\n';
    return false;
  }

  bool ok = false;
  char bc = _get_char(); // '\"' or '\''
  while(!_is_end()) {
    c = _get_char();
    if(c == bc) {
      ok = true;
      break;
    }
    else res.push_back(c);
  }

  if(!ok) {
    LOG(WARNING) << "Fail to read string, expect '\"' at " <<
      _filename << ":" << _line_no << '\n';
  }

  return ok;
}

void 
Simulate::Setting::dump(std::ostream &os)
{
  os << "*************************\n";
  os << "  Setting        \n";
  os << "*************************\n";
  os << "Chip name: " << chip_name << '\n';
  os << "Ouptut folder: " << output_folder << '\n';
  os << "-------\n";
  os << "Simulation boundary x ratio: " << sim_boundary_x_ratio << '\n';
  os << "Simulation maximum time step: " << max_timestep << '\n';
  os << "-------\n";
  os << "Partical initial position shift: " << 
    to_string(init_position_shift) << '\n';
  os << "Partical initial velocity: (" << init_v.vx << ", " << init_v.vy << ")\n";
  os << "Partical diamter: ";
  for(const auto& val : dPs) os << val << " "; 
  os << '\n';
  os << "-------\n";
  os << "Time resolution: " << time_resolution << '\n';
  os << "alpha: " << alpha << '\n';
  os << "beta: " << beta << '\n';
  os << "-------\n";
  os << "Use grid flow: "   << use_sim_gridflow << '\n';
  os << "-------\n";
  os << "adv_wall_effect_enabled: " << adv_wall_effect_enabled << '\n';
  os << "adv_wall_effect_collision_threshold: " 
    << adv_wall_effect_collision_threshold<< '\n';
  os << "adv_wall_effect_candidate_threshold: " 
    << adv_wall_effect_candidate_threshold << '\n';
  os << "adv_wall_effect_extra_shift: " << 
   adv_wall_effect_extra_shift << '\n';
  os << "debug_adv_wall_effect_consistency: " << 
   debug_adv_wall_effect_consistency << '\n';
  os << "-------\n";
  os << "Dump debug file: " << dump_debug_file << '\n';
  os << "*************************\n";
  os << '\n';
}

/**************************************
  Class Simulate
 **************************************/
Simulate::Simulate(
  Chip *chip, 
  Setting * setting)
  : 
    _chip(chip)
  , _setting(setting)
  , _particle(nullptr)
  , _flowgrids(nullptr)
  , _debug_wall_effect(false)
{
  if(!std::filesystem::exists(_setting->output_folder)) {
    std::filesystem::create_directories(_setting->output_folder);
  }
}

Simulate::~Simulate()
{
  delete _flowgrids;
}

bool 
Simulate::_build_flow_grids()
{
  double max_dp = 0;
  for(const auto & val : _setting->dPs) max_dp = std::max(max_dp, val);

  _flowgrids = new SimFlowGrid(_chip, max_dp);

  return _flowgrids->build();
}

bool 
Simulate::simulate()
{
  LOG(INFO) << "====== Start simulating " << _setting->dPs.size() 
    << " different diameter. ======\n";

  if(_setting->use_sim_gridflow and !_build_flow_grids()) return false;

  bool ok = true;
  for(const auto &dp : _setting->dPs) {

    mopsa::point p = {
      _chip->design().min_x(), 
      (_chip->design().min_y() + _chip->design().max_y())/2
    };
    p = p + _setting->init_position_shift;

    mopsa::Particle particle(p, dp, 200);

    _particle = &particle;
    if(!_simulate_low()) {
      ok = false;
      break;
    }
    _particle = nullptr;
  }

  return ok;
}

bool 
Simulate::_simulate_low()
{
  std::vector<point> trajectory;
  double sim_boundary_x = 0;
  velocity pre_vel;
  bool dump_debug_file = _setting->dump_debug_file;
  FILE* debug_fp = nullptr;
  std::vector<int> *covered_nodes_id = nullptr;

  printf("\n");
  LOG(INFO) << "Particle starts at " << to_string(_particle->coord()) 
    << ", diameter = " << _particle->diameter() << '\n';

  trajectory.push_back(_particle->coord());

  sim_boundary_x = _setting->sim_boundary_x_ratio * _chip->design().width();

  pre_vel = _setting->init_v;

  if(dump_debug_file) {
    char name[100];
    sprintf(name, "debug_%s_%s.log", _setting->chip_name.c_str(), 
      to_string(_particle->diameter()).c_str());
    debug_fp = fopen(name, "w");
    covered_nodes_id = new std::vector<int>;
    LOG(INFO) << "Dump debug file to " << name << '\n';
  }

  _current_sim_step = 0;
  while(true) 
  {
    _current_sim_step += 1;
    bool debug = _is_debug_step(_current_sim_step);

    _debug_wall_effect = debug;

    velocity vel = _cal_particle_velocity(covered_nodes_id);

    vel.vx *= _setting->alpha;
    vel.vy *= _setting->beta;

    if(float_equal(vel.vx, 0) and float_equal(vel.vy, 0)) {
      vel = pre_vel;
    }

    if(dump_debug_file) {
      //std::sort(covered_nodes_id->begin(), covered_nodes_id->end());
      fprintf(debug_fp, "TimeStep: %d\n", _current_sim_step);
      fprintf(debug_fp, "Current position: %f %f\n", 
        _particle->coord().x(), _particle->coord().y()
      );
      fprintf(debug_fp, "Current velocity: %f %f\n", vel.vx, vel.vy);
      fprintf(debug_fp, "covered nodes:\n");
      for(const auto & id : *covered_nodes_id) {
        auto & node = _chip->flow().nodes()[id];
        fprintf(debug_fp, "%f %f\n", node.coord.x(), node.coord.y());
      }
    }
    pre_vel = vel;

    point new_coord = _apply_velocity(_particle, vel);
    (void)new_coord;

    point after_wall = _apply_wall_effect(_particle); 
    (void)after_wall;

    trajectory.push_back(_particle->coord());
    
    if(_current_sim_step > _setting->max_timestep) {
      printf("\n");
      LOG(INFO) << "Stop due to simulation step > " 
        << _setting->max_timestep << '\n';
      break;
    }

    if(_particle->coord().x() >= sim_boundary_x) {
      printf("\n");
      LOG(INFO) << "Stop due to x >= bondary: " 
        << sim_boundary_x << '\n';
      break;
    }

    printf("step = %d, coord = %s\r", _current_sim_step, 
        to_string(_particle->coord()).c_str());
    std::fflush(stdout);

    if(debug) { 
      std::filesystem::path debug_file = 
        "./debug_" + std::to_string(_current_sim_step) + ".txt";

      _dump(trajectory, debug_file);

      std::cout << "\nPlease check" << std::endl;
      int a; std::cin >> a;
    }
  }

  if(dump_debug_file) {
    fclose(debug_fp);
    delete covered_nodes_id;
  }


  LOG(INFO) << "Particle ends at " << to_string(_particle->coord()) << " "
    << " diameter = " << _particle->diameter() << '\n';

  // Dump output
  std::filesystem::path output_path = _setting->output_folder
    / ("cpp_" + _setting->chip_name + "_" 
        + to_string(_particle->diameter())+ ".txt");

  _dump(trajectory, output_path);

  return true;
}

velocity 
Simulate::_cal_particle_velocity(std::vector<int> *covered_nodes_id)
{
  velocity vel(0, 0);
  int cnt = 0;
  FlowBlockGroup block_group;
  int nodes_sum = 0;

  constexpr int MAX_SUPPORT_THREAD = 16;
  static    int MAX_THREAD = std::thread::hardware_concurrency();

  if(covered_nodes_id) covered_nodes_id->clear();

  if(_setting->use_sim_gridflow) {
    _flowgrids->get_adjacent_blocks(_particle, block_group);

    nodes_sum = block_group.size(FlowBlock::Type::flow_node);

    static int      cnt_t[MAX_SUPPORT_THREAD];
    static velocity vel_t[MAX_SUPPORT_THREAD];

    int used_thread_num = std::min(MAX_THREAD, MAX_SUPPORT_THREAD);
    if(nodes_sum <= 400) used_thread_num = 1;

    //else if(nodes_sum <= 1000) used_thread_num = 2;
    //else if(nodes_sum <= 2000) used_thread_num = 4;
    //else if(nodes_sum <= 4000) used_thread_num = 8;
    //else if(nodes_sum <= 8000) used_thread_num = 16;

    omp_set_num_threads(used_thread_num);

    for(int i=0; i<used_thread_num; i++) {
      cnt_t[i]    = 0;
      vel_t[i].vx = 0;
      vel_t[i].vy = 0;
    }

    #pragma omp parallel for
    for(int i=0; i<nodes_sum; i++) {
      int tid      = omp_get_thread_num();
      int node_id  = block_group.get(FlowBlock::Type::flow_node, i);

      const auto & node = _chip->flow().nodes()[node_id];
      bool covered      = _particle->cover(node.coord);

      if(covered) {
        if(covered_nodes_id) covered_nodes_id->push_back(i);
        cnt_t[tid]    += 1;
        vel_t[tid].vx += node.vx;
        vel_t[tid].vy += node.vy;
      }
    }

    for(int i=0; i<used_thread_num; i++) {
      cnt    += cnt_t[i];
      vel.vx += vel_t[i].vx;
      vel.vy += vel_t[i].vy;
    }

    //Logger::add_record(std::to_string(_particle->diameter()), 
        //"iterate nodes", nodes_sum);

    //Logger::add_record("Veolocity", 
      //to_string(_particle->diameter()) + " # of blocks " 
        //+ std::to_string(blocks.size()),  1
    //);

    //Logger::add_record("Veolocity", 
      //to_string(_particle->diameter()) + " total nodes iterations", nodes_sum
    //);
  }
  else {

    #pragma omp parallel for
    for(size_t i = 0; i<_chip->flow().nodes().size(); i++) {
      const auto & node = _chip->flow().nodes()[i];
      bool covered = _particle->cover(node.coord);

      if(covered) {
        #pragma omp critical
        {
          if(covered_nodes_id) covered_nodes_id->push_back(i);
          cnt += 1;
          vel.vx += node.vx;
          vel.vy += node.vy;
        }
      }
    }
  }

  if(cnt > 0) {
    vel.vx /= cnt;
    vel.vy /= cnt;
  }
  else { 
    /* 
     * if the particle doesn't cover any nodes, we will set the velocity 
     * to the nodes the most cloest to particle. 
     */
    static double distance_t[MAX_SUPPORT_THREAD]  = {0};
    static int    node_id_t[MAX_SUPPORT_THREAD];

    int used_thread_num = std::min(MAX_THREAD, MAX_SUPPORT_THREAD);
    if(_setting->use_sim_gridflow) {
      if(nodes_sum <= 400) used_thread_num = 1;
    }
    omp_set_num_threads(used_thread_num);

    for(int i=0; i<used_thread_num; i++) {
      distance_t[i] = 1e10;
      node_id_t[i] = 0;
    }

    if(_setting->use_sim_gridflow) {
      #pragma omp parallel for
      for(int i=0; i<nodes_sum; i++) {
        
        int tid      = omp_get_thread_num();
        int node_id  = block_group.get(FlowBlock::Type::flow_node, i);

        const auto & node = _chip->flow().nodes()[node_id];

        double dist = boost::geometry::distance(_particle->coord(), node.coord);
        if(distance_t[tid] > dist) {
          distance_t[tid] = dist;
          node_id_t[tid]  = node_id;
        }
      }
    }
    else {
      #pragma omp parallel for
      for(size_t i = 0; i<_chip->flow().nodes().size(); i++) {
        int tid = omp_get_thread_num();

        const auto & node = _chip->flow().nodes()[i];
        double dist = boost::geometry::distance(_particle->coord(), node.coord);
        if(distance_t[tid] > dist) {
          distance_t[tid] = dist;
          node_id_t[tid]  = i;
        }
      }
    }

    // Merge results
    int node_id = -1;
    double distance = 1e10;
    for(int i=0; i<used_thread_num; i++) {
      if(distance_t[i] < distance) {
        distance = distance_t[i];
        node_id  = node_id_t[i];
      }
    }

    if(covered_nodes_id)  covered_nodes_id->push_back(node_id);
    vel.vx = _chip->flow().nodes()[node_id].vx;
    vel.vy = _chip->flow().nodes()[node_id].vy;
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
Simulate::_apply_wall_effect(Particle *particle)
{
  point new_coord;
  bool collision = false;

  if(_setting->adv_wall_effect_enabled) {
    collision = _apply_wall_effect_low_adv(particle, new_coord);
  }
  else {
    std::pair<point, point> two_points;
    double portion;
    collision = _apply_wall_effect_low(particle, two_points, portion);
    if(collision) {
      new_coord = _cal_wall_effect_position_based_on_A_B_porition(
        particle,
        two_points.first, two_points.second,
        portion
      );
    }
  }

  if(collision) {
    particle->update_coord(new_coord);

    Logger::add_record("Collision", to_string(particle->diameter()), 1);
  }

  return particle->coord();
}

point 
Simulate::_cal_wall_effect_position_based_on_A_B_porition(
  Particle *particle,
  point A,
  point B,
  double portion
)
{
  point middle       = (A + B) * 0.5;
  point direction    = particle->coord() - middle;
  point dist         = direction * (portion * _particle->radius());
  point new_position = particle->coord() + dist;

  return new_position;
}

bool
Simulate::_apply_wall_effect_low(
  /* input */
  Particle *particle,
  /* output */
  std::pair<point, point> &two_points,
  double &portion
)
{
  std::set<int> obstacles_cand;
  bool          collision = false;

  if(_setting->use_sim_gridflow) {
    FlowBlockGroup group_blocks;
    _flowgrids->get_adjacent_blocks(_particle, group_blocks);

    for(int i=0; i<group_blocks.size(FlowBlock::Type::obstacle); i++) {
      obstacles_cand.insert(group_blocks.get(FlowBlock::Type::obstacle, i));
    }
  }
  else {
    for(size_t i=0; i<_chip->design().obstacles().size(); i++) {
      obstacles_cand.insert(i);
    }
  }

  for(const auto & id : obstacles_cand) {

    const auto & obst = _chip->design().obstacles()[id];
    std::vector<point> res;
    if(particle->overlap_obstacle(obst, res, _debug_wall_effect)) {

      auto A = res.front();
      auto B = res.back();
      portion = (double) res.size() / 201.0;
      two_points = {A, B};
      collision = true;

      if(_debug_wall_effect) {
        point new_coord = _cal_wall_effect_position_based_on_A_B_porition(
          particle,
          A, B,
          portion
        );

        point middle       = (A + B) * 0.5;
        point direction    = particle->coord() - middle;

        std::cout << "!!!!" << portion << std::endl;
        std::cout << "Cov front = " << to_string(A) << 
          " back = " << to_string(B) << std::endl;
        std::cout << "Middle = " << to_string(middle) << std::endl;
        std::cout << "Direction = " << to_string(direction) << std::endl;

        std::cout << to_string(particle->coord()) << " ===> " 
          << to_string(new_coord) << std::endl;
        int a;
        std::cin >> a;
      }
    }
  }

  return collision;
}

bool
Simulate::_apply_wall_effect_low_adv(
  Particle *particle,
  point & adv_new_position
)
{
  ASSERT(_setting->use_sim_gridflow == true);

  FlowBlockGroup group_blocks;
  int num_obstacle_nodes = 0;
  std::vector<point> points_on_circumference;
  std::vector<point> points_in_particle;
  std::vector<point> candidate_points;
  point adv_A(0, 0), adv_B(0, 0);
  bool adv_collision = false;
  bool adv_collision_with_candidate = false;;
  bool adv_collsion_by_candidate = false;

  double collision_threshold 
    = _setting->adv_wall_effect_collision_threshold;
  double candidate_points_threshold 
    = _setting->adv_wall_effect_candidate_threshold;
  double extra_shift 
    = _setting->adv_wall_effect_extra_shift;

  _flowgrids->get_adjacent_blocks(_particle, group_blocks);
  num_obstacle_nodes = group_blocks.size(FlowBlock::Type::obstacle_node);

  for(int i=0; i<num_obstacle_nodes; i++) {
    int node_id = group_blocks.get(FlowBlock::Type::obstacle_node, i);
    const auto & node = _chip->flow().node(node_id);

    double dist = boost::geometry::distance(_particle->coord(), node.coord);
    dist = (dist - particle->radius());

    if(fabs(dist) <= collision_threshold) {
      points_on_circumference.push_back(node.coord);
    }

    else if(dist > 0 and dist < candidate_points_threshold) {
      candidate_points.push_back(node.coord);
    }

    else if(dist < 0) {
      points_in_particle.push_back(node.coord);
    }
  }

  // Choose A B point
  if(points_on_circumference.size() == 1) {
    adv_B = adv_A = points_on_circumference.front();
    adv_collision = true;
  }
  else if(points_on_circumference.size() == 2) {
    adv_A = points_on_circumference.front();
    adv_B = points_on_circumference.back();
    adv_collision = true;
  }
  else if(points_on_circumference.size() > 3) {
    LOG(WARNING) << "Over two points on circumference.\n";

    adv_collision = true;
    std::sort(points_on_circumference.begin(), points_on_circumference.end(),
      [&](const point & a, const point &b)
      {
        point vec_a = a - _particle->coord();
        point vec_b = b - _particle->coord();

        double degree_a = std::atan2(vec_a.y(), vec_a.x());
        double degree_b = std::atan2(vec_b.y(), vec_b.x());

        return degree_a < degree_b;
      }
    );
    adv_A = points_on_circumference.front();
    adv_B = points_on_circumference.back();
  }
  else {
    if(points_in_particle.size()) {
      LOG(WARNING) << "Time resolution is too large."
        " Cannot detect wall during simulation. "
        "current step = \n" << _current_sim_step;
    }

    // Choose A B point from candidate_points
    if(candidate_points.size() > 1) {
      std::sort(candidate_points.begin(),candidate_points.end(),
        [&](const point & a, const point &b)
        {
          double dista = boost::geometry::distance(_particle->coord(), a);
          double distb = boost::geometry::distance(_particle->coord(), b);
          return dista < distb;
        }
      );
      point middle = 
        (candidate_points.front() + candidate_points.back())*0.5;

      double dist = boost::geometry::distance(_particle->coord(), middle);
      dist = (dist - particle->radius());

      if(fabs(dist) <= collision_threshold) {
        adv_collision = true;
        adv_collision_with_candidate = true;
        adv_A = adv_B = middle;
      }

      adv_collsion_by_candidate = true;
    }
  }

// Calculate new position
  if(adv_collision) {
    point middle = (adv_A + adv_B) * 0.5;
    point dir = _particle->coord() - middle;

    double len = boost::geometry::distance(dir, point(0,0));

    double dist = (_particle->radius() - len) + extra_shift;

    if(dist < 0) adv_collision = false;
    else adv_new_position = _particle->coord() + dir * (dist/len);
  }

  if(!adv_collision) adv_new_position = _particle->coord();

  bool consistency_check = false;
  consistency_check = 
    _setting->debug_adv_wall_effect_consistency | _debug_wall_effect;

// Consistency check
  if(consistency_check) {
    point error;
    bool adv_large_shift = false;
    Particle tmp_particle = *particle;
    Particle adv_particle = *particle;
    Particle golden_particle = *particle;

    adv_particle.update_coord(adv_new_position);

  // Calculate golden position
    std::pair<point, point> golden_two_points;
    point golden_A, golden_B;
    point golden_new_position;
    bool golden_collision = false;
    double golden_portion = 0;

    golden_collision = _apply_wall_effect_low(particle, 
      golden_two_points, golden_portion
    );

    golden_A = golden_two_points.first;
    golden_B = golden_two_points.second;

    golden_new_position = golden_collision? 
      _cal_wall_effect_position_based_on_A_B_porition(
        particle,
        golden_A, golden_B,
        golden_portion
      )
      :
      particle->coord()
    ;
    golden_particle.update_coord(golden_new_position);

    error = adv_new_position - golden_new_position;

    if(adv_collision)
      adv_large_shift = _is_large_shift(adv_new_position, particle->coord());

  // Check new position is collided.
    bool adv_new_pos_collision = false;
    if(adv_collision) {
      tmp_particle.update_coord(adv_new_position);
      double tmp_porition = 0;
      std::pair<point, point> tmp_two_points;
      adv_new_pos_collision = _apply_wall_effect_low(&tmp_particle, 
          tmp_two_points, tmp_porition);
    }

  // should we dump debug file
    bool debug = 
  (fabs(error.x()) >= 0.2 or fabs(error.y()) >= 0.2) 
        or adv_new_pos_collision
  or _debug_wall_effect
  or adv_large_shift
        or adv_collision_with_candidate 
  //or adv_A != adv_B
       //or golden_collision ^ adv_collision
    ;

    if(debug) 
    {
      std::cout << "\n";
      std::cout << "Particle coord = " 
  << to_string(_particle->coord()) << std::endl;
      std::cout << "Adv large shift = " << adv_large_shift << std::endl;
      std::cout << "Adv New position collision = " << adv_new_pos_collision 
  << std::endl;
      std::cout << "Adv collsion by candidate = " << 
        adv_collsion_by_candidate << std::endl;
      std::cout << "Adv points_on_circumference.size() = " <<
        points_on_circumference.size() << std::endl;
      std::cout << "Adv candidate_points.size() = " 
        << candidate_points.size() << std::endl;
      std::cout << "Adv collision with candidate points: " << 
        adv_collision_with_candidate << std::endl;

      std::cout << "Golden:\n";
      std::cout << "\tCollision : " << golden_collision << std::endl;
      std::cout << "\tA         : " << to_string(golden_A) << std::endl;
      std::cout << "\tB         : " << to_string(golden_B) << std::endl;
      std::cout << "\tNew coord : " << to_string(golden_particle.coord()) 
  << std::endl;
      std::cout << "Advance collision:\n";
      std::cout << "\tCollision : " << adv_collision << std::endl;
      std::cout << "\tA         : " << to_string(adv_A) << std::endl;
      std::cout << "\tB         : " << to_string(adv_B) << std::endl;
      std::cout << "\tNew coord : " << to_string(adv_new_position)
  << std::endl;
      std::cout << "===================================\n";
      std::cout << "Error: " <<  to_string(error) << std::endl << std::endl;

      point middle = (adv_A + adv_B) * 0.5;
      std::cout << "dist(Particle,middle) = " 
        << boost::geometry::distance(middle, _particle->coord()) << std::endl;

      for(auto point : candidate_points) {
        double dista = boost::geometry::distance(_particle->coord(), point);
        std::cout << dista << std::endl;
      }

      if(adv_collision) 
      {
        if(adv_collsion_by_candidate == false) {
          point middle = (adv_A + adv_B) * 0.5;
          double dist = boost::geometry::distance(middle, _particle->coord());
          std::cout << "dist(Particle, middle) = " << dist << std::endl;
          std::cout << "Move distacnce = " 
            << _particle->radius() - dist << std::endl;
        }
        else {
          point cand_middle = 
            (candidate_points.front() + candidate_points.back()) *0.5;
          std::cout << "dist(Particle,cand_middle) = " 
            << boost::geometry::distance(cand_middle, _particle->coord()) << std::endl;
        }
      }

      // dump debug information
      std::ofstream out("debug_advance_collision.txt");
      particle->dump(out);
      adv_particle.dump(out);
      golden_particle.dump(out);
      group_blocks.dump_obstacle_nodes(_chip, out);
      out << golden_collision << std::endl;
      out << golden_A.x() << " " << golden_A.y() << std::endl;
      out << golden_B.x() << " " << golden_B.y() << std::endl;
      out << adv_collision << std::endl;
      out << adv_A.x() << " " << adv_A.y() << std::endl;
      out << adv_B.x() << " " << adv_B.y() << std::endl;
      out.close();

      //for(auto [dist, id] : nodes_distance) {
  //std::cout << dist << ", " << id << "\n";
      //}
      int a;
      std::cin >> a;
    }
  }

  return adv_collision;
}

//double 
//Simulate::_cal_adv_wall_effect_portion(
  //Particle *particle,
  //point A,
  //point B
//)
//{
  //if(A == B) {
    //double distance = boost::geometry::distance(particle->coord(), A);
    //double diff = distance - particle->radius();
    //if(diff < 0) {
      //return std::fabs(diff) / (particle->radius() * std::fabs(distance));
    //}
    //return 1.0/201;
  //}
  //else {
    ////TODO:
    //Logger::add_record("Adv collision", "Unable to calculate portion", 1);
    //return 1.0/201;
  //}
//}

bool 
Simulate::_is_large_shift(
  point new_coord, 
  point old_coord
)
{
  point diff = new_coord - old_coord;
  return (std::fabs(diff.x()) >= 0.3) || (std::fabs(diff.y()) >= 0.3);
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
