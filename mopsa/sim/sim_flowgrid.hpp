#ifndef MOPSA_SIM_SIM_FLOW_GRID_H
#define MOPSA_SIM_SIM_FLOW_GRID_H

#include <cmath>
#include <mopsa/chip/chip.hpp>
#include <mopsa/util/util_funcs.h>
#include <iostream>

namespace mopsa
{

struct FlowBlock
{
  point lower_corner;
  point upper_corner; 
  std::vector<int> nodes; // nodes id in Flow
  std::set<int> obstacles;
};

class Particle;
class SimFlowGrid
{

public:

  SimFlowGrid(Chip *chip, double len);

  ~SimFlowGrid();
  
  bool build();

  inline FlowBlock* get_blocks(const point & node);

  void get_adjacent_blocks(Particle *, std::vector<FlowBlock*>& );

private:

  inline std::pair<int, int> _points_to_grid_xy(const point & node);

  inline FlowBlock * _get_flow_block(int x, int y);

  inline bool _is_valid_grid_xy(int gridx, int gridy);

private:
  Chip *_chip;
  double _grid_len;

  int _num_row;
  int _num_column;
  int _num_blocks;

  FlowBlock *_flowgrid;
  //std::vector<std::vector<*FlowBlock>> _flowgrid;
};

/**************************************
  Inline Function
 **************************************/
inline FlowBlock *
SimFlowGrid::get_blocks(const point & node)
{
  auto [gridx, gridy] = _points_to_grid_xy(node);

  return _get_flow_block(gridx, gridy);
}

inline FlowBlock *
SimFlowGrid::_get_flow_block(int gridx, int gridy)
{
  if(!_is_valid_grid_xy(gridx, gridy)) return nullptr;

  return &_flowgrid[ gridy * _num_column + gridx];
}

inline std::pair<int, int> 
SimFlowGrid::_points_to_grid_xy(const point & node)
{
  auto node_shifted = point_minus(node,
    {_chip->design().min_x(), _chip->design().min_y()}
  );

  if(float_equal(node_shifted.x(), 0))  node_shifted.x(0);
  if(float_equal(node_shifted.y(), 0))  node_shifted.y(0);

  return {std::floor(node_shifted.x() / _grid_len), 
    std::floor(node_shifted.y() / _grid_len)};
}

inline bool 
SimFlowGrid::_is_valid_grid_xy(int gridx, int gridy)
{
  if(gridx < 0 or gridx >= _num_column) return false;

  if(gridy < 0 or gridy >= _num_row) return false;

  return true;
}

}

#endif
