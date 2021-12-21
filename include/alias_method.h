#pragma once

#ifndef ALIAS_METHOD_H_
#define ALIAS_METHOD_H_

#include <algorithm>
#include <queue>
#include <random>
#include <vector>

#include "ostream_ops.h"

// due to the non-definite implementation of standard template library, many may implement std::discrete_distribution
// as Cumulative Probability Function with Binary Search (also called roulette wheel selection), whose time complexity
// is O(logn) for every query. However, there is a kind of method, which only costs O(1) for every query, named Alias
// Method. Both methods need at least O(n) space complexity to save distribution.

struct AliasMethod {
   public:
    using result_type = size_t;

    struct param_type {
        friend struct AliasMethod;

        param_type() : m_prob() {
            m_initialize();
        }

        template <typename InputIterator>
        param_type(InputIterator wbegin, InputIterator wend) : m_prob(wbegin, wend) {
            m_initialize();
        }

        param_type(std::initializer_list<double> wil) : m_prob(wil) {
            m_initialize();
        }

        template <typename Func>
        param_type(size_t nw, double xmin, double xmax, Func fw) {
            const size_t n = nw == 0 ? 1 : nw;
            const double delta = (xmax - xmin) / n;
            m_prob.reserve(n);
            for (size_t k = 0; k < nw; ++k) {
                m_prob.emplace_back(fw(xmin + (k + 0.5) * delta));
            }
            m_initialize();
        }

        param_type(const param_type&) = default;

        param_type& operator=(const param_type&) = default;

        template <typename UniformRandomNumberGenerator>
        size_t result(size_t index, UniformRandomNumberGenerator& urng) {
            return m_dis[index](urng) ? index : m_alias[index];
        }

        std::vector<double> probabilities() const {
            return m_prob;
        }

        size_t size() const { return m_prob.size(); }

        friend bool operator==(const param_type& p1, const param_type& p2) {
            return p1.m_prob == p2.m_prob;
        }

        friend bool operator!=(const param_type& p1, const param_type& p2) {
            return !(p1 == p2);
        }

        friend std::ostream& operator<<(std::ostream& os, const param_type& p) {
            return os << p.m_prob;
        }

       private:
        void m_initialize() {
            if (m_prob.size() < 2) {
                m_prob.resize(1, 1.0);
            }

            const size_t size = m_prob.size();
            m_dice = std::uniform_int_distribution<size_t>{0, size - 1};

            const double sum = std::accumulate(m_prob.begin(), m_prob.end(), double(0.0));
            std::transform(m_prob.begin(), m_prob.end(), m_prob.begin(), [&sum](double x) { return x / sum; });

            std::vector<double> scaled(size);
            std::transform(m_prob.begin(), m_prob.end(), scaled.begin(), [&size](double x) { return x * size; });

            std::queue<size_t> small, large;
            for (size_t i = 0; i < size; i++) {
                if (scaled[i] < 1) {
                    small.push(i);
                } else {
                    large.push(i);
                }
            }

            std::vector<double> fake_prob;
            fake_prob.resize(size, 0);
            m_alias.resize(size, 0);

            while (!small.empty() && !large.empty()) {
                size_t l = small.front();
                small.pop();
                size_t g = large.front();
                large.pop();
                fake_prob[l] = scaled[l];
                m_alias[l] = g;
                scaled[g] -= (1 - scaled[l]);
                if (scaled[g] < 1) {
                    small.push(g);
                } else {
                    large.push(g);
                }
            }
            while (!large.empty()) {
                size_t g = large.front();
                large.pop();
                fake_prob[g] = 1;
            }
            while (!small.empty()) {
                size_t l = small.front();
                small.pop();
                fake_prob[l] = 1;
            }

            m_dis.reserve(size);
            std::transform(fake_prob.begin(), fake_prob.end(), std::back_inserter(m_dis),
                           [&](double x) -> std::bernoulli_distribution {
                               return std::bernoulli_distribution{x};
                           });

            std::vector<double>().swap(scaled);
            std::vector<double>().swap(fake_prob);
            std::queue<size_t>().swap(small);
            std::queue<size_t>().swap(large);
        }

        std::vector<double> m_prob;
        std::vector<size_t> m_alias;
        std::vector<std::bernoulli_distribution> m_dis;
        std::uniform_int_distribution<size_t> m_dice;
    };

    AliasMethod() : m_param() {}

    template <typename InputIterator>
    AliasMethod(InputIterator begin, InputIterator end) : m_param(begin, end) {}

    AliasMethod(std::initializer_list<double> wl) : m_param(wl) {}

    template <typename Func>
    AliasMethod(size_t nw, double xmin, double xmax, Func fw)
        : m_param(nw, xmin, xmax, fw) {}

    explicit AliasMethod(const param_type& p) : m_param(p) {}

    std::vector<double> probabilities() const {
        return m_param.m_prob;
    }

    param_type param() const {
        return m_param;
    }

    void param(const param_type& param) {
        m_param = param;
    }

    void reset() {}

    result_type min() const {
        return result_type(0);
    }

    result_type max() const {
        return result_type(m_param.size() - 1);
    }

    template <typename UniformRandomNumberGenerator>
    result_type operator()(UniformRandomNumberGenerator& urng) {
        return this->operator()(urng, m_param);
    }

    template <typename UniformRandomNumberGenerator>
    result_type operator()(UniformRandomNumberGenerator& urng, const param_type& param) {
        return m_param.result(m_param.m_dice(urng), urng);
    }

    template <typename ForwardIterator, typename UniformRandomNumberGenerator>
    void generate(ForwardIterator f, ForwardIterator t, UniformRandomNumberGenerator& urng,
                  const param_type& param) {
        while (f != t) {
            *f++ = m_param.result(m_param.m_dice(urng), urng);
        }
    }

    template <typename ForwardIterator, typename UniformRandomNumberGenerator>
    void generate(ForwardIterator f, ForwardIterator t, UniformRandomNumberGenerator& urng) {
        this->generate(f, t, urng, m_param);
    }

    friend std::ostream& operator<<(std::ostream& os, const AliasMethod& x) {
        return os << x.m_param;
    }

    friend bool operator==(const AliasMethod& d1, const AliasMethod& d2) {
        return d1.m_param == d2.m_param;
    }

    friend std::istream& operator>>(std::istream& is, AliasMethod& x) {
        size_t n;
        if (is >> n) {
            std::vector<double> prob_vec;
            prob_vec.reserve(n);
            while (n--) {
                double val;
                if (is >> val) {
                    prob_vec.emplace_back(val);
                } else {
                    break;
                }
            }
            if (is) {
                x.param({prob_vec.begin(), prob_vec.end()});
            }
        }
        return is;
    }

   private:
    param_type m_param;
};

inline bool operator!=(const AliasMethod& d1, const AliasMethod& d2) {
    return !(d1 == d2);
}

#endif  // ALIAS_METHOD_H_
