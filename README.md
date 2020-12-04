# extended-cpp-util -- A tool for simplify C++ behavior implemented in modern C++

## I. Purpose For This Project

Intended for a deeper look into `'Modern' C++ Language Features`. The reason for quoting `modern` is that the features I use have already been introduced as the oldest modern standard about 10 years ago(I mean c++11), and I have little knowledge about C++ currently compared to those who have already master this. So I think code more can make me have a better understanding of C++, like template meta programming -- more specific SFINAE(Substitution Failure Is Not An Error), typical class design(less extended) etc.

## II. Current Function

Functions are divided into 2 parts now: time_util and ostream_util 

1. time_util

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

## III. Way To Use

### 0. What you need

Nothing but a c++ compiler which supports `c++14` or higher standard.

Actually, `c++11` is also OK, but you need to make some changes: I use `std::enable_if_t` for a 'tidy' code.

see demo folder for more detail.

### 4. other option

If the way to include this tool really annoyed you, you can simply dump all lines of code in a single file `xxxx.h`, then include it.

## IV. Miscellaneous

### 1. **THE LICENSE**

Please read thoroughly if you have no idea what **the license** is.

### 2. Robusticity

Behavior is not guaranteed when an UB happened.

### 3. Correctness

If some bugs are found, please open an issue to inform me.

### 4. Functions I used

These functions can be seen clearly on the top few lines in `time_util.cc`. 

### 5. Improve it!

I'd be flattered if you could share the thoughts for improvement with me.
