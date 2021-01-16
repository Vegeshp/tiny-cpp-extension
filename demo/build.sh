cc=g++
out=a.out

$cc ./../src/time_util.cc time.cc -I ./../include -O2 -std=c++14 -o $out -Wall -Werror
./$out
printf '\n'
$cc ostream.cc -I ./../include -O2 -std=c++14 -o $out -Wall -Werror
./$out
printf '\n'
$cc random.cc -I ./../include -O2 -std=c++14 -o $out -Wall -Werror
./$out
printf '\n'
$cc type.cc -I ./../include -O2 -std=c++14 -o $out -Wall -Werror
./$out
printf '\n'
$cc type.cc -I ./../include -O2 -std=c++14 -o $out -Wall -Werror -DFILT
./$out | c++filt -t

rm ./*.out