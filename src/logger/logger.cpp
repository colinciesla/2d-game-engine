//
// Created by colin on 6/26/2024.
//

#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>

#include "logger.h"

std::vector<logEntry> logger::messages;

std::string currentDateTimeToString() {
  const auto now = std::chrono::system_clock::now();
  const auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%m-%d-%Y %X");

  return ss.str();
}

void logger::log(const std::string& message) {
  logEntry logEntry;
  logEntry.type = INFO;
  logEntry.message = "\033[1;32mLOG | " + currentDateTimeToString() + " - " + message +
                     " \033[0m";

  std::cout << logEntry.message << std::endl;

  messages.push_back(logEntry);
}

void logger::err(const std::string& message) {
  logEntry logEntry;
  logEntry.type = ERROR;
  logEntry.message = "\033[1;31mERR | " + currentDateTimeToString() + " - " + message +
                     " \033[0m";

  std::cerr << logEntry.message << std::endl;

  messages.push_back(logEntry);
}
