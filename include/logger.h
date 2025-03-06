#ifndef LOGGER_H
#define LOGGER_H

#include "../include/transpiler_types.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

/* Logger macro */
#define LOG(msg) logger.log(msg)


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
