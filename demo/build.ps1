rm *.a, *.exe -r -force
g++ -c ../src/ftime.cc -I../include/ -static -fPIC -O2 -std=c++11 -o libftime.a
g++ main.cc -L. -I../include/ -lftime -o a.exe
.\a.exe
rm *.a, *.exe -r -force