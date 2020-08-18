#include <iostream>

#include "ftime.h"

using namespace std;

int main() {
  int64_t ts = TS::get_ts();
  cout << ts << endl;
  string s = TS::ts2s(ts);
  cout << s << endl;
  int64_t res = TS::s2ts(s);
  cout << res << endl;
  cout << TS::ts2s(res) << endl;
  return 0;
}