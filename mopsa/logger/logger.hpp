#ifndef MOPSA_LOGGER_LOGGER_H
#define MOPSA_LOGGER_LOGGER_H

#include <mopsa/logger/debug.hpp>
#include <mopsa/logger/def.hpp>
#include <fstream>
#include <vector>
#include <map>

namespace mopsa 
{

typedef enum 
{
  INFO,
  WARNING,
  ERROR,
  DEBUG,
} LoggerSensivity;

class Logger 
{

public:
  Logger();
  ~Logger();

  static Logger& get_instance();

  // Log to file
  Logger& log(LoggerSensivity sensivity);

  template<class T>
  Logger& operator << (const T& message);

  // profiling 
  static void start();
  static void stop();

  static void add_record(const std::string& group, const std::string& name, int val);
  static void set_record(const std::string& group, const std::string& name, int val);

private:

  const std::vector<std::string> _sensivity_names = {"Info", "Warning", "Error", "Debug"};
  std::ofstream _file_log_stream;
  bool _is_first_time{1};
  std::map<std::string, std::map<std::string, int>> _records;
};

template<class T>
Logger& 
Logger::operator << (const T& message) 
{
  _file_log_stream << message;

  std::cout << message;
  std::cout.flush();

  return *this;
}

}

#endif
