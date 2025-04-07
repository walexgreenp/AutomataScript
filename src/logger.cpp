#include "../include/logger.h"

// Global logger initialization, initialized with output file path
Logger logger("logs/logfile.log");

/**
 * Logger::Logger
 * @brief Constructs a Logger instance and opens the specified log file.
 *
 * @param filename Path to the log file to be written.
 *
 * If the program is running in a GitHub Actions environment (CI),
 * the log file opening error is silently ignored.
 */
Logger::Logger(const std::string &filename) {
  logFile.open(filename);

  if (std::getenv("GITHUB_ACTIONS") != nullptr) {
    // std::cerr << "Error opening log file!\n";
  }
}

/**
 * Logger::~Logger
 * @brief Destructor for Logger.
 *
 * Closes the log file if it's open.
 */
Logger::~Logger() {
  if (logFile.is_open()) {
    logFile.close();
  }
}

/**
 * Logger::log
 * @brief Logs a single message to the log file with a timestamp.
 *
 * @param message The message string to be written to the log file.
 *
 * There is a macro defined that uses this, see LOG
 */
void Logger::log(const std::string &message) {
  std::string timestamp = get_current_time();
  if (logFile) {
    logFile << "[" << timestamp << "] " << message
            << std::endl; // Write to file with timestamp
  }
}

/**
 * Logger::log_tokens
 * @brief Logs a list of tokens with their corresponding indices.
 *
 * @param tokens A vector of string tokens to be logged.
 * Each token is printed on a new line in the format: "<index>: <token>".
 */
void Logger::log_tokens(std::vector<std::string> tokens) {
  for (int i = 0; i < tokens.size(); i++) {
    std::string output = "" + std::to_string(i) + ": " + tokens[i];
    LOG(output);
  }
}

/**
 * Logger::get_current_time
 * @brief Returns the current time as a formatted string.
 *
 * @return A string representing the current local time in the format
 * "YYYY-MM-DD HH:MM:SS".
 *
 * Used in Logger::log for consistent log data
 */
std::string Logger::get_current_time() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm local_tm = *std::localtime(&now_time);

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}
