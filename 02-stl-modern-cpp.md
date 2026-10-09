# Module 2 — STL and modern C++

**Expanded textbook:** Start with [containers and adaptors](textbook/02-stl/01-containers.md), then follow the four linked chapters. This file is the introductory/revision version.

Prerequisite: Module 1. Outcome: choose tools using operation costs and lifetime requirements.

## Lesson 2.1 — Choosing a sequence container

`vector` stores elements contiguously. Indexing is O(1); inserting into the middle usually moves later elements and costs O(n). Appending is amortized O(1), although a particular append may allocate new storage and move all existing elements. `size()` counts constructed elements; `capacity()` describes available storage. `reserve()` changes capacity without creating elements; `resize()` changes size.

`deque` supports efficient insertion at both ends and indexed access, but its storage is not one contiguous array. `list` supports constant-time insertion next to a known iterator, but finding the position is still O(n), and nodes introduce allocation and pointer costs. “Lots of insertions” alone is not sufficient reason to choose a linked list.

Reallocation invalidates vector pointers, references, and iterators. Without reallocation, insertion can still invalidate positions at or after the insertion. Do not hold a borrowed reference and assume `push_back` cannot affect it.

**Exercise:** Which container fits an append-heavy list with random indexing? **Answer:** Usually `vector`; reserve expected capacity when that estimate is useful.

## Lesson 2.2 — Maps, sets, and lookup behavior

Ordered maps and sets support O(log n) lookup and ordered traversal. Unordered variants have average O(1) lookup under suitable hashing, but worst-case lookup can be O(n). Their iteration order is not sorted or a stable presentation order.

```cpp
#include <string>
#include <unordered_map>
#include <vector>
std::unordered_map<std::string, int>
frequencies(const std::vector<std::string>& words) {
    std::unordered_map<std::string, int> counts;
    for (const auto& word : words) ++counts[word];
    return counts;
}
```

`counts[word]` inserts a zero-initialized integer when the word is absent. That is useful for counting but dangerous for read-only lookup. Use `find` when absence must not mutate the map. A custom key needs equality and hashing that agree: equal keys must have equal hashes.

**Lab:** Count words, then print them alphabetically. Choose either an ordered map or sort the keys from an unordered map. Explain the tradeoff.

## Lesson 2.3 — Adaptors and priority

A stack exposes last-in-first-out operations; a queue exposes first-in-first-out operations. Restricting operations can make the intended algorithm clearer.

A priority queue exposes the highest-priority item, not arbitrary indexed retrieval. Its default ordering gives a max-heap. A min-heap uses `std::greater`:

```cpp
#include <functional>
#include <queue>
#include <vector>
std::priority_queue<int, std::vector<int>, std::greater<int>> minimumFirst;
```

Push and pop cost O(log n); reading the top costs O(1). Calling `top()` on an empty queue is invalid, so check `empty()` when emptiness is possible.

## Lesson 2.4 — Algorithms and half-open ranges

Library ranges commonly use `[first, last)`: include the first position, exclude the last. An empty range has `first == last`, and the convention composes naturally with container `begin()` and `end()`.

`lower_bound` finds the first element not less than a target; `upper_bound` finds the first greater element. On a sorted vector, these support logarithmic comparison counts and efficient random-access movement. On non-random-access iterators, iterator movement can be linear. For an ordered set, prefer its member `lower_bound`.

```cpp
#include <algorithm>
#include <vector>
int occurrences(const std::vector<int>& sorted, int target) {
    auto left = std::lower_bound(sorted.begin(), sorted.end(), target);
    auto right = std::upper_bound(sorted.begin(), sorted.end(), target);
    return static_cast<int>(right - left);
}
```

The function requires sorted input. A binary-search algorithm does not make unsorted input meaningful. Similarly, `unique` removes adjacent duplicates logically and returns a new logical end; it does not shrink the container by itself.

```cpp
std::sort(values.begin(), values.end());
values.erase(std::unique(values.begin(), values.end()), values.end());
```

## Lesson 2.5 — Comparators, lambdas, and generic code

```cpp
#include <algorithm>
#include <string>
#include <vector>
struct Candidate { std::string name; int score; };
void rankCandidates(std::vector<Candidate>& candidates) {
    std::sort(candidates.begin(), candidates.end(),
        [](const Candidate& a, const Candidate& b) {
            if (a.score != b.score) return a.score > b.score;
            return a.name < b.name;
        });
}
```

The comparator must define a strict weak ordering. Using `a.score >= b.score` fails because it claims an element precedes itself. For equal scores, the example uses a name tie-breaker. Use `stable_sort` when retaining the original order of equivalent elements is the actual requirement.

Lambda captures can own copies or borrow references. A reference capture that outlives the referred local object dangles. Templates allow the same logic to work for multiple types, but those types still need to support the operations the template performs.

## Lesson 2.6 — Ownership, moves, and borrowed views

Copying a vector copies its elements. Moving a vector can transfer its storage rather than duplicate it; `std::move` itself is a cast that enables move operations. A moved-from standard-library object remains valid but its value may be unspecified unless a stronger guarantee is documented. Do not assume its old contents remain.

`unique_ptr` expresses sole ownership. `shared_ptr` expresses shared lifetime and has bookkeeping costs; use it when ownership is actually shared. A reference or raw pointer can express a nonowning relationship when the owner's lifetime is clear.

`string_view` is a borrowed view, not a string owner. A view into a temporary string becomes dangling. C++20 `span` similarly provides a view over contiguous elements without owning them. Views can reduce copying, but ownership contracts become more important.

## Exercise set and answers

1. Preserve the largest five values in a stream. **Answer:** Keep a min-heap of at most five; replace its smallest item when a larger one arrives. O(n log 5) processing.
2. Count a target in a sorted vector. **Answer:** Subtract upper and lower bounds, as above.
3. Why can `reserve(100)` followed by `values[0] = 2` be invalid? **Answer:** Capacity is not size; no element exists until constructed.
4. Diagnose `string_view view = std::string("hello");`. **Answer:** The temporary owner dies at the statement's end.
5. Sort records by descending score and ascending ID. **Answer:** Compare score first, then ID using strict comparisons.

**Exit ticket:** Give costs and invalidation risks for each container used in your project. Refactor one loop with a library algorithm and explain why the result preserves behavior.
