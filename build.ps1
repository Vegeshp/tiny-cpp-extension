g++ -c ftime.cc -static -fPIC -O2 -std=c++11 -o libftime.a
# g++ main.cc -L. -lftime -o a.exe
# .\a.exe