//
// Created by colin on 6/26/2024.
//

#pragma once

#include <string>
#include <vector>

enum logType
{
  INFO,
  WARNING,
  ERROR
};

struct logEntry
{
  logType type;
  std::string message;
};

class logger
{
public:
  static std::vector<logEntry> messages;

  static void log(const std::string &message);

  static void err(const std::string &message);
};
