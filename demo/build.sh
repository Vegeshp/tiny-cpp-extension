cc=clang++
$cc -c ./../src/time_util.cc -I ./../include/ -O2 -std=c++14 -o libtime_util.a
$cc main.cc -L ./ -I ./../include/ -ltime_util -o a.out
./a.out
rm *.a *.out