#include <iostream>
#include <string>

#include "time_util.h"

using namespace std;

int main() {
    uint64_t ts = TimeUtil::now();
    cout << ts << endl; // number
    string s = TimeUtil::ts2s(ts);
    cout << s << endl; // number -> string
    ts = TimeUtil::s2ts(s);
    cout << ts << endl; // string -> number
    s = TimeUtil::ts2s(ts);
    cout << s << endl; // test number -> string again
    ts = TimeUtil::s2ts(s);
    cout << ts << endl; // test string -> number again

    cout << "======== New Format Again ========" << endl;

    string format = "Now the time is (M2)/(D2)/(Y4) (h2):(m2):(s2):(m3).";
    s = TimeUtil::ts2s(ts, true, format); // number -> string
    cout << s << endl;
    ts = TimeUtil::s2ts(s, true, 8, format); // string -> number
    cout << ts << endl;
    s = TimeUtil::ts2s(ts, true, format); // test number -> string again
    cout << s << endl;
    ts = TimeUtil::s2ts(s, true, 8, format); // test string -> number again
    cout << ts << endl;
    return 0;
}