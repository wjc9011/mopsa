#include <cstdlib>
#include <iostream>

#include <mopsa/logger/debug.hpp>
#include <mopsa/chip/flow.hpp>

namespace mopsa
{

bool
Flow::load_flow(
  const std::filesystem::path &file_x, 
  const std::filesystem::path &file_y
)
{
  open_file(file_x);
  while(!_is_end()) 
  {
    double x, y, v;
    std::string token = read_token();
    if(token.size() == 0) break;

    x = std::stod(token);
    if(!_expect_token(read_token(), ",")) return false;
    y = std::stod(read_token());
    if(!_expect_token(read_token(), ",")) return false;
    v = std::stod(read_token());
    _nodes.emplace_back(point(x, y), v, 0);

    //if(_nodes.size() <= 10) {
      //std::cout << to_string(_nodes.back().coord) << " "
  //<< _nodes.back().vx << '\n';
    //}
  }

  open_file(file_y);
  size_t cnt = 0;
  while(!_is_end()) 
  {
    double x, y, v;
    std::string token = read_token();
    if(token.empty()) break;

    x = std::stod(token);
    if(!_expect_token(read_token(), ",")) return false;
    y = std::stod(read_token());
    if(!_expect_token(read_token(), ",")) return false;
    v = std::stod(read_token());

    ASSERT(_nodes[cnt].coord == point(x, y));
    _nodes[cnt].vy = v;

    //if(cnt < 10) {
      //std::cout << to_string(_nodes[cnt].coord) << " "
  //<< _nodes[cnt].vy << '\n';
    //}
    cnt += 1;
  }
  ASSERT(cnt == _nodes.size());

  return true;
}

std::string 
Flow::read_comment() 
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
Flow::_is_comment_prefix()
{
  if(_read_cur < _length) {
    return ( _buffer[_read_cur] == '%');
  }
  return false;
}

}
