cc=clang++
out=a.out

# $cc -c ./../src/time_util.cc -I ./../include/ -O2 -std=c++14 -o libtime_util.a
# $cc main.cc -L ./ -I ./../include/ -ltime_util -o a.out
# rm *.a *.out

$cc ./../src/time_util.cc main.cc -I ./../include -O2 -std=c++14 -o $out
./$out
$cc ostream.cc -I ./../include -O2 -std=c++14 -o $out
./$out
rm *.out