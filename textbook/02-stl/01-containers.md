# STL 2.1 — Containers and adaptors

[Contents](../../TEXTBOOK.md) · [Next](02-iterators-callables.md)

## 1. Start with operations, not container names

A container owns a collection and defines how to access and change it. Choose it by the operations you need: indexing, ordered lookup, membership, stable references, insertion near a known position, or priority extraction. Big-O is only one factor: locality, allocation, object size, and iterator stability also matter.

Most containers provide `empty`, `size`, `begin/end`, constructors, assignment, and `swap`. `forward_list` has no size member. `array` has fixed size and cannot insert or erase. Adaptors intentionally expose a narrower interface. Type aliases such as `value_type`, `iterator`, and `size_type` support generic code.

## 2. Contiguous sequences: array and vector

`array<T,N>` stores exactly N elements as part of the object. It supports indexing, `at`, `front/back` when nonempty, `data`, `fill`, and iteration. It cannot grow. Copying copies all elements. A zero-length array is allowed, but accessing its front is not.

`vector<T>` supports dynamic growth: `push_back`, `emplace_back`, `pop_back`, `insert`, `emplace`, `erase`, `clear`, `resize`, `reserve`, `capacity`, and `shrink_to_fit`. The last is a nonbinding request to reduce capacity. `clear` destroys elements but need not release capacity. `erase` shifts later elements and returns a usable iterator to the following position.

```cpp
#include <vector>
std::vector<int> scores{10,20,30};
scores.reserve(20);
scores.insert(scores.begin() + 1, 15); // 10,15,20,30
auto next = scores.erase(scores.begin() + 2); // 10,15,30; next points to 30.
```

Appending is amortized constant time; middle insertion/erasure is linear. `emplace_back(args...)` constructs an element from arguments; it is not automatically faster in every situation than pushing a value. Avoid repeated `reserve(size()+1)`, which can defeat geometric growth.

`vector<bool>` is a special packed representation with proxy references rather than ordinary bool references. Prefer another representation when algorithms require normal reference/address semantics.

## 3. Deque, list, and forward_list

`deque` supports constant-time indexed access and efficient pushes/pops at both ends. It has `push_front`, `emplace_front`, and `pop_front` as well as back operations. Middle insertions and erasures are linear. Its storage is not one guaranteed contiguous array, so there is no vector-like `data()` promise.

`list` is doubly linked. Given an iterator, insertion and erasure are constant time; searching for that position remains linear. It supports member `sort`, `merge`, `splice`, `remove/remove_if`, `unique`, and `reverse`. `splice` transfers nodes without copying their values under its allocator/precondition rules. `merge` expects both lists already sorted under the same ordering.

`forward_list` is singly linked, with `before_begin`, `insert_after`, `emplace_after`, `erase_after`, and `splice_after`. Working from the predecessor matches its topology. It has front access but no back member, random indexing, or constant-time size interface.

```cpp
#include <forward_list>
std::forward_list<int> values{2,3};
values.insert_after(values.before_begin(), 1); // 1,2,3
```

Do not use a linked list solely because insertions exist. If every insertion first searches n nodes, total operation cost is still linear and can have poor locality.

## 4. Ordered associative containers

`set<Key>` stores unique keys; `multiset<Key>` permits equivalent keys. `map<Key,T>` stores unique-key pairs; `multimap<Key,T>` permits equivalent keys. Their iteration follows the comparator, with logarithmic search and ordinary insertion costs. Keys are treated as immutable while stored because changing a key in place could break ordering.

Useful operations are `insert`, `emplace`, `find`, `count`, `lower_bound`, `upper_bound`, `equal_range`, and `erase`. `map` additionally offers `at`, `operator[]`, `try_emplace`, and `insert_or_assign`. The multi variants do not have a unique mapped value to return through `operator[]`.

```cpp
#include <map>
#include <string>
std::map<std::string,int> stock;
auto [it, inserted] = stock.try_emplace("pen", 10);
stock.insert_or_assign("pen", 12);
auto found = stock.find("book"); // Does not insert a missing key.
```

`operator[]` inserts a default-initialized mapped value when absent. `at` throws if absent. `try_emplace` avoids constructing the mapped object inside the container on duplicate key, but function arguments are still evaluated before the call. A costly argument-producing function is not skipped merely because try_emplace is used.

For a multiset, `erase(key)` removes all equivalent keys; `erase(iterator)` removes one. `equal_range` returns the full group as `[first,last)`. C++17 node handles from `extract` permit controlled key changes outside the container and reinsertion, subject to compatibility rules.

## 5. Unordered associative containers

`unordered_set`, `unordered_multiset`, `unordered_map`, and `unordered_multimap` use hashing rather than sorted order. They support corresponding lookup and insertion operations but no ordered bounds. Expected lookup is constant; pathological collisions can make it linear.

`bucket_count`, `bucket_size`, `load_factor`, `max_load_factor`, `rehash`, and `reserve` expose capacity planning. Reserve based on expected element count before bulk insertions when useful. Do not depend on iteration order for reproducible reports; sort the output separately.

A custom equality and hash must agree: equal keys must hash equally. Unequal keys may collide. Hashing a pair should combine both components, while equality compares both. A weak hash can preserve correctness but hurt performance dramatically.

## 6. Adaptors: stack, queue, priority_queue

| Adaptor | Observation | Insert/remove | Typical underlying choice |
|---|---|---|---|
| `stack` | `top` | `push/emplace/pop` | deque by default; vector/list can also fit |
| `queue` | `front/back` | `push/emplace/pop` | deque by default; list can fit |
| `priority_queue` | `top` | `push/emplace/pop` | vector by default |

All offer empty and size. Pop removes but does not return the element: read/copy/move the value first if needed and permitted. Calling top/front/back on an empty adaptor violates the contract.

A priority queue's default comparator yields the largest element. `greater<T>` produces a minimum-first queue. For records, define which item should appear at top and test two different priorities and a tie; comparator direction is a frequent error. Insertion/removal is logarithmic, top inspection constant.

## 7. Companion representations

`string` is taught as an owning character sequence in Module 1. `bitset<N>` stores a fixed number of bits with `set`, `reset`, `flip`, `test`, `count`, `any/all/none`, shifts, and bitwise operations. `valarray` supports numeric elementwise operations and slices; it is specialized numeric machinery, not the ordinary default sequence container.

## 8. Decision table and exercises

| Requirement | Starting choice | Reason |
|---|---|---|
| Append and index | vector | Contiguous ownership and efficient indexing |
| Add/remove both ends and index | deque | End operations plus random access |
| Move known nodes among sequences | list | Stable nodes and splice |
| Sorted unique membership | set | Ordered traversal and bounds |
| Key to value, no order needed | unordered_map | Expected constant lookup |
| Repeated smallest extraction | min priority_queue | Maintains priority without full sorting |

1. Store duplicate event times and remove only one occurrence. **Answer:** multiset; find one iterator then erase that iterator.
2. Need predecessor of a key. **Answer:** ordered set/map and a bound, checking begin before decrementing; a hash table lacks ordered predecessor.
3. Why can a map insertion fail without throwing? **Answer:** A unique equivalent key already exists; inspect the inserted flag.
4. Why is array swap linear? **Answer:** Elements are part of each array object, so it swaps element values rather than exchanging one owned heap pointer.

**Lab:** Implement the same word-frequency report with map and unordered_map, including sorted presentation. Explain expected costs and iterator invalidation before selecting one.
