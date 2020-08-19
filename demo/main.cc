#include <iostream>
#include <string>

#include "ftime.h"

using namespace std;

int main() {
  string format = "Now the time is (M2)/(D2)/(Y4) (h2):(m2):(s2):(m3).";
  int64_t ts = TS::get_ts();
  cout << ts << endl;
  string s = TS::ts2s(ts, format);
  cout << s << endl;
  ts = TS::s2ts(s, format);
  cout << ts << endl;
  cout << TS::ts2s(ts, format) << endl;
  
  s = TS::ts2s(ts);
  cout << s << endl;
  ts = TS::s2ts(s);
  cout << ts << endl;
  s = TS::ts2s(ts);
  cout << s << endl;
  ts = TS::s2ts(s);
  cout << ts << endl;

  s = TS::ts2s(ts, format);
  cout << s << endl;
  ts = TS::s2ts(s, format);
  cout << ts << endl;
  cout << TS::ts2s(ts, format) << endl;
  return 0;
}