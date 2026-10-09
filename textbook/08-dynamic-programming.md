# Volume 8 — Greedy algorithms, backtracking, and dynamic programming

[Contents](../TEXTBOOK.md) · [Introductory module](../08-greedy-backtracking-dp.md)

## Chapter 1. Three ways to manage decisions

Greedy commits to a locally chosen decision and proves there is no need to reconsider it. Backtracking explores alternatives, undoing choices as it returns. DP merges repeated subproblems by recognizing which histories lead to the same future problem. They are not interchangeable labels for “hard algorithms.”

For activity selection, earliest finish has an exchange proof. For weighted intervals, choosing the earliest finish can discard a much more valuable interval; a DP must compare skipping an interval with taking it plus the best compatible prefix. A small requirement change can invalidate the greedy argument.

## Chapter 2. Prove a greedy choice

Use either an exchange argument or a stays-ahead invariant. In activity selection, replace an optimal solution's first interval with the earliest-finishing one without harming later choices. In merging ropes with minimum total merge cost, repeatedly joining the two smallest weights is justified by the structure of weighted merge trees; simply saying “smaller is cheaper” is insufficient for every scheduling problem.

For fractional knapsack, taking value per unit weight in descending order is valid because portions can be exchanged continuously. For indivisible items, that exchange may be impossible. Coins `[1,3,4]` and amount 6 defeat largest-denomination-first: 4+1+1 loses to 3+3.

## Chapter 3. Backtracking state, pruning, and duplicates

Each recursive frame represents one partial decision sequence. Maintain an invariant that mutable state matches that sequence, and undo every mutation before trying the next sibling. If returning early on a found solution, ensure ownership and output semantics remain deliberate.

For combinations of duplicate values, sort the input and skip equal choices at the same recursion depth. Do not globally skip every repeated value: the combination may legitimately contain two equal values from different positions. For permutations, a used array distinguishes positions, and a duplicate rule avoids equivalent sibling arrangements.

Pruning must prove no completion of that branch can succeed or improve the best answer. For positive numbers targeting a sum, exceeding the target can prune. With negative values, that argument fails because a later value might reduce the sum. Bounds are only valid under their assumptions.

## Chapter 4. The seven-part DP specification

Write: state meaning, state range, base cases, transition, evaluation order, answer location, and complexity. Add reconstruction if an actual choice sequence is required. “dp[i] stores the answer” is not a definition.

For edit distance, `dp[i][j]` is the minimum edits transforming the first i source characters into the first j target characters. Empty-source cost is j insertions; empty-target cost is i deletions. A matching final character uses `dp[i-1][j-1]`. Otherwise choose delete, insert, or replace plus the relevant cost.

For `cat` to `cut`, the outer c and t match; replacing a with u gives one edit. The table's neighbors represent concrete operations, not arbitrary cells to memorize. Reconstruct by following a transition achieving the current value.

## Chapter 5. Knapsack and coin variants

| Problem | State/result | Crucial update rule |
|---|---|---|
| 0/1 knapsack | Best value at capacity | Descend capacity to use each item once |
| Unbounded knapsack | Best value with repeated items | Ascend capacity to allow current-item reuse |
| Subset feasibility | Whether sum is reachable | Boolean combination; descend for 0/1 |
| Minimum coins | Fewest coins for exact sum | Min over last coin; impossible sentinel |
| Count combinations | Number of unordered multisets | Coin outside, sums ascending |
| Count sequences | Number of ordered sequences | Sum outside, coin choices inside |

For counting, overflow often occurs much earlier than students expect. Choose an exact big-number representation or a problem-specified modulus. Applying a convenient modulus changes the requested answer unless the contract permits it.

The [reference lab](../examples/algorithm_lab.cpp) includes 0/1 knapsack with a test that catches the wrong loop direction.

## Chapter 6. LIS and reconstruction

Quadratic LIS sets `length[i]=1+max(length[j])` over j<i with a[j]<a[i]. Store predecessor j when improving to reconstruct a subsequence. The O(n log n) method stores the smallest possible ending value for each length and replaces a tail using lower_bound.

For `[3,1,2,5,4]`, tails evolve `[3]`, `[1]`, `[1,2]`, `[1,2,5]`, `[1,2,4]`. Length is three. Tails optimizes future extensibility, not necessarily one consistent historical subsequence in every instance. Keep tail indexes and predecessor links for reconstruction.

## Chapter 7. Interval DP from the last operation

Matrix-chain multiplication asks where to split the final multiplication. If dimensions are p0,p1,...,pn, splitting between k and k+1 costs `dp[l][k]+dp[k+1][r]+p[l-1]*p[k]*p[r]`. Increasing interval lengths ensure smaller intervals are ready.

```cpp
#include <algorithm>
#include <limits>
#include <vector>
long long matrixChain(const std::vector<int>& p) {
    // Preconditions: 2 <= p.size() <= 51; dimensions in [1,1000].
    int n=static_cast<int>(p.size())-1;
    std::vector<std::vector<long long>> dp(n, std::vector<long long>(n,0));
    for (int len=2; len<=n; ++len)
        for (int l=0; l+len<=n; ++l) {
            int r=l+len-1;
            dp[l][r]=std::numeric_limits<long long>::max();
            for (int k=l; k<r; ++k)
                dp[l][r]=std::min(dp[l][r], dp[l][k]+dp[k+1][r]
                                  +1LL*p[l]*p[k+1]*p[r+1]);
        }
    return dp[0][n-1];
}
```

For dimensions `[10,30,5,60]`, multiplying the first two first costs 1500+3000=4500; multiplying the last two first costs 9000+18000=27000. The recurrence explores both parenthesizations. Time O(n³), space O(n²).

## Chapter 8. Tree, DAG, bitmask, and digit states

Tree independent set uses take/skip values. Taking a node forces children skipped; skipping it allows each child's better state. Rooting establishes dependencies, but the algorithm must not treat an arbitrary cyclic graph as a tree.

Bitmask DP represents a small chosen set. In traveling-salesperson-style DP, `(mask,last)` retains both visited membership and current endpoint. Complexity O(2^n n²) is practical only for small n. Estimate states times transitions before allocating.

Digit DP tracks position, tightness to the upper bound, and the property being counted. Leading zeros may or may not be actual digits under the property, so track a started flag when needed. To count over `[L,R]`, subtract counts up to L-1 from counts up to R, treating L=0 carefully.

Rerooting DP reuses information to compute an answer for every possible tree root. First aggregate child contributions downward; then pass the complementary contribution from parent to child. Prefix/suffix child aggregates can exclude one child efficiently. This extends the same “what information must a neighbor receive?” reasoning.

## Exercises with solutions

1. Weighted interval scheduling state? **Answer:** Best value among the first i finish-sorted intervals; compare skipping i with taking its value plus the best compatible predecessor prefix.
2. Why is state `(index)` insufficient for subset sum recursion? **Answer:** Future success also depends on remaining sum.
3. How reconstruct matrix parentheses? **Answer:** Store the split k attaining each interval minimum, then recursively print both subintervals.
4. When can DP rows be discarded? **Answer:** Only after no future transition or required reconstruction needs them.
5. Why can pruning by “sum exceeded target” fail? **Answer:** Negative remaining values can bring the sum back down.

**Practical gate:** Derive three unfamiliar recurrences without looking at code. Submit the seven-part specification and a trace on a tiny instance before implementation.
