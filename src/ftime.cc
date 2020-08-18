#include "ftime.h"

#include <cassert>
#include <chrono>
#include <iterator>
#include <regex>

using std::map;
using std::regex;
using std::sregex_iterator;
using std::string;
using std::to_string;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::time_point;

const map<string, vector<string>> format_string_map = {
    {"(Wl)",
     {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
      "Saturday"}},
    {"(Ws)", {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"}}};

regex default_time_format_regex{default_time_format},
    number_format_regex{"([0-9]+)"};

int64_t TS::get_ts(bool to_milli, int UTC) {
  return (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
              .count() +
          UTC * 3600000LL) /
         (to_milli ? 1 : 1000);
}

string TS::ts2s(int64_t timestamp, bool is_milli) {
  return TS::ts2s(timestamp, default_time_format, is_milli);
}

int64_t TS::s2ts(const string &s, bool to_milli, int UTC) {
  return s2ts(s, default_time_format, to_milli, UTC);
}

string TS::ts2s(int64_t timestamp, const string &format_string, bool is_milli) {
  if (!is_milli) {
    timestamp *= 1000;
  }
  const time_t const_time_point = system_clock::to_time_t(
      time_point<system_clock, milliseconds>(milliseconds(timestamp)));
  tm time = *gmtime(&const_time_point);
  map<char, int> mp = {{'Y', time.tm_year + 1900}, {'M', time.tm_mon + 1},
                       {'D', time.tm_mday},        {'h', time.tm_hour},
                       {'s', time.tm_sec},         {'W', time.tm_wday}};
  string res = string(format_string);
  for (const string &s : formats) {
    int index = 0;
    if ((index = res.find(s)) != string::npos) {
      res.replace(index, s.size(), [&]() -> string {
        if (s[1] == 'W') {
          return (*format_string_map.find(s)).second.at(mp['W']);
        } else if (s == "(m2)") {
          string res = to_string(time.tm_min);
          if (res.size() == 1) {
            return ' ' + res;
          }
          return res;
        } else if (s == "(m3)") {
          string res = to_string(timestamp % 1000);
          if (res.size() < 3) {
            return string(3 - res.size(), '0') + res;
          }
          return res;
        }
        string res = to_string(mp[s[1]]);
        int length = s[2] - '0';
        if (length <= res.size()) {
          return res;
        }
        return string(length - res.size(), '0') + res;
      }());
    }
  }
  map<char, int>().swap(mp);
  return res;
}

int64_t TS::s2ts(const string &s, const string &format, bool to_milli,
                 int UTC) {
  sregex_iterator begin =
                      sregex_iterator(s.begin(), s.end(), number_format_regex),
                  end = sregex_iterator();
  tm t;
  int milli;
  vector<std::pair<int, int>> pair_vec;
  for (const string &type : formats) {
    if (type[1] == 'W') {
      break;
    }
    int index;
    if ((index = format.find(type)) != string::npos) {
      pair_vec.emplace_back(index, pair_vec.size());
    } else {
      assert(type != "m3");
    }
  }
  assert(pair_vec.size() >= 6);
  std::sort(pair_vec.begin(), pair_vec.end());
  vector<std::pair<int, int>>::iterator pair_vec_it = pair_vec.begin();
  while (begin != end) {
    (*pair_vec_it).first = stoi((*begin).str());
    std::swap((*pair_vec_it).first, (*pair_vec_it).second);
    ++pair_vec_it;
    ++begin;
  }
  std::sort(pair_vec.begin(), pair_vec.end());
  pair_vec_it = pair_vec.begin();
  vector<int *> vec{&(t.tm_year = -1900), &(t.tm_mon = -1), &(t.tm_mday = 0),
                    &(t.tm_hour = 0),     &(t.tm_min = 0),  &(t.tm_sec = 0),
                    &(milli = 0)};
  vector<int *>::iterator vec_begin = vec.begin();
  while (vec_begin != vec.end()) {
    *(*vec_begin) += (*pair_vec_it).second;
    ++pair_vec_it;
    ++vec_begin;
  }
  vector<int *>().swap(vec);
  vector<std::pair<int, int>>().swap(pair_vec);
  return (mktime(&t) * 1000LL + milli + UTC * 3600000LL) /
         (to_milli ? 1 : 1000);
}
