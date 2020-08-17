# TS -- A tool for timestamp implemented in C++

## I. Way To Use

### 0. What you need

Nothing but a c++ compiler which supports `c++11` or higher standard.

### 1. Get a `.a` static/dynamic linking library

To give an example:

```shell
g++ -c ftime.cc -static -fPIC -O2 -std=c++11 -o libftime.a
```

### 2. Link in the way you want

Eg.

```shell
g++ main.cc -L. -lftime -o a.exe
```

### 3. Run the executable file

```shell
.\a.exe
```

### 4. other option

If the way to include this tool really annoyed you, you can simply dump all lines of code in a file `ftime.h`, then include it.

If this happened, please inform me.

(More likely, nobody will even notice or use this project, since it's so easy to implement directly.)

## II. Miscellaneous

### 1. **THE LICENSE**

Please read thoroughly if you have no idea what **the license** is.

### 2. Robusticity

This tool only supports formal environment, which means the correctness can be guaranteed if the input is within a normal range. So, when you pass parameters into these lovely functions, the post-conditions need to be fulfilled. Otherwise, the program breaks down easily.(Yes, all my fault)

### 3. Correctness

If you have found some bugs lie in the logic I wrote, please open an issue to inform me.

### 4. Functions I used

These functions can be seen clearly on the top few lines in `ftime.cc`. If you have a better way to implement, I'd be flattered if you could share the thoughts with me.
