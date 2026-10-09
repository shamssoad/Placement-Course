// C++17 reference lab. Read WORKED-LABS.md before viewing solutions.
// This file includes deterministic checks, but has not been run during authoring.
#include <algorithm>
#include <cstddef>
#include <deque>
#include <iostream>
#include <list>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::vector<int> windowMaximum(const std::vector<int>& values, int k) {
    if (k <= 0 || static_cast<std::size_t>(k) > values.size())
        throw std::invalid_argument("Invalid window size");
    if (values.size() > 100000)
        throw std::invalid_argument("Lab supports at most 100000 values");
    std::deque<int> candidates;
    std::vector<int> result;
    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        while (!candidates.empty() && candidates.front() <= i - k)
            candidates.pop_front();
        while (!candidates.empty() && values[candidates.back()] <= values[i])
            candidates.pop_back();
        candidates.push_back(i);
        if (i >= k - 1) result.push_back(values[candidates.front()]);
    }
    return result;
}

// Lab contract: n <= 100000; values and update deltas must have bounds
// guaranteeing every aggregate and lazy value fits in long long.
class RangeSum {
    int n_;
    std::vector<long long> sums_, pending_;

    void build(int node, int left, int right, const std::vector<int>& values) {
        if (right - left == 1) {
            sums_[node] = values[left];
            return;
        }
        int middle = left + (right - left) / 2;
        build(node * 2, left, middle, values);
        build(node * 2 + 1, middle, right, values);
        sums_[node] = sums_[node * 2] + sums_[node * 2 + 1];
    }
    void apply(int node, int length, long long delta) {
        sums_[node] += delta * length;
        pending_[node] += delta;
    }
    void push(int node, int left, int right) {
        if (pending_[node] == 0 || right - left == 1) return;
        int middle = left + (right - left) / 2;
        apply(node * 2, middle - left, pending_[node]);
        apply(node * 2 + 1, right - middle, pending_[node]);
        pending_[node] = 0;
    }
    void addImpl(int node, int left, int right, int ql, int qr, long long delta) {
        if (qr <= left || right <= ql) return;
        if (ql <= left && right <= qr) {
            apply(node, right - left, delta);
            return;
        }
        push(node, left, right);
        int middle = left + (right - left) / 2;
        addImpl(node * 2, left, middle, ql, qr, delta);
        addImpl(node * 2 + 1, middle, right, ql, qr, delta);
        sums_[node] = sums_[node * 2] + sums_[node * 2 + 1];
    }
    long long sumImpl(int node, int left, int right, int ql, int qr) {
        if (qr <= left || right <= ql) return 0;
        if (ql <= left && right <= qr) return sums_[node];
        push(node, left, right);
        int middle = left + (right - left) / 2;
        return sumImpl(node * 2, left, middle, ql, qr) +
               sumImpl(node * 2 + 1, middle, right, ql, qr);
    }
    void checkRange(int left, int right) const {
        if (left < 0 || left > right || right > n_)
            throw std::out_of_range("Invalid half-open range");
    }
public:
    explicit RangeSum(const std::vector<int>& values) : n_(0) {
        if (values.size() > 100000) throw std::invalid_argument("Lab size limit");
        n_ = static_cast<int>(values.size());
        sums_.assign(4 * std::max(1, n_), 0);
        pending_.assign(sums_.size(), 0);
        if (n_ > 0) build(1, 0, n_, values);
    }
    void add(int left, int right, long long delta) {
        checkRange(left, right);
        if (left == right) return;
        addImpl(1, 0, n_, left, right, delta);
    }
    long long sum(int left, int right) {
        checkRange(left, right);
        return left == right ? 0 : sumImpl(1, 0, n_, left, right);
    }
};

class LRU {
    using Entry = std::pair<int, int>;
    std::size_t capacity_;
    std::list<Entry> order_; // Front is most recent.
    std::unordered_map<int, std::list<Entry>::iterator> index_;
public:
    explicit LRU(std::size_t capacity) : capacity_(capacity) {}
    LRU(const LRU&) = delete;
    LRU& operator=(const LRU&) = delete;
    LRU(LRU&&) = delete;
    LRU& operator=(LRU&&) = delete;
    std::optional<int> get(int key) {
        auto found = index_.find(key);
        if (found == index_.end()) return std::nullopt;
        order_.splice(order_.begin(), order_, found->second);
        return found->second->second;
    }
    void put(int key, int value) {
        if (capacity_ == 0) return;
        auto found = index_.find(key);
        if (found != index_.end()) {
            found->second->second = value;
            order_.splice(order_.begin(), order_, found->second);
            return;
        }
        order_.emplace_front(key, value);
        try {
            index_.emplace(key, order_.begin());
        } catch (...) {
            order_.pop_front(); // Restore the invariant if allocation fails.
            throw;
        }
        if (order_.size() > capacity_) {
            index_.erase(order_.back().first);
            order_.pop_back();
        }
    }
};

std::string oneLCS(const std::string& a, const std::string& b) {
    if (a.size() > 500 || b.size() > 500)
        throw std::invalid_argument("Lab supports lengths at most 500");
    int n = static_cast<int>(a.size()), m = static_cast<int>(b.size());
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            dp[i][j] = a[i - 1] == b[j - 1] ? 1 + dp[i - 1][j - 1]
                         : std::max(dp[i - 1][j], dp[i][j - 1]);
    std::string result;
    while (n > 0 && m > 0) {
        if (a[n - 1] == b[m - 1]) {
            result.push_back(a[n - 1]);
            --n; --m;
        } else if (dp[n - 1][m] >= dp[n][m - 1]) {
            --n;
        } else {
            --m;
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

long long knapsack01(const std::vector<int>& weights,
                     const std::vector<int>& values, int capacity) {
    if (capacity < 0 || capacity > 100000 || weights.size() != values.size())
        throw std::invalid_argument("Invalid knapsack input");
    std::vector<long long> best(capacity + 1, 0);
    for (std::size_t i = 0; i < weights.size(); ++i) {
        if (weights[i] <= 0 || values[i] < 0)
            throw std::invalid_argument("Positive weights, nonnegative values required");
        for (int space = capacity; space >= weights[i]; --space)
            best[space] = std::max(best[space], best[space - weights[i]] + values[i]);
    }
    return best[capacity]; // Assumes possible total value fits long long.
}

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

int main() {
    require(windowMaximum({1,3,-1,-3,5,3,6,7}, 3) ==
            std::vector<int>({3,3,5,5,6,7}), "Window maxima");
    require(windowMaximum({-4,-4}, 1) == std::vector<int>({-4,-4}), "Window ties");
    RangeSum tree({1,2,3,4});
    require(tree.sum(0,4) == 10, "Initial sum");
    tree.add(1,3,5);
    require(tree.sum(0,4) == 20 && tree.sum(1,3) == 15, "Range addition");
    tree.add(0,4,-2);
    require(tree.sum(2,4) == 8 && tree.sum(0,4) == 12, "Lazy push");
    RangeSum empty({});
    require(empty.sum(0,0) == 0, "Empty range");
    LRU cache(2);
    cache.put(1,10); cache.put(2,20);
    require(cache.get(1) == std::optional<int>(10), "Cache retrieval");
    cache.put(3,30);
    require(!cache.get(2).has_value(), "Least-recent eviction");
    cache.put(1,11);
    require(cache.get(1) == std::optional<int>(11), "Replacement");
    LRU zero(0); zero.put(1,1);
    require(!zero.get(1).has_value(), "Zero capacity");
    require(oneLCS("abcde", "ace") == "ace", "LCS reconstruction");
    require(oneLCS("", "abc").empty(), "Empty LCS");
    require(knapsack01({2}, {3}, 4) == 3, "Once-per-item rule");
    require(knapsack01({2,3,4}, {3,4,5}, 5) == 7, "Knapsack combination");
    std::cout << "All reference lab checks passed\n";
}
