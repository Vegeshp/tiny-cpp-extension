cc=g++
out=a.out

$cc ./../src/time_util.cc time.cc -I ./../include -O2 -std=c++14 -o $out
./$out

$cc ostream.cc -I ./../include -O2 -std=c++14 -o $out
./$out

$cc random.cc -I ./../include -O2 -std=c++14 -o $out
./$out

rm *.out