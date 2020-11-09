rm *.a, *.exe -r -force
g++ -c ../src/time_util.cc -I../include/ -static -fPIC -O2 -std=c++14 -o libtime_util.a
g++ main.cc -L. -I../include/ -ltime_util -o a.exe
.\a.exe
rm *.a, *.exe -r -force