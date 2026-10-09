# STL 2.2 — Iterators, invalidation, and callables

[Previous](01-containers.md) · [Contents](../../TEXTBOOK.md) · [Next](03-algorithms.md)

## 1. Iterators separate algorithms from storage

An iterator represents a position and supports a defined collection of operations. Dereferencing reads or writes an element when that position is valid. Increment advances. `end()` denotes the boundary after the final element; it is not an element to dereference.

The half-open range `[first,last)` makes empty ranges natural and allows adjacent ranges to share a boundary. Both iterators must describe a valid range from the appropriate sequence. Mixing begin from one vector with end from another is invalid.

| C++17 category | Capabilities | Example |
|---|---|---|
| Input | Read a single-pass sequence | Stream input iterator |
| Output | Write a sequence | Back-insert iterator |
| Forward | Multipass traversal forward | forward_list iterator |
| Bidirectional | Forward and backward | list/set iterator |
| Random access | Constant-time jumps, difference, ordering | vector/deque iterator |

A random-access iterator need not imply contiguous storage: deque is the important example. C++20 explicitly models contiguous iterators for cases where address relationships hold.

## 2. Generic traversal tools

`std::next(it,n)` returns an advanced copy; `std::advance(it,n)` changes the supplied iterator. `distance(first,last)` can be constant time for random access and linear otherwise. Never assume a helper's compact syntax makes it constant-time.

Reverse iterators reverse traversal direction. Their `base()` refers to the forward position after the reverse-referred element. Converting a reverse result for erasure therefore requires care; blindly erasing `rit.base()` removes the wrong position or end.

Insertion iterators adapt assignment into insertion: `back_inserter`, `front_inserter`, and `inserter`. Use them when an output algorithm should grow a container. Ordinary `copy` into `destination.begin()` requires enough existing elements; reserved capacity is not enough.

```cpp
#include <algorithm>
#include <iterator>
#include <vector>
std::vector<int> source{1,2,3}, destination;
std::copy(source.begin(), source.end(), std::back_inserter(destination));
```

Move iterators change dereference expressions so an algorithm can move from elements. Moving leaves the source elements alive but possibly changed; it does not erase them.

## 3. Invalidation is part of correctness

| Operation | Essential rule |
|---|---|
| vector reallocation | All iterators, pointers, and references into elements are invalidated |
| vector insert without reallocation | Positions at/after insertion, including old end, are invalidated |
| vector erase | Positions at/after erasure are invalidated |
| deque insert at either end | Iterators invalidated; references to existing elements survive |
| deque middle insert/erase | Broad invalidation; do not retain element borrows across it |
| list/forward_list insert | Existing element iterators/references survive |
| list/forward_list erase | Only erased elements' iterators/references are invalidated |
| ordered associative insert | Existing element iterators/references survive |
| ordered associative erase | Only erased elements' iterators/references are invalidated |
| unordered rehash | Iterators invalidated; references/pointers to existing elements survive |
| unordered erase | Only erased elements' iterators/references are invalidated |

For deque erasure at the front without removing the last element, remaining element iterators/references survive; tail erasure changes the past-end boundary. Its detailed end cases are why reacquiring iterators after mutation is often the clearest local code. Clearing destroys every element. Swapping containers can preserve element iterators while changing which container owns those elements; cached end iterators require separate care.

```cpp
// Remove odd values safely from a vector.
for (auto it = values.begin(); it != values.end(); ) {
    if (*it % 2 != 0) it = values.erase(it);
    else ++it;
}
```

This is correct but potentially quadratic due to repeated shifts. The erase-remove idiom performs one compaction and one erase, which is usually better for bulk filtering.

## 4. Predicates and strict ordering

A predicate answers a Boolean question. An ordering comparator says whether a belongs before b. For sort, it must be irreflexive and consistent with a strict weak ordering. `a<=b` is wrong because it returns true for equal objects.

For descending score and ascending name:

```cpp
auto compare = [](const Candidate& a, const Candidate& b) {
    if (a.score != b.score) return a.score > b.score;
    return a.name < b.name;
};
```

Comparator equivalence means neither a<b nor b<a. Ordered sets/maps use that notion to determine key equivalence; it need not be identical to operator==. Sorting floating-point values containing NaNs requires an explicitly valid ordering policy rather than assuming ordinary `<` handles all cases.

## 5. Function objects and captures

A callable can be a function pointer, lambda, or object with `operator()`. A stateful functor stores configuration such as a threshold. Algorithms may copy their callables; do not rely on mutation of an internal copied counter unless the API explicitly returns the relevant object or you arrange referenced state deliberately.

`std::ref` creates a reference wrapper for APIs that normally copy arguments. The wrapper does not extend the target lifetime. `std::bind` can prebind arguments, but a lambda usually makes ownership and parameter mapping easier to read. `std::invoke` uniformly invokes ordinary callables and supported member pointers.

## Exercises with solutions

1. Why can't generic `sort` sort a list? **Answer:** It requires random-access iterators; list supplies a member sort designed for its representation.
2. Why is a cached vector end unsafe after push_back? **Answer:** End changes even if storage does not reallocate.
3. Does unordered rehash move every referred-to element? **Answer:** It invalidates iterators but preserves references/pointers to existing elements; these are distinct guarantees.
4. How do you copy into an initially empty vector? **Answer:** Resize first or use back_inserter; reserve alone does not create destinations.
5. A predicate stores a reference to a local threshold and escapes. **Answer:** Capture an owned value or make the caller's lifetime guarantee explicit.

**Lab:** For every container used in your application, write one valid mutation loop and one invalidated-iterator anti-example. Explain the exact invalidating event without executing undefined behavior.
