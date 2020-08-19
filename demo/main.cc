#include <iostream>
#include <string>

#include "ftime.h"

using namespace std;

int main() {
  string format = "Now the time is (M2)/(D2)/(Y4) (h2):(m2):(s2):(m3).";
  int64_t ts = TS::get_ts();
  cout << ts << endl;
  string s = TS::ts2s(ts, format, true);
  cout << s << endl;
  int64_t res = TS::s2ts(s, format);
  cout << res << endl;
  cout << TS::ts2s(res, format) << endl;
  return 0;
}