#include <boost/math/special_functions/math_fwd.hpp>
#include <boost/geometry/algorithms/detail/distance/interface.hpp>

#include <mopsa/util/util_funcs.h>
#include <mopsa/geometry/point.hpp>
#include <mopsa/sim/sim.hpp>
#include <mopsa/logger/logger.hpp>

#include <omp.h>

#include <filesystem>
#include <iostream>
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
  os << "Time resoltion: " << time_resolution << '\n';
  os << "alpha: " << alpha << '\n';
  os << "beta: " << beta << '\n';
  os << "-------\n";
  os << "Dump debug file: " << dump_debug_file << '\n';
  os << "Use Grid flow: " << use_sim_gridflow << '\n';
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

    mopsa::point p = {_chip->design().min_x(), 
      (_chip->design().min_y() + _chip->design().max_y())/2};
    p = mopsa::point_add(p, _setting->init_position_shift);

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
  int step_count = 0;
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

  while(true) 
  {
    step_count += 1;
    bool debug = _is_debug_step(step_count);

    _debug_wall_effect = debug;

    velocity vel = _cal_particle_velocity(covered_nodes_id);

    vel.vx *= _setting->alpha;
    vel.vy *= _setting->beta;

    if(float_equal(vel.vx, 0) and float_equal(vel.vy, 0)) {
      vel = pre_vel;
    }

    if(dump_debug_file) {
      //std::sort(covered_nodes_id->begin(), covered_nodes_id->end());
      fprintf(debug_fp, "TimeStep: %d\n", step_count);
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

    point after_wall = _apply_well_effect(_particle); 
    (void)after_wall;

    trajectory.push_back(_particle->coord());
    
    if(step_count > _setting->max_timestep) {
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
  std::vector<FlowBlock*> blocks;
  int nodes_sum = 0;

  constexpr int MAX_SUPPORT_THREAD = 16;
  static    int MAX_THREAD = std::thread::hardware_concurrency();

  if(covered_nodes_id) covered_nodes_id->clear();

  if(_setting->use_sim_gridflow) {
    _flowgrids->get_adjacent_blocks(_particle, blocks);

    for(size_t i = 0; i<blocks.size(); i++) 
      nodes_sum += blocks[i]->nodes.size();

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

      size_t cur_i = i;
      int node_id  = -1;
      int tid      = omp_get_thread_num();

      for(size_t j = 0; j<blocks.size(); j++) {
        if(blocks[j]->nodes.size() == 0) continue;
        if(blocks[j]->nodes.size() <= cur_i)  {
          cur_i -= blocks[j]->nodes.size();
        }
        else {
          node_id = blocks[j]->nodes[cur_i];
          break;
        }
      }

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
        size_t cur_i = i;
        int node_id  = -1;
        int tid      = omp_get_thread_num();

        for(size_t j = 0; j<blocks.size(); j++) {
          if(blocks[j]->nodes.size() == 0) continue;
          if(blocks[j]->nodes.size() <= cur_i)  {
            cur_i -= blocks[j]->nodes.size();
          }
          else {
            node_id = blocks[j]->nodes[cur_i];
            break;
          }
        }
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
Simulate::_apply_well_effect(Particle *particle)
{
  point coord = particle->coord();

  std::set<int> obstacles_cand;

  if(_setting->use_sim_gridflow) {
    std::vector<FlowBlock*> blocks;
    _flowgrids->get_adjacent_blocks(_particle, blocks);

    for(size_t i=0; i<blocks.size(); i++) {
      for(const auto & id : blocks[i]->obstacles) {
        obstacles_cand.insert(id);
      }
    }

    //Logger::add_record("Wall Effect", 
      //to_string(_particle->diameter()) + " total obstacles check", 
      //obstacles_cand.size()
    //);
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
