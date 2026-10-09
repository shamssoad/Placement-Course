# Volume 4 — Arrays, searching, sorting, and hashing

[Contents](../TEXTBOOK.md) · [Introductory module](../04-arrays-search-sort-hashing.md)

## Chapter 1. Build the brute-force baseline

For “maximum subarray sum,” enumerate every start and end. Recomputing each sum from scratch is O(n³); extending a running sum for each start reduces it to O(n²). A prefix array also gives constant-time sum per candidate but still leaves O(n²) candidates. Distinguish cheaper candidate evaluation from fewer candidates.

The baseline reveals structure and supplies a correctness oracle for small generated tests. Before optimizing, define whether the subarray may be empty, how ties are resolved, and what to return for an empty input. These choices change initialization and answer representation.

## Chapter 2. Prefixes, suffixes, and differences

A prefix array stores an aggregate from the beginning. Range subtraction works for sums because addition has an inverse. Prefix minima do not generally support arbitrary range minima by “subtracting” two prefixes; the earlier minimum can hide all information about the desired range.

For product-except-self without division, build each position's product of earlier elements, then multiply by a running product of later elements. This naturally handles zeros, unlike dividing the total by each value. It still needs a numeric-range contract.

A difference array records changes, not final values. Adding d to `[l,r)` changes only two boundaries; a prefix pass reconstructs the per-position additions. In two dimensions, a rectangle update changes four corners, and a two-dimensional prefix reconstruction recovers the field. The signs follow inclusion-exclusion.

## Chapter 3. Two pointers and windows

Sorted pair sum works because one pointer move eliminates a provably impossible set of pairs. For 3-sum, fix one value and solve a two-pointer pair problem on the suffix. Sorting and skipping equal fixed/side values prevents duplicate value-triplets. If indexes matter, retain original indexes and define duplicate-output semantics separately.

A sliding window maintains a property as endpoints move. For longest substring with at most k distinct symbols, counts determine validity. Increasing the right boundary can violate the limit; advance left until it is restored. For exactly k distinct, “longest exactly k” can check distinct count before updating an answer. Counting exactly k can often use `countAtMost(k)-countAtMost(k-1)`, because every eligible window belongs to one distinct-count class.

For at-most counting, once `[left,right]` is valid and the property is preserved by shortening, all suffixes starting from left through right are valid, contributing `right-left+1`. This counting step needs proof; it is not applicable to arbitrary predicates.

## Chapter 4. Binary search on values and answers

Boundary search maintains a false region and a true region for a monotonic predicate. In a half-open representation, the result may equal n, representing no qualifying element. Decide that outcome before indexing it.

For minimum shipping capacity, feasibility packs packages in order until the next one would exceed capacity, then starts a day. Larger capacity never needs more days. Lower bound is the maximum package; upper bound is total weight. Each predicate costs O(n), so total time is O(n log U) for the searched numeric range U.

```cpp
#include <algorithm>
#include <stdexcept>
#include <vector>
long long minimumCapacity(const std::vector<int>& weights, int days) {
    if (weights.empty() || days <= 0) throw std::invalid_argument("Invalid input");
    long long low = 0, high = 0;
    for (int w : weights) {
        if (w <= 0) throw std::invalid_argument("Positive weights required");
        low = std::max(low, static_cast<long long>(w)); high += w;
    }
    auto feasible = [&](long long capacity) {
        int used = 1; long long load = 0;
        for (int w : weights) {
            if (load + w > capacity) { ++used; load = 0; }
            load += w;
        }
        return used <= days;
    };
    while (low < high) {
        long long mid = low + (high-low)/2;
        if (feasible(mid)) high = mid;
        else low = mid+1;
    }
    return low;
}
```

Assume size and total fit the selected types. For `[1,2,3,4,5]`, d=2, capacity 8 needs three days, while 9 needs two; the answer is 9. Test the predicate independently before debugging the binary search around it.

## Chapter 5. Sorting algorithms from invariants

Insertion sort maintains a sorted prefix and inserts the next element by shifting larger values. It is quadratic in the worst case but efficient for small/nearly ordered data and stable when equal values are not shifted past each other.

Merge sort maintains two sorted subranges and repeatedly takes the smaller front. Choose the left on equality to preserve stability. Quicksort's partition invariant separates values relative to the pivot. A two-way partition can behave badly with many equal keys; three-way partitioning explicitly groups less/equal/greater. Randomized pivot selection improves expected behavior but does not remove the mathematical worst case.

Heap sort builds a heap, exchanges the root with the final active element, shrinks the active range, and restores the heap. It gives O(n log n) worst-case time with small auxiliary storage, but does not preserve equal-element order.

Counting sort uses frequencies over a bounded key range. Stable reconstruction uses cumulative positions. Radix sorting applies stable sorting to successive digit positions; stability preserves information from earlier digit passes. Its cost depends on digits and base, not just n.

## Chapter 6. Hash the information the question needs

For exact subarray count, store prefix frequencies. For longest exact subarray, store earliest prefix positions. For shortest, recent positions may be useful. Storing the wrong summary can make the desired answer unrecoverable even though the same high-level technique applies.

For anagram grouping, a canonical key can be a sorted string or fixed-alphabet count tuple. Sorting each word costs O(L log L); counting costs O(L+alphabet) with a suitable key representation. Hash collisions must be resolved by equality; an integer hash alone is not a safe unique identity.

## Chapter 7. Selection and intervals

Quickselect partitions and recurses only into the side containing the desired rank. Expected linear work depends on suitable pivot behavior. For top k in a stream, a size-k heap uses O(n log k) time and does not need all n values stored.

To merge overlapping intervals, sort by start and maintain the last merged interval. If the next start lies within the current interval under your endpoint convention, extend the end; otherwise start a new group. Half-open intervals touching at an endpoint can be treated as disjoint or merged according to the problem's requested representation—state the rule.

## Exercises with solutions

1. Count subarrays with exactly two distinct values. **Solution:** Subtract two at-most-window counts with frequency maintenance.
2. Does prefix minimum answer arbitrary range minimum? **Solution:** No; unlike sums, there is no cancellation operation recovering discarded values.
3. Why is the shipping predicate monotonic? **Solution:** Any partition fitting capacity C also fits a larger capacity.
4. Sort events while preserving arrival order for equal keys. **Solution:** Use a stable algorithm or add arrival position as a secondary key when that matches the contract.
5. Return all 3-sum value triples. **Solution:** Sort, fix an index, use two pointers, and skip equivalent values at each relevant step; O(n²) after sorting, excluding output storage.

**Practical gate:** Implement a brute-force and optimized version for three patterns, compare small cases, and explain the assumption that makes each optimization valid.
