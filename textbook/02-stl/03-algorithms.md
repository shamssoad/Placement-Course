# STL 2.3 — Algorithms and numeric operations

[Previous](02-iterators-callables.md) · [Contents](../../TEXTBOOK.md) · [Next](04-utilities-workflows.md)

## 1. Read an algorithm by its contract

Ask four questions: what range does it read, what does it write, what preconditions does it need, and what does it return? Output ranges must exist unless an insertion iterator grows them. Some overlapping input/output arrangements are valid and others are not; choose copy direction deliberately rather than assuming all overlap is safe.

The following catalogue covers the named C++17 algorithm families taught here. Names in one row share a purpose; predicate and copying variants change the stated part of the contract. Learn a representative operation, then derive why each variant exists.

## 2. Inspection and search

| Names | Meaning and important result |
|---|---|
| `all_of`, `any_of`, `none_of` | Test a predicate over a range; empty all/none are true, empty any is false |
| `for_each`, `for_each_n` | Invoke a callable on every element or the next n elements |
| `count`, `count_if` | Return number of matching elements |
| `find`, `find_if`, `find_if_not` | Return first matching/nonmatching position, or end |
| `mismatch`, `equal` | Compare sequences; use length-aware overloads when lengths may differ |
| `adjacent_find` | Find first matching adjacent pair |
| `find_first_of` | Find first input element matching any element of a second range |
| `search`, `search_n` | Find a subsequence or a run of n matching values |
| `find_end` | Find last matching subsequence |

Most simple inspections are linear and may stop early. Two-range search costs depend on both lengths. `find` is not magically logarithmic on sorted data; binary-search algorithms are a separate family.

```cpp
auto firstFail = std::find_if(marks.begin(), marks.end(),
                             [](int x) { return x < 50; });
if (firstFail != marks.end()) { /* safe to inspect *firstFail */ }
```

## 3. Copying, transformation, and construction

| Names | Meaning |
|---|---|
| `copy`, `copy_n`, `copy_if`, `copy_backward` | Copy all, n, selected, or toward a destination end |
| `move`, `move_backward` | Move elements; source elements remain alive |
| `swap`, `iter_swap`, `swap_ranges` | Exchange values, pointed-to elements, or ranges |
| `transform` | Write unary or binary transformations to an output range |
| `replace`, `replace_if`, `replace_copy`, `replace_copy_if` | Replace matching values in place or into another range |
| `fill`, `fill_n` | Assign the same value to existing elements |
| `generate`, `generate_n` | Obtain each assigned value from a callable |

When shifting elements right within the same initialized array, backward copying can protect unread source values. For a dynamic container insertion, prefer its insert operation unless manually shifting is the exercise. `transform` does not allocate an output container for you.

```cpp
#include <algorithm>
#include <iterator>
#include <vector>
std::vector<int> input{1,2,3}, squared;
std::transform(input.begin(), input.end(), std::back_inserter(squared),
               [](int x) { return x*x; }); // Small-input arithmetic contract.
```

## 4. Removal, rearrangement, and partitioning

| Names | Meaning |
|---|---|
| `remove`, `remove_if`, `remove_copy`, `remove_copy_if` | Compact retained values or copy them elsewhere |
| `unique`, `unique_copy` | Collapse adjacent equivalent values |
| `reverse`, `reverse_copy` | Reverse element order |
| `rotate`, `rotate_copy` | Make a chosen middle become the first element |
| `shuffle` | Random permutation using a supplied generator |
| `sample` | Select a requested-size random sample under the algorithm contract |
| `is_partitioned`, `partition`, `stable_partition` | Test/create true-predicate group followed by false group |
| `partition_copy`, `partition_point` | Split into two outputs; find boundary of an already partitioned range |

Remove does not change a vector's size. It moves retained values forward and returns the logical end. Erase the tail afterward. Unique removes adjacent duplicates only; sorting first makes equal values adjacent, but changes original order.

```cpp
values.erase(std::remove_if(values.begin(), values.end(),
                            [](int x) { return x < 0; }), values.end());
```

Partition is not sorting: elements within each group need not be ordered. Stable partition preserves their relative order. Partition_point requires the predicate to already divide the range monotonically.

## 5. Ordering and selection

| Names | Meaning |
|---|---|
| `sort`, `stable_sort` | Full ordering; stable form preserves equivalent-element order |
| `is_sorted`, `is_sorted_until` | Test order or identify where it first fails |
| `partial_sort`, `partial_sort_copy` | Obtain an ordered smallest prefix |
| `nth_element` | Put the nth element at its sorted position and partition around it |
| `lower_bound`, `upper_bound`, `equal_range`, `binary_search` | Ordered/partitioned-range lookup |

Sort uses O(n log n) comparisons in its C++17 requirement. Nth_element gives average linear selection in the ordinary overload, but does not sort either side. Partial sorting k values is useful when only a small ordered prefix is needed. Stable_sort may use extra memory and has different fallback complexity when memory is insufficient.

Lower_bound returns the first element not less than target. Upper_bound returns the first greater element. Their difference counts equivalent elements in a sorted random-access range. Use the same ordering for sorting and searching; mixing ascending sort with descending search violates the precondition.

## 6. Sorted-range combinations, heaps, and permutations

| Names | Meaning |
|---|---|
| `merge`, `inplace_merge` | Combine sorted ranges into one sorted sequence |
| `includes` | Test sorted multiset containment |
| `set_union`, `set_intersection`, `set_difference`, `set_symmetric_difference` | Sorted multiset operations, preserving appropriate multiplicities |
| `make_heap`, `push_heap`, `pop_heap`, `sort_heap` | Build and maintain heap-formatted ranges |
| `is_heap`, `is_heap_until` | Inspect heap structure |
| `min`, `max`, `minmax`, `clamp` | Select/bound values; avoid dangling references from temporary operands |
| `min_element`, `max_element`, `minmax_element` | Return positions of extrema |
| `lexicographical_compare`, `is_permutation` | Compare sequence order; compare multiplicities ignoring order |
| `next_permutation`, `prev_permutation` | Rearrange to adjacent lexicographic permutation |

`pop_heap` moves the top to the range's last position and restores the preceding heap; it does not erase the last element. A common workflow is pop_heap then pop_back. Sorted-set algorithms require sorted input and correctly sized/output-adapted destinations; the word “set” does not mean they only accept set containers.

Starting next_permutation from sorted values enumerates distinct lexicographic arrangements, including duplicate-aware behavior. When no next arrangement exists, it returns false and rearranges to the first ordering.

## 7. Numeric algorithms

`iota` fills successive values. `accumulate` folds in order with an initial value whose type matters: use `0LL` for a long-long sum. `inner_product` combines paired elements and folds them. `partial_sum` builds prefixes; `adjacent_difference` expresses changes between neighbors.

C++17 adds `reduce`, `transform_reduce`, `inclusive_scan`, `exclusive_scan`, `transform_inclusive_scan`, and `transform_exclusive_scan`. Reduce can regroup operations; floating-point addition and subtraction are not mathematically associative under finite precision, so it need not reproduce a left-to-right accumulation. An exclusive sum scan emits the sum before each input, while an inclusive scan includes that input.

`gcd` and `lcm` provide integer arithmetic helpers with representability preconditions. Do not assume library arithmetic functions eliminate overflow in the mathematical result.

## 8. Execution policies and practice

C++17 provides execution-policy overloads for many algorithms. They permit different execution arrangements but do not guarantee useful parallel speedup. Shared mutation inside callbacks may race; unsequenced execution has additional restrictions. Exceptions from user operations under standard execution policies can terminate rather than propagate normally. Begin with ordinary sequential algorithms until the callback and performance contracts are understood.

**Exercises:** (1) Get the smallest ten values sorted: partial_sort. (2) Find only a median value: nth_element with defined even-length policy. (3) Remove duplicate adjacent log events without reordering: unique plus erase. (4) Compute prefix totals: partial_sum or inclusive_scan with appropriate arithmetic. (5) Merge two sorted reports: merge into a sufficiently sized destination or back_inserter.

For each answer, write the preconditions and returned result before coding. That habit is more useful than memorizing every overload signature.
