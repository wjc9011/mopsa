#ifndef MOPSA_CHIP_FLOW_H
#define MOPSA_CHIP_FLOW_H

#include <vector>
#include <filesystem>

#include <mopsa/headerdef.hpp>
#include <mopsa/util/reader.hpp>

namespace mopsa
{

class Flow : protected Reader
{

public:
  struct Node
  {
    Node() : coord(0, 0), vx(0), vy(0) {}
    
    Node(point p, double vx, double vy) : 
      coord(p), vx(vx), vy(vy) {}

    point coord;
    double vx, vy;
  };


  Flow() : Reader("%,", " \r\t\n") {}

  bool load_flow(const std::filesystem::path &file_x, 
      const std::filesystem::path &file_y);

  inline int size() const;

  inline const auto & nodes() const;

  inline const Node & node(int i) const;

private:

  std::string read_comment() override;

  bool _is_comment_prefix() override;

private:

  std::vector<Node> _nodes;

};

/**************************************
  Inline Function
 **************************************/

inline int 
Flow::size() const
{
  return _nodes.size();
}

inline const auto &
Flow::nodes() const
{
  return _nodes;
}

inline const Flow::Node & 
Flow::node(int i) const
{
  return _nodes[i];
}

}
#endif
