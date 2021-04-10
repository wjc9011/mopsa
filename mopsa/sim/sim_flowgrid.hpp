#ifndef MOPSA_SIM_SIM_FLOW_GRID_H
#define MOPSA_SIM_SIM_FLOW_GRID_H

#include <mopsa/chip/chip.hpp>
#include <mopsa/util/util_funcs.h>
#include <mopsa/logger/debug.hpp>

#include <cmath>
#include <iostream>
#include <optional>
#include <vector>

namespace mopsa
{

class Particle;

class FlowBlock
{

  friend class SimFlowGrid;

public:

  enum class Type : int
  {
    flow_node,
    obstacle_node,
    obstacle,
    last
  };

  static constexpr int type_size = static_cast<int>(Type::last);

  inline point lower_corner() const;

  inline point upper_corner() const;

  inline std::vector<int> const * get(FlowBlock::Type type) const;

private:

  void _make_obstacle_unique();

  point _lower_corner;
  point _upper_corner; 
  std::vector<int> _nodes;          // nodes id in Flow
  std::vector<int> _obstacle_nodes; 
  std::vector<int> _obstacles;
};

class FlowBlockGroup
{

public:

  FlowBlockGroup();

  inline void append(FlowBlock* block);

  inline void clear();

  inline int get(FlowBlock::Type type, int id) const;

  inline int size(FlowBlock::Type type) const;

private:

  std::vector<FlowBlock*> _blocks;

  std::array<int, FlowBlock::type_size> _size;
};

class SimFlowGrid
{

public:

  SimFlowGrid(Chip *chip, double len);

  ~SimFlowGrid();
  
  bool build(bool with_design_obstacle);

  inline FlowBlock* get_blocks(const point & node);

  void get_adjacent_blocks(Particle *, std::vector<FlowBlock*>& );

  void get_adjacent_blocks(Particle *, FlowBlockGroup &group);

private:

  inline std::pair<int, int> _node_to_grid_xy(const point & node);

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

/******************
  FlowBlock
 ******************/
inline point
FlowBlock::lower_corner() const
{
  return _lower_corner;
}

inline point
FlowBlock::upper_corner() const
{
  return _upper_corner;
}

inline std::vector<int> const *
FlowBlock::get(FlowBlock::Type type) const
{
  switch (type)
  {
    case Type::flow_node:
      return &_nodes;

    case Type::obstacle_node:
      return &_obstacle_nodes;

    case Type::obstacle:
      return &_obstacles;

    case Type::last:
      ASSERT(false);
  }

  return nullptr;
}

/******************
  FlowBlockGroup
 ******************/

inline void 
FlowBlockGroup::append(FlowBlock* block)
{
  _blocks.push_back(block);

  for(int i=0; i<FlowBlock::type_size; i++) {
    auto * data = block->get(static_cast<FlowBlock::Type>(i)); 
    ASSERT(data != nullptr);
    _size[i] += data->size();
  }
}

inline void 
FlowBlockGroup::clear()
{
  _blocks.clear();

  for(int i=0; i<FlowBlock::type_size; i++) {
    _size[i] += 0;
  }
}

inline int
FlowBlockGroup::size(FlowBlock::Type type) const
{
  return _size[static_cast<int>(type)];
}

inline int
FlowBlockGroup::get(FlowBlock::Type type, int id) const
{
  size_t cur_id  = id;
  int    node_id = 0;
  for(size_t i=0; i<_blocks.size(); i++) {
    auto *data = _blocks[i]->get(type);

    if(data->size() == 0) continue;
    if(data->size() <= cur_id) cur_id -= data->size();
    else {
      node_id = data->at(cur_id);
      break;
    }
  }

  return node_id;
}

/******************
  FlowBlock
 ******************/
inline FlowBlock *
SimFlowGrid::get_blocks(const point & node)
{
  auto [gridx, gridy] = _node_to_grid_xy(node);

  return _get_flow_block(gridx, gridy);
}

inline FlowBlock *
SimFlowGrid::_get_flow_block(int gridx, int gridy)
{
  if(!_is_valid_grid_xy(gridx, gridy)) return nullptr;

  return &_flowgrid[ gridy * _num_column + gridx];
}

inline std::pair<int, int> 
SimFlowGrid::_node_to_grid_xy(const point & node)
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
