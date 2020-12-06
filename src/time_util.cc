#include "time_util.h"

#include <cassert>
#include <chrono>
#include <iterator>
#include <map>

using std::map;
using std::memset;
using std::pair;
using std::regex;
using std::sort;
using std::sregex_iterator;
using std::string;
using std::to_string;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::time_point;

extern const string default_time_format =
    "(Y4)/(M2)/(D2) (h2):(m2):(s2).(m3) (Wl)";

extern const vector<string> formats = {
    "(Y4)", "(M2)", "(D2)", "(h2)", "(m2)", "(s2)", "(m3)", "(Ws)", "(Wl)"};

extern const map<string, vector<string>> format_string_map = {
    {"(Wl)", {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}},
    {"(Ws)", {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"}}};

extern const regex number_format_regex{"([0-9]+)"};

int basic_to_integer(const string &s) {
    int ans = 0;
    for (const char &c : s) {
        assert(isdigit(c));
        ans = ans * 10 + c - '0';
    }
    return ans;
}

uint64_t TimeUtil::now(int UTC) {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + UTC * 3600000ULL;
}

string TimeUtil::ts2s(uint64_t timestamp, const string &format_string, bool is_milli) {
    if (!is_milli) {
        timestamp *= 1000;
    }
    const time_t const_time_point = system_clock::to_time_t(time_point<system_clock, milliseconds>(milliseconds(timestamp)));
    tm time_struct = *gmtime(&const_time_point);
    map<char, int> mp = {{'Y', time_struct.tm_year + 1900}, {'M', time_struct.tm_mon + 1}, {'D', time_struct.tm_mday}, {'h', time_struct.tm_hour}, {'s', time_struct.tm_sec}, {'W', time_struct.tm_wday}};
    string res = string(format_string);
    for (const string &s : formats) {
        size_t index = 0;
        if ((index = res.find(s)) != string::npos) {
            res.replace(index, s.size(), [&]() -> string {
                if (s[1] == 'W') {
                    return (*format_string_map.find(s)).second.at(mp['W']);
                } else if (s == "(m2)") {
                    string res = to_string(time_struct.tm_min);
                    if (res.size() == 1) {
                        return '0' + res;
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
                unsigned length = s[2] - '0';
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

uint64_t TimeUtil::s2ts(const string &s, const string &format, bool to_milli, int UTC) {
    sregex_iterator begin = sregex_iterator(s.begin(), s.end(), number_format_regex), end = sregex_iterator();
    int milli, count = 0;
    vector<pair<int, int>> pair_vec;
    for (const string &type : formats) {
        if (type[1] == 'W') {
            break;
        }
        size_t index;
        if ((index = format.find(type)) != string::npos) {
            pair_vec.emplace_back(index, ++count);
        }
    }
    assert(pair_vec.size() >= 6);
    sort(pair_vec.begin(), pair_vec.end());
    vector<pair<int, int>>::iterator pair_vec_it = pair_vec.begin();
    while (begin != end && pair_vec_it != pair_vec.end()) {
        (*pair_vec_it).first = (*pair_vec_it).second;
        (*pair_vec_it).second = basic_to_integer((*begin).str());
        ++pair_vec_it;
        ++begin;
    }
    sort(pair_vec.begin(), pair_vec.end());
    pair_vec_it = pair_vec.begin();
    tm t;
    memset(&t, 0, sizeof(t));
    vector<int *> vec{&(t.tm_year = -1900), &(t.tm_mon = -1), &(t.tm_mday = 0), &(t.tm_hour = 0), &(t.tm_min = 0), &(t.tm_sec = 0), &(milli = 0)};
    vector<int *>::iterator vec_begin = vec.begin();
    while (vec_begin != vec.end() && pair_vec_it != pair_vec.end()) {
        *(*vec_begin) += (*pair_vec_it).second;
        ++pair_vec_it;
        ++vec_begin;
    }
    vector<int *>().swap(vec);
    vector<pair<int, int>>().swap(pair_vec);
    return (mktime(&t) * 1000ULL + milli + UTC * 3600000ULL) / (to_milli ? 1 : 1000);
}
