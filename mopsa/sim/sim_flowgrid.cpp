#include <algorithm>
#include <cmath>
#include <math.h>
#include <mopsa/sim/sim_flowgrid.hpp>
#include <mopsa/sim/particle.hpp>
#include <mopsa/chip/chip.hpp>
#include <mopsa/logger/logger.hpp>
#include <mopsa/util/util_funcs.h>

namespace mopsa
{

/******************
  FlowBlock
 ******************/
void 
FlowBlock::_make_obstacle_unique()
{
  std::sort(_obstacles.begin(), _obstacles.end());
  _obstacles.resize(
    std::unique(_obstacles.begin(), _obstacles.end()) - _obstacles.begin()
  );
}

/******************
  FlowBlockGroup
 ******************/

FlowBlockGroup::FlowBlockGroup()
{
  _blocks.reserve(9);
  for(size_t i=0; i<_size.size(); i++) {
    _size[i] = 0;
  }
}

void 
FlowBlockGroup::dump_obstacle_nodes(Chip *chip, std::ostream &os)
{
  os << size(FlowBlock::Type::obstacle_node) << '\n';
  for(int i=0; i<size(FlowBlock::Type::obstacle_node); i++) {
    int node_id = get(FlowBlock::Type::obstacle_node, i);
    os << chip->flow().node(node_id).coord.x() << " " 
       << chip->flow().node(node_id).coord.y() << '\n';
  }
}

/******************
  SimFlowGrid
 ******************/

SimFlowGrid::SimFlowGrid(
  Chip *chip,
  double len
):
  _chip(chip),
  _grid_len(len)
{

  double width  = chip->width();
  double height = chip->height();

  _num_row    = std::ceil(height / _grid_len) + 1;
  _num_column = std::ceil(width  / _grid_len) + 1;
  _num_blocks = _num_row * _num_column;

  _flowgrid = new FlowBlock[_num_row * _num_column];

  for(int i=0; i<_num_row; i++) {
    for(int j=0; j<_num_column; j++) {
      auto *block = _get_flow_block(j, i);
      ASSERT(block != nullptr);
      block->_lower_corner = {i*_grid_len, j*_grid_len};
      block->_upper_corner = {(i+1)*_grid_len, (j+1)*_grid_len};
    }
  }

  LOG(INFO) << "Build SimFlogGrid: grid length = " << _grid_len
    << ", Grid Size: " << _num_column << " x " << _num_row << '\n';
}

SimFlowGrid::~SimFlowGrid()
{
  delete[] _flowgrid;
}

bool
SimFlowGrid::build()
{
  int node_id = -1;
  for(const auto & node : _chip->flow().nodes()) {
    node_id += 1;
    auto * block = get_blocks(node.coord);
    if(!block) {

      LOG(WARNING) << to_string(node.coord) << " is out of design: "
        << to_string(_chip->design().upper_left()) << " x " 
        << to_string(_chip->design().lower_right()) 
      << '\n';

      ASSERT( node.coord.x() < _chip->design().min_x()
           || node.coord.x() > _chip->design().max_x()
           || node.coord.y() < _chip->design().min_y()
           || node.coord.y() > _chip->design().max_y()
      );

      continue;
    }

    if(float_equal(node.vx, 0) and float_equal(node.vy, 0)) 
    {
      block->_obstacle_nodes.push_back(node_id);
    }
    else block->_nodes.push_back(node_id);
  }

  int obstacle_id = -1;
  for(const auto & obstacle: _chip->design().obstacles()) {
    obstacle_id += 1;
    for(const auto & node: obstacle.get_polygon().outer()) {
      auto * block = get_blocks(node);
      if(!block) {
  LOG(WARNING) << to_string(node) << " is out of design "
    << to_string(_chip->design().upper_left()) << " x " 
    << to_string(_chip->design().lower_right()) 
  << '\n';

  ASSERT( node.x() < _chip->design().min_x()
      || node.x() > _chip->design().max_x()
  );

  ASSERT( node.y() < _chip->design().min_y()
     || node.y() > _chip->design().max_y()
  );

  continue;
      }
      block->_obstacles.push_back(obstacle_id);
    }
  }

  size_t maximum_nodes = 0;
  size_t maximum_obstacle_nodes = 0;

  size_t nodes_sum = 0;
  size_t obstacle_node_sum = 0;
  for(int i=0; i<_num_blocks; i++) {

    nodes_sum	      += _flowgrid[i]._nodes.size();
    obstacle_node_sum += _flowgrid[i]._obstacle_nodes.size();

    maximum_nodes = 
      std::max(maximum_nodes, _flowgrid[i]._nodes.size());

    maximum_obstacle_nodes = 
      std::max(maximum_obstacle_nodes, _flowgrid[i]._obstacle_nodes.size());

    _flowgrid[i]._make_obstacle_unique();
  }

  Logger::add_record("FlowGrid", "# of rows",    _num_row);
  Logger::add_record("FlowGrid", "# of columns", _num_column);
  Logger::add_record("FlowGrid", "# of nodes", nodes_sum);
  Logger::add_record("FlowGrid", "# of obstacle nodes", obstacle_node_sum);
  Logger::add_record("FlowGrid", "Maximum # of nodes in block", maximum_nodes);
  Logger::add_record("FlowGrid", "Maximum # of obstacle  nodes in block", 
      maximum_obstacle_nodes);

  return true;
}

void 
SimFlowGrid::get_adjacent_blocks(
  Particle *particle, 
  std::vector<FlowBlock*> &blocks
)
{
  blocks.clear();
  auto [gridx, gridy] = _node_to_grid_xy(particle->coord());

  for(int i=-1; i<=1; i++) {
    for(int j=-1; j<=1; j++) {
      if(_is_valid_grid_xy(gridx + i, gridy + j))  {
        auto *block = _get_flow_block(gridx + i, gridy + j);
        blocks.push_back(block);
      }
    }
  }
}

void
SimFlowGrid::get_adjacent_blocks(
  Particle *particle,
  FlowBlockGroup &group
)
{
  group.clear();
  auto [gridx, gridy] = _node_to_grid_xy(particle->coord());

  for(int i=-1; i<=1; i++) {
    for(int j=-1; j<=1; j++) {
      if(_is_valid_grid_xy(gridx + i, gridy + j))  {
        auto *block = _get_flow_block(gridx + i, gridy + j);
        group.append(block);
      }
    }
  }
}

}
