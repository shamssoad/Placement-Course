# Module 4 — Arrays, search, sort, and hashing

**Expanded textbook:** [Arrays, searching, sorting, and hashing](textbook/04-array-patterns.md). This file is the introductory/revision version.

Prerequisite: Module 3. Outcome: derive a pattern from the problem's structure.

## Lesson 4.1 — Prefixes and difference arrays

Let `prefix[0]=0` and `prefix[i+1]=prefix[i]+a[i]`. The sum of `[l,r)` is `prefix[r]-prefix[l]`: subtracting the earlier prefix cancels every element before l. Preprocessing costs O(n); a query costs O(1). Updates to the original array invalidate later prefix values.

```cpp
#include <vector>
std::vector<long long> makePrefix(const std::vector<int>& a) {
    std::vector<long long> prefix(a.size() + 1, 0);
    for (std::size_t i = 0; i < a.size(); ++i)
        prefix[i + 1] = prefix[i] + a[i];
    return prefix;
}
```

For many range additions followed by one final reconstruction, use differences. Adding x to `[l,r)` means `difference[l] += x` and `difference[r] -= x`, with an extra sentinel position. A final prefix accumulation reveals the additions at every index. This does not answer arbitrary online range queries by itself.

For two-dimensional sums, use inclusion-exclusion: add the whole upper-left rectangle, subtract the two unwanted strips, and add their overlap back once.

**Exercise:** On `[3,-2,5,1]`, compute the sum of `[1,3)`. **Answer:** Prefixes are `[0,3,1,6,7]`; `6-3=3`.

## Lesson 4.2 — Two pointers and safe elimination

In a sorted array, if `a[left]+a[right]` is too small, pairing that left value with any smaller right value cannot reach the target. Thus incrementing left is safe. If the sum is too large, decrementing right is safe. This proof is the reason the technique works; “use two pointers” alone is not an explanation.

Sorting changes element order. If the output requires original indexes, sort value/index pairs or use a hash map instead.

For in-place removal, a write index marks the end of the retained prefix. Scan each input once and copy only accepted items. The invariant is that `[0,write)` contains exactly the accepted elements seen so far, in their original order.

## Lesson 4.3 — Sliding windows and monotonic validity

For a fixed k-element window, subtract the element leaving and add the element entering. For variable windows, the validity rule must support predictable expansion or contraction.

Consider the smallest subarray with sum at least S when all values are positive. Expanding increases the sum; shrinking decreases it. Once a window meets S, shrink repeatedly to find its shortest valid suffix. Negative values destroy this monotonic behavior: adding an element can reduce the sum, so the same reasoning no longer applies.

For “longest substring with at most k distinct characters,” expanding cannot reduce the number of distinct characters. Maintain counts and shrink until valid. The left and right pointers each advance at most n times, giving O(n) expected time with a suitable hash table.

**Exercise:** Give a counterexample to the positive-sum window algorithm on signed inputs. **Answer:** `[1,-1,5]` with target 5 can leave a valid shorter suffix hidden after the sum falls during a shrink; the optimum is `[5]`.

## Lesson 4.4 — Kadane's algorithm

Define `ending` as the maximum sum of a nonempty subarray ending at the current index. A candidate either starts here or extends the best candidate ending at the preceding index. Thus `ending=max(a[i], ending+a[i])`. Track the maximum over all endpoints.

```cpp
#include <algorithm>
#include <stdexcept>
#include <vector>
long long maximumSubarray(const std::vector<int>& a) {
    if (a.empty()) throw std::invalid_argument("Need a nonempty array");
    long long ending = a[0], best = a[0];
    for (std::size_t i = 1; i < a.size(); ++i) {
        ending = std::max<long long>(a[i], ending + a[i]);
        best = std::max(best, ending);
    }
    return best;
}
```

Initialization with the first value preserves the nonempty requirement. Initializing everything to zero incorrectly returns zero for an all-negative array. For maximum product, track both maximum and minimum ending products because multiplying by a negative value reverses their roles; also control overflow.

## Lesson 4.5 — Binary search as a boundary proof

Use a half-open interval `[left,right)`. To find the first value at least target, maintain that everything before left is too small and everything at or after right is a possible qualifying boundary.

```cpp
#include <vector>
std::size_t firstAtLeast(const std::vector<int>& a, int target) {
    std::size_t left = 0, right = a.size();
    while (left < right) {
        std::size_t mid = left + (right - left) / 2;
        if (a[mid] < target) left = mid + 1;
        else right = mid;
    }
    return left; // a.size() means no qualifying element.
}
```

For binary search on an answer, create a monotonic feasibility predicate. In shipping-capacity problems, a capacity feasible within d days stays feasible when increased. Search between the largest package and the sum of all packages, assuming positive weights and sufficient integer range. The feasibility scan greedily fills each day; it cannot benefit from leaving unused space before the next package because package order is fixed.

A rotated-array search compares which half is sorted, then checks whether the target lies in that half. Duplicates can make the sorted-half decision ambiguous and degrade worst-case performance.

## Lesson 4.6 — Sorting tradeoffs

Merge sort recursively sorts halves and merges them. Its work is Θ(n log n); an ordinary array implementation uses O(n) extra storage and can be stable. Quicksort partitions around a pivot; balanced partitions are fast, but repeated poor pivots can produce Θ(n²) work. Heap sort has O(n log n) worst-case time and can operate in place, but is not generally stable.

Counting sort is useful when keys lie in a manageable range K. Its O(n+K) work may beat comparison sorting, but K can make the approach impractical. Do not claim linear time without mentioning the key range.

**Lab:** Implement merge sort with a reusable temporary buffer. Compare its result against `std::sort` for empty arrays, duplicates, sorted arrays, and reverse-sorted arrays.

## Lesson 4.7 — Prefix states plus hashing

A subarray sum is k exactly when `currentPrefix - earlierPrefix == k`. Therefore count how often `currentPrefix-k` has appeared. Look up before inserting the current prefix, otherwise k=0 would incorrectly count an empty subarray.

```cpp
#include <unordered_map>
#include <vector>
long long countSubarrays(const std::vector<int>& a, long long k) {
    std::unordered_map<long long, long long> frequency;
    frequency[0] = 1;
    long long prefix = 0, answer = 0;
    for (int x : a) {
        prefix += x;
        auto it = frequency.find(prefix - k);
        if (it != frequency.end()) answer += it->second;
        ++frequency[prefix];
    }
    return answer;
}
```

The initial zero prefix counts subarrays beginning at index zero. Expected time is O(n); storage is O(n); totals and counts must fit. For the longest matching subarray, store the earliest index of each prefix instead of its frequency. The information stored depends on the question.

## Exercises and answers

1. Move zeros to the end while preserving order. **Answer:** Write nonzero values forward, then fill the remainder with zero.
2. Count zero-sum subarrays in `[0,0]`. **Answer:** Three; prefix frequencies evolve from one initial zero to two and then three.
3. Find the first true position in a monotonic Boolean sequence. **Answer:** Use the boundary-search invariant; handle all-false and all-true cases.
4. Why does pair-sum two-pointer search require ordering? **Answer:** Its discarded candidates are ruled out using value monotonicity.
5. Process many range additions with one final print. **Answer:** Difference array, followed by prefix accumulation.

**Exit ticket:** Solve one unlabeled array problem. Include a brute-force baseline, why the optimization is valid, and a test that would expose a wrong boundary.
