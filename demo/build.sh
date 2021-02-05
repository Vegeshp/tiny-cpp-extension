cc=clang++
out=a.out
version=c++14

$cc ./../src/time_util.cc time.cc -I ./../include -O2 -std=$version -o $out -Wall -Werror
./$out
printf '\n'
$cc ostream.cc -I ./../include -O2 -std=$version -o $out -Wall -Werror
./$out
printf '\n'
$cc random.cc -I ./../include -O2 -std=$version -o $out -Wall -Werror
./$out
printf '\n'
$cc type.cc -I ./../include -O2 -std=$version -o $out -Wall -Werror -DFILT
./$out | c++filt -t
printf '\n'
$cc new_idea.cc -I ./../include -O2 -std=$version -o $out -Wall -Werror
./$out

rm ./*.out