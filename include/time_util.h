#pragma once

#include <regex>
#include <string>
#include <vector>

extern const std::vector<std::string> formats;
extern const std::string default_time_format;
extern const std::map<std::string, std::vector<std::string>> format_string_map;
extern const std::regex number_format_regex;

class TimeUtil {
   public:
    static uint64_t now(int UTC = 8);
    static std::string ts2s(uint64_t timestamp, const std::string& format_string = default_time_format, bool is_milli = true);
    static uint64_t s2ts(const std::string& s, const std::string& format_string = default_time_format, bool to_milli = true, int UTC = 8);
};
