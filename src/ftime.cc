#include "ftime.h"

#include <cassert>
#include <chrono>
#include <iomanip>
#include <iterator>
#include <regex>
#include <sstream>
#include <vector>

using std::gmtime;
using std::mktime;
using std::regex;
using std::regex_search;
using std::setfill;
using std::setw;
using std::sregex_iterator;
using std::stoi;
using std::string;
using std::stringstream;
using std::tm;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::time_point;

const string time_format =
    "(\\d{1,4})\\/(\\d{1,2})\\/(\\d{1,2}) "
    "(\\d{1,2}):(\\d{1,2}):(\\d{1,2})(\\.\\d{1,3})?";
// in the format of YYYY/MM/DD HH:MM:SS.MMM
const string number_format = "(\\d+)";
regex time_format_regex{time_format}, number_format_regex{number_format};

int64_t TS::get_ts(bool is_milli, int UTC) {
  return (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
              .count() +
          UTC * 3600000LL) /
         (is_milli ? 1 : 1000);
}

string TS::ts2s(int64_t timestamp, bool is_milli, bool to_milli, int UTC) {
  if (!is_milli) {
    timestamp *= 1000;
  }
  const time_t const_time_point = system_clock::to_time_t(
      time_point<system_clock, milliseconds>(milliseconds(timestamp)));
  tm time = *gmtime(&const_time_point);
  stringstream ss;
  ss << time.tm_year + 1900 << "/" << time.tm_mon + 1 << "/" << time.tm_mday
     << " " << time.tm_hour << ":" << setw(2) << setfill('0') << time.tm_min
     << ":" << setw(2) << setfill('0') << time.tm_sec;
  if (to_milli) {
    ss << "." << setw(3) << setfill('0') << timestamp % 1000;
  }
  return ss.str();
}

bool TS::is_ts(const string &s) { return regex_search(s, time_format_regex); }

int64_t TS::s2ts(string s, bool is_milli, int UTC) {
  assert(is_ts(s));
  sregex_iterator begin =
                      sregex_iterator(s.begin(), s.end(), number_format_regex),
                  end = sregex_iterator();
  tm t;
  int milli;
  vector<int *> vec{&(t.tm_year = -1900), &(t.tm_mon = -1), &(t.tm_mday = 0),
                    &(t.tm_hour = 0),     &(t.tm_min = 0),  &(t.tm_sec = 0),
                    &(milli = 0)};
  vector<int *>::iterator vec_begin = vec.begin();
  while (begin != end) {
    *(*vec_begin) += stoi((*begin).str());
    ++begin;
    ++vec_begin;
  }
  vector<int *>().swap(vec);
  int64_t res = mktime(&t) * 1000LL + milli + UTC * 3600000LL;
  return res / (is_milli ? 1 : 1000);
}
