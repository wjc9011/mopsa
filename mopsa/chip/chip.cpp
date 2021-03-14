#include <cstdlib>
#include <mopsa/chip/chip.hpp>
#include <mopsa/logger/logger.hpp>
#include <mopsa/geometry/point.hpp>
#include <mopsa/chip/flow.hpp>
#include <mopsa/headerdef.hpp>

namespace mopsa
{

Chip::Chip()
{

}

bool 
Chip::load_design(const std::filesystem::path &design_path)
{
  LOG(INFO) << "Load chip design from " << design_path << '\n';
  bool res = _design.load_design(design_path);

  Logger::add_record("chip", "# of obstacles", _design.num_obstacles());
  Logger::add_record("chip", "width:", _design.width());
  Logger::add_record("chip", "height:", _design.height());
  Logger::add_record("chip", "upper left : "
     + to_string(_design.upper_left()), 1);
  Logger::add_record("chip", "lower right : " 
      + to_string(_design.lower_right()), 1);

  return res;
}

bool 
Chip::load_flow(
  const std::filesystem::path &flow_x_path,
  const std::filesystem::path &flow_y_path
)
{
  LOG(INFO) << "Load flow from x: " << flow_x_path 
    << ", y: " << flow_y_path << '\n';

  bool res = _flow.load_flow(flow_x_path, flow_y_path);

  Logger::add_record("chip", "# of flow nodes", _flow.size());

  LOG(INFO) << _flow.size() << " nodes are loaded.\n";
  return res;
}


}
