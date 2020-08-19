#pragma once

#include <string>
#include <vector>

extern const std::vector<std::string> formats;
extern const std::string default_time_format;

class TS {
 public:
  static int64_t get_ts(bool to_milli = true, int UTC = 8);
  static std::string ts2s(int64_t timestamp, const std::string& format_string,
                          bool is_milli = true);
  static std::string ts2s(int64_t timestamp, bool is_milli = true);
  static int64_t s2ts(const std::string& s, bool to_milli = true, int UTC = 8);
  static int64_t s2ts(const std::string& s, const std::string& format,
                      bool to_milli = true, int UTC = 8);
};
