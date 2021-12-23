cc=g++
out=a.out
version=c++20
$cc ostream.cc -I ./../include -O2 -std=$version -o $out -Wall -Werror
./$out
printf '\n'
$cc random.cc -I ./../include -O2 -std=$version -o $out -Wall -Werror
./$out
printf '\n'
$cc new_idea.cc -I ./../include -O2 -std=$version -o $out -Wall -Werror
./$out
printf '\n'
$cc type.cc -I ./../include -O2 -std=$version -o $out -Wall -Werror
./$out

rm ./*.out
