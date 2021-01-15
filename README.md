# extended-cpp-util

[toc]

a tool for simplifying C++ behavior implemented in modern C++

## I. Purpose For This Project

Intended for a deeper look into `'Modern' C++ Language Features`. The reason for quoting `modern` is that the features I use have already been introduced as the oldest modern standard about 10 years ago(I mean c++11), and I have little knowledge about C++ currently compared to those who have already master this. So I think code more can make me have a better understanding of C++, like template meta programming -- more specific SFINAE(Substitution Failure Is Not An Error), typical class design(less extended) etc.

## II. Current Function

Functions are divided into 4 parts now: time_util, ostream_util, random_util and type_util

### 1. time_util

The class definition is as follows:

```c++
class TimeUtil {
   public:
    static uint64_t now(int UTC = 8);
    static std::string ts2s(uint64_t timestamp, const std::string& format_string = default_time_format, bool is_milli = true);
    static uint64_t s2ts(const std::string& s, const std::string& format_string = default_time_format, bool to_milli = true, int UTC = 8);
};
```
now: get the current time in a certain timezone(default UTC+8)

ts2s: get a formatted time string(timestamp to string)

s2ts: convert time string to a timestamp in a certain timezone (string to timestamp)

to_milli: if the timestamp is in milliseconds

```c++
extern const string default_time_format =
    "(Y4)/(M2)/(D2) (h2):(m2):(s2).(m3) (Wl)";

extern const vector<string> formats = {
    "(Y4)", "(M2)", "(D2)", "(h2)", "(m2)", "(s2)", "(m3)", "(Ws)", "(Wl)"};

extern const map<string, vector<string>> format_string_map = {
    {"(Wl)", {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}},
    {"(Ws)", {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"}}};

extern const regex number_format_regex{"([0-9]+)"};
```

external spec meaning is as above:

Y4: year in 4 digits

M2: month in 2 digits

D2: day in 2 digits

h2: hour in 2 digits

m2: minute in 2 digits

s2: second in 2 digits

m3: milliseconds in 3 digits

Ws: what day in a week in short

Wl: which day in a week ~~in long~~ in a complete expression

Implementation is naive substitution, and the use of map and some round of traverse may cause a loss in mass acquisition. So performance can not be made sure of. 

### 2. ostream_util

This part is the most fancy one. 

It can output pair, tuple, const-iterable stl or stl-like container with 5 major functions.

However, the support class is comparative heavier. It implemented 3(more precise 6) test functions to test if a function or operator exists in a class, using SFINAE.  

### 3. random_util

This part is harder than I thought. 

It is a method for generate discrete distribution according to the passed-in parameters. Usage is almost the same as `std::discrete_distribution<Tp>`. But the new method I wrote is not template-like, because it seems useless. I think now that we sample index, why not just use `size_t`? (

Implementing this needs to know **alias method**, which is a good performance method. Arrangement referred to g++ implementation.

// TODO: time consumption contrast with original implementation

### 4. type_util

Use libc++abi to output the `RealType` of a type. Better than `typeid().name()` via `c++filt -t`, it can show l/rvalue reference and both bottom-level top-level const-volatile qualifier. Meanwhile, instead of showing scary order of type decorator, it uses the widest order, like `const type * const`, rather than `type const * const`. For more detail, see `demo/type.cc`.

Prerequisitive:
have either `c++filt` from `binutils` or `libc++abi`.

Guide:
If you have `c++filt` correctly installed on your system, simply compile adding `-DFILT` flag, then use `./a.out | c++filt -t`. Or more easily, if you have `libc++abi`, just compile and run normally.

## III. Way To Use

### What you need

Nothing but a c++ compiler which supports `c++14` or higher standard.

Actually, `c++11` is also OK, but you need to make some changes: I use `std::enable_if_t` for a 'tidy' code.

see demo folder for more detail.

### other option

If the way to include this tool really annoyed you, you can simply dump all lines of code in a single file `xxxx.h`, then include it.

## IV. Miscellaneous

### 1. **THE LICENSE**

Please read thoroughly if you have no idea what **the license** is.

### 2. Robusticity

Behavior is not guaranteed when a UB happened.

### 3. Correctness

If some bugs are found, please open an issue to inform me.

### 4. Functions I used

These functions can be seen clearly on the top few lines in `time_util.cc`. 

### 5. Improve it!

I'd be flattered if you could share the thoughts for improvement with me.
