#include "../include/logger.h"

// Logger initialization
Logger logger("logs/logfile.log");

/*
 * Log tokens with their index/identifier
 */
Logger::Logger(const std::string &filename) {
  logFile.open(filename);

    if (std::getenv("GITHUB_ACTIONS") == nullptr) {
        std::cerr << "Error opening log file!\n";
    }
}

Logger::~Logger() {
  if (logFile.is_open()) {
    logFile.close();
  }
}

void Logger::log(const std::string &message) {
  std::string timestamp = get_current_time();
  if (logFile) {
    logFile << "[" << timestamp << "] " << message
            << std::endl; // Write to file with timestamp
  }
}

void Logger::log_tokens(std::vector<std::string> tokens){
  for(int i = 0; i < tokens.size(); i++){
    std::string output = "" + std::to_string(i) + ": " + tokens[i];
    LOG(output);
  }
}

std::string Logger::get_current_time() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm local_tm = *std::localtime(&now_time);

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}






