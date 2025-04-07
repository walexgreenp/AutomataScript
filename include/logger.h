#ifndef LOGGER_H
#define LOGGER_H

#include "../include/transpiler_types.h"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

/* Logger macro */
#define LOG(msg) logger.log(msg)

// -----------------------------------------------------------------------------
// Logger Class
// -----------------------------------------------------------------------------
// The Logger class is responsible for allowing support of logs, so devs
// can log things to a logfile for easier debugging
class Logger {
public:
  Logger(const std::string &filename);
  ~Logger();

  void log(const std::string &message);
  void log_tokens(std::vector<std::string> tokens);

private:
  std::ofstream logFile;
  std::string get_current_time();
};

/* Global Loger declaration */
extern Logger logger;

#endif /* LOGGER_H */
