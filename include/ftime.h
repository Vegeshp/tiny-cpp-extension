#pragma once

#include <string>

class TS {
 public:
  static int64_t get_ts(bool to_milli = true, int UTC = 8);
  static std::string ts2s(int64_t timestamp, const std::string& format_string,
                          bool is_milli = true, bool to_milli = true,
                          int UTC = 8);
  static std::string ts2s(int64_t timestamp, bool is_milli = true,
                          bool to_milli = true, int UTC = 8);
  static bool is_ts(const std::string& s);
  static int64_t s2ts(std::string s, bool to_milli = true, int UTC = 8);
};
