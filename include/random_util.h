#pragma once

#include <algorithm>
#include <queue>
#include <random>
#include <vector>

// due to the non-definite implementation of standard template library, many may implement std::discrete_distribution
// as Cumulative Probability Function with Binary Search (also called roulette wheel selection), whose time complexity
// is O(logn) for every query. However, there is a kind of method, which only costs O(1) for every query, named Alias
// Method. Both methods need at least O(n) space complexity to save distribution.

struct DiscreteDistribution {
   public:
    using result_type = size_t;

    struct param_type {
        friend struct DiscreteDistribution;

        param_type() : _M_prob() {
            _M_initialize();
        }

        template <typename _InputIterator>
        param_type(_InputIterator __wbegin, _InputIterator __wend) : _M_prob(__wbegin, __wend) {
            _M_initialize();
        }

        param_type(std::initializer_list<double> __wil) : _M_prob(__wil) {
            _M_initialize();
        }

        template <typename _Func>
        param_type(size_t __nw, double __xmin, double __xmax, _Func __fw) {
            const size_t __n = __nw == 0 ? 1 : __nw;
            const double __delta = (__xmax - __xmin) / __n;
            _M_prob.reserve(__n);
            for (size_t __k = 0; __k < __nw; ++__k) {
                _M_prob.emplace_back(__fw(__xmin + (__k + 0.5) * __delta));
            }
            _M_initialize();
        }

        param_type(const param_type&) = default;

        param_type& operator=(const param_type&) = default;

        template <typename _UniformRandomNumberGenerator>
        size_t result(size_t __index, _UniformRandomNumberGenerator& __urng) {
            return _M_dis[__index](__urng) ? __index : _M_alias[__index];
        }

        std::vector<double> probabilities() const {
            return _M_prob;
        }

        size_t size() const { return _M_prob.size(); }

        friend bool operator==(const param_type& __p1, const param_type& __p2) {
            return __p1._M_prob == __p2._M_prob;
        }

        friend bool operator!=(const param_type& __p1, const param_type& __p2) {
            return !(__p1 == __p2);
        }

        friend std::ostream& operator<<(std::ostream& os, const param_type& __p) {
            return os << __p._M_prob;
        }

       private:
        void _M_initialize() {
            if (_M_prob.size() < 2) {
                _M_prob.resize(1, 1.0);
            }

            const size_t __size = _M_prob.size();
            _M_dice = std::uniform_int_distribution<size_t>{0, __size - 1};

            const double __sum = std::accumulate(_M_prob.begin(), _M_prob.end(), double(0.0));
            std::__detail::__normalize(_M_prob.begin(), _M_prob.end(), _M_prob.begin(), __sum);

            std::vector<double> _scaled;
            _scaled.reserve(__size);
            std::transform(_M_prob.begin(), _M_prob.end(), std::back_inserter(_scaled),
                           [&](double x) -> double {
                               return x * __size;
                           });

            std::queue<size_t> _small, _large;
            for (size_t i = 0; i < __size; i++) {
                if (_scaled[i] < 1) {
                    _small.push(i);
                } else {
                    _large.push(i);
                }
            }

            std::vector<double> _M_fake_prob;
            _M_fake_prob.resize(__size, 0);
            _M_alias.resize(__size, 0);

            while (!_small.empty() && !_large.empty()) {
                size_t l = _small.front();
                _small.pop();
                size_t g = _large.front();
                _large.pop();
                _M_fake_prob[l] = _scaled[l];
                _M_alias[l] = g;
                _scaled[g] -= (1 - _scaled[l]);
                if (_scaled[g] < 1) {
                    _small.push(g);
                } else {
                    _large.push(g);
                }
            }
            while (!_large.empty()) {
                size_t g = _large.front();
                _large.pop();
                _M_fake_prob[g] = 1;
            }
            while (!_small.empty()) {
                size_t l = _small.front();
                _small.pop();
                _M_fake_prob[l] = 1;
            }

            _M_dis.reserve(__size);
            std::transform(_M_fake_prob.begin(), _M_fake_prob.end(), std::back_inserter(_M_dis),
                           [&](double x) -> std::bernoulli_distribution {
                               return std::bernoulli_distribution{x};
                           });

            std::vector<double>().swap(_scaled);
            std::vector<double>().swap(_M_fake_prob);
            std::queue<size_t>().swap(_small);
            std::queue<size_t>().swap(_large);
        }

        std::vector<double> _M_prob;
        std::vector<size_t> _M_alias;
        std::vector<std::bernoulli_distribution> _M_dis;
        std::uniform_int_distribution<size_t> _M_dice;
    };

    DiscreteDistribution() : _M_param() {}

    template <typename _InputIterator>
    DiscreteDistribution(_InputIterator __begin, _InputIterator __end) : _M_param(__begin, __end) {}

    DiscreteDistribution(std::initializer_list<double> __wl) : _M_param(__wl) {}

    template <typename _Func>
    DiscreteDistribution(size_t __nw, double __xmin, double __xmax, _Func __fw)
        : _M_param(__nw, __xmin, __xmax, __fw) {}

    explicit DiscreteDistribution(const param_type& __p) : _M_param(__p) {}

    std::vector<double> probabilities() const {
        return _M_param._M_prob;
    }

    param_type param() const {
        return _M_param;
    }

    void param(const param_type& __param) {
        _M_param = __param;
    }

    void reset() {}

    result_type min() const {
        return result_type(0);
    }

    result_type max() const {
        return result_type(_M_param.size() - 1);
    }

    template <typename _UniformRandomNumberGenerator>
    result_type operator()(_UniformRandomNumberGenerator& __urng) {
        return this->operator()(__urng, _M_param);
    }

    template <typename _UniformRandomNumberGenerator>
    result_type operator()(_UniformRandomNumberGenerator& __urng, const param_type& __param) {
        return _M_param.result(_M_param._M_dice(__urng), __urng);
    }

    template <typename _ForwardIterator, typename _UniformRandomNumberGenerator>
    void __generate(_ForwardIterator __f, _ForwardIterator __t, _UniformRandomNumberGenerator& __urng,
                    const param_type& __param) {
        while (__f != __t) {
            *__f++ = _M_param.result(_M_param._M_dice(__urng), __urng);
        }
    }

    template <typename _ForwardIterator, typename _UniformRandomNumberGenerator>
    void __generate(_ForwardIterator __f, _ForwardIterator __t, _UniformRandomNumberGenerator& __urng) {
        this->__generate(__f, __t, __urng, _M_param);
    }

    friend std::ostream& operator<<(std::ostream& __os, const DiscreteDistribution& __x) {
        return __os << __x._M_param;
    }

    friend bool operator==(const DiscreteDistribution& __d1, const DiscreteDistribution& __d2) {
        return __d1._M_param == __d2._M_param;
    }

    friend std::istream& operator>>(std::istream& __is, DiscreteDistribution& __x) {
        size_t __n;
        if (__is >> __n) {
            std::vector<double> __prob_vec;
            if (std::__detail::__extract_params(__is, __prob_vec, __n)) {
                __x.param({__prob_vec.begin(), __prob_vec.end()});
            }
        }
        return __is;
    }

   private:
    param_type _M_param;
};

inline bool operator!=(const DiscreteDistribution& __d1, const DiscreteDistribution& __d2) {
    return !(__d1 == __d2);
}
