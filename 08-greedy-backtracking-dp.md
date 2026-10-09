# Module 8 — Greedy, backtracking, and dynamic programming

**Expanded textbook:** [Greedy reasoning, backtracking, and DP](textbook/08-dynamic-programming.md). This file is the introductory/revision version.

Prerequisite: Modules 3–7. Outcome: derive decisions, states, and proofs.

## Lesson 8.1 — Greedy needs an exchange argument

To choose the largest number of nonoverlapping intervals, sort by finishing time and take each compatible interval. Why not earliest start or shortest duration? An early-starting long interval can block many later ones; a short interval in the middle can block two compatible intervals around it.

Let E be the earliest-finishing interval. In an optimal schedule, replace its first chosen interval with E. E finishes no later, so every later choice still fits. There is an optimal schedule starting with E. Repeating the argument on the remaining intervals proves the algorithm.

```cpp
#include <algorithm>
#include <utility>
#include <vector>
int maximumActivities(std::vector<std::pair<int,int>> intervals) {
    std::sort(intervals.begin(), intervals.end(), [](auto a, auto b) {
        return a.second < b.second;
    });
    bool selected = false;
    int finish = 0, count = 0;
    for (auto [start, end] : intervals) {
        if (!selected || start >= finish) {
            selected = true; finish = end; ++count;
        }
    }
    return count;
}
```

Assume positive-length half-open intervals; touching endpoints are compatible. Sorting costs O(n log n). Fractional knapsack can greedily choose value density because fractions are allowed. The same rule fails for 0/1 knapsack: choosing one high-density item may block a more valuable combination.

## Lesson 8.2 — Backtracking is reversible state

Backtracking explores choices and reverses each change before exploring siblings. The central invariant is that the current state contains exactly the choices on the active recursion path.

```cpp
#include <functional>
#include <vector>
std::vector<std::vector<int>> subsets(const std::vector<int>& values) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    std::function<void(std::size_t)> visit = [&](std::size_t index) {
        if (index == values.size()) {
            result.push_back(current);
            return;
        }
        visit(index + 1);
        current.push_back(values[index]);
        visit(index + 1);
        current.pop_back();
    };
    visit(0);
    return result;
}
```

This treats positions as distinct; repeated values may produce duplicate value-subsets. To avoid duplicates, sort and skip equivalent sibling choices. Output size itself is exponential, so no algorithm can list every subset in polynomial total time.

For N-Queens, track used columns and diagonals, whose identities are `row-column` and `row+column`. Reject impossible choices before recursing. For Sudoku, validate the given board first, then choose an empty cell with few candidate values to improve pruning. Pruning changes practical work but does not remove exponential worst cases.

## Lesson 8.3 — Define a DP state in a sentence

Dynamic programming stores answers to overlapping subproblems. A state must include all information needed for future choices; it should exclude irrelevant history. Write its meaning before the recurrence.

For the maximum sum of nonadjacent elements allowing an empty selection, let `best[i]` be the optimum over the first i elements. Either skip the last element, giving `best[i-1]`, or take it, giving `best[i-2]+a[i-1]`. Base values are `best[0]=0` and `best[1]=max(0,a[0])`. Space can shrink to two preceding values because no transition reads older states.

Memoization computes states on demand; tabulation chooses an order that satisfies dependencies. Both can implement the same recurrence. A memoized function can still overflow the call stack on a long dependency chain.

## Lesson 8.4 — Coin change and loop meaning

For the minimum number of coins totaling amount A, define `dp[s]` as the minimum count for sum s. Set `dp[0]=0`; all positive sums begin unreachable. For every sum, consider its last coin.

```cpp
#include <algorithm>
#include <stdexcept>
#include <vector>
int minimumCoins(const std::vector<int>& coins, int amount) {
    if (amount < 0 || amount > 1000000)
        throw std::invalid_argument("Unsupported amount");
    for (int coin : coins)
        if (coin <= 0) throw std::invalid_argument("Coins must be positive");
    const int INF = amount + 1;
    std::vector<int> dp(amount + 1, INF);
    dp[0] = 0;
    for (int sum = 1; sum <= amount; ++sum)
        for (int coin : coins)
            if (coin <= sum && dp[sum - coin] != INF)
                dp[sum] = std::min(dp[sum], dp[sum - coin] + 1);
    return dp[amount] == INF ? -1 : dp[amount];
}
```

Complexity is O(A * number of coins), which depends on the numeric amount, not only the bit-length of its representation. This is pseudopolynomial complexity.

For counting combinations, process each coin outside and sums in ascending order. For counting ordered sequences, process sums outside and coins inside. With coins 1 and 2 and amount 3, combinations are `{1,1,1}` and `{1,2}`; ordered sequences also distinguish `{2,1}`. Duplicate denominations must be removed if they do not represent different types.

## Lesson 8.5 — Knapsack and update direction

For 0/1 knapsack, each item can be used once. A one-dimensional table must update capacities in descending order; otherwise an update for the current item can feed another update and reuse it. For unbounded knapsack, ascending updates intentionally allow repeated use.

Subset sum is the Boolean version: `possible[s]` asks whether a sum can be achieved. Start with sum zero possible. Descending updates preserve the once-per-item rule. Negative inputs require a different state range or formulation; the ordinary nonnegative table is not enough.

**Exercise:** Why does ascending order incorrectly solve 0/1 subset sum for one item of weight 2 and target 4? **Answer:** It marks sum 2, then uses that newly marked state to mark sum 4, selecting the item twice.

## Lesson 8.6 — Sequence DP

LCS uses `dp[i][j]` for the first i characters of one string and first j of the other. If the last characters match, use `1+dp[i-1][j-1]`; otherwise choose the better of dropping one last character. This takes O(nm) time. Keeping the full table supports reconstruction by walking backward through the chosen transitions.

Edit distance considers delete, insert, and replace, with empty-prefix lengths as base cases. Explain the operation represented by each neighboring cell. Costs need not all be one, but the recurrence must reflect the chosen model.

LIS has an O(n²) formulation: `dp[i]` is the longest increasing subsequence ending at i. A faster method stores the smallest possible tail for each achievable length and uses `lower_bound`. The tail array is not necessarily the actual recovered subsequence; store predecessor information when reconstruction is required. `upper_bound` changes the problem to nondecreasing subsequences.

## Lesson 8.7 — Advanced state designs

**Interval DP:** For matrix-chain multiplication, `dp[l][r]` is the minimum cost to multiply matrices l through r. Try each split k and add left cost, right cost, and the final multiplication cost. Initialize single matrices to zero. Increasing interval length satisfies dependencies.

**Tree DP:** For independent-set value, keep `take[u]` and `skip[u]`. Taking u forces all children skipped; skipping u allows the better child state. Root the tree and avoid returning through the parent edge.

**Bitmask DP:** For visiting a small number of cities, `dp[mask][last]` records which cities were visited and the final city. Tracking only `last` loses essential history. O(2^n n²) time means n must be small.

**Digit DP:** Count numbers up to a bound with states for position, whether the prefix remains tight to the bound, leading-zero status if relevant, and a property such as digit sum. Reusing a memo entry across different tight bounds can be incorrect.

## Exercises and answers

1. Count obstacle-free grid paths. **Answer:** Each cell sums states from above and left; blocked cells contribute zero. Choose a count type or modulus that fits.
2. Produce one minimum-coin solution. **Answer:** Record which coin improved each reachable state and follow those choices back to zero.
3. Longest increasing versus contiguous increasing run? **Answer:** A subsequence can skip elements; the run cannot. They need different states.
4. Explain memoization's benefit for Fibonacci. **Answer:** Each distinct argument is evaluated once rather than repeatedly across branches.
5. Find a counterexample to greedy largest-coin selection. **Answer:** Coins `[1,3,4]`, amount 6; greedy uses three coins, optimum is two 3s.

**Exit ticket:** Solve a new DP problem by submitting state meaning, transitions, bases, evaluation order, answer extraction, complexity, and reconstruction strategy before code.
