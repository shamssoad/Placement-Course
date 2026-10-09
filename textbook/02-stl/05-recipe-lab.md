# STL workshop — derive complete library pipelines

[Contents](../../TEXTBOOK.md)

These recipes connect the catalogue to executable work. Each is a local C++17 fragment with named headers. Supply a main function, run the stated example, and then change the input. Do not skip the contracts: a pipeline is correct only when every stage supplies the next stage's assumptions.

## Recipe 1. Filter, transform, and aggregate

Input marks are whole numbers in 0–100. Retain passing marks, add a bonus capped at 100, and total them.

```cpp
#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>
std::vector<int> marks{30,50,95,100}, passing;
std::copy_if(marks.begin(),marks.end(),std::back_inserter(passing),
             [](int x){ return x>=50; });
std::transform(passing.begin(),passing.end(),passing.begin(),
               [](int x){ return std::min(100,x+5); });
long long total=std::accumulate(passing.begin(),passing.end(),0LL);
```

Result is `[55,100,100]`, total 255. Back-insertion constructs output elements. The transform reads and writes each corresponding element in a permitted in-place arrangement. The initial 0LL selects a wide accumulator. Three linear passes remain O(n).

## Recipe 2. Distinct values while preserving first appearance

Sorting plus unique would change order. Use a membership set and a separate result vector:

```cpp
#include <unordered_set>
#include <vector>
std::vector<int> input{3,1,3,2,1}, result;
std::unordered_set<int> seen;
for (int value:input)
    if (seen.insert(value).second) result.push_back(value);
```

Result is `[3,1,2]`. The insertion Boolean says whether this is the first occurrence. Expected time O(n), extra memory O(n). The result's order comes from scanning input, not the unordered set's iteration order.

## Recipe 3. Ordered interval lookup

Store unique event times in a set. Find the first event at or after a requested time and the immediate earlier event.

```cpp
#include <set>
std::set<int> times{10,20,40};
auto next=times.lower_bound(25); // Points to 40.
if (next!=times.begin()) {
    auto previous=next;
    --previous; // Points to 20.
}
```

If next is end and the set is nonempty, decrementing its copy finds the final event. If next is begin, there is no predecessor. Never decrement begin. For duplicate event times, use a multiset and define whether predecessor means previous occurrence or previous distinct time.

## Recipe 4. Two sorted inventories

Combine sorted item IDs, keeping the multiset union's maximum multiplicity:

```cpp
#include <algorithm>
#include <iterator>
#include <vector>
std::vector<int> a{1,1,3}, b{1,2,3,3}, combined;
std::set_union(a.begin(),a.end(),b.begin(),b.end(),std::back_inserter(combined));
```

Output is `[1,1,2,3,3]`. Plain merge would retain the sum of multiplicities instead. Set_intersection uses the smaller multiplicity. Both inputs must use the same ordering; names alone do not validate that precondition.

## Recipe 5. Heap operations are range operations

```cpp
#include <algorithm>
#include <vector>
std::vector<int> heap{3,1,4};
std::make_heap(heap.begin(),heap.end());
heap.push_back(5);
std::push_heap(heap.begin(),heap.end());
std::pop_heap(heap.begin(),heap.end());
int largest=heap.back();
heap.pop_back();
```

Largest is 5. Push_heap assumes the preceding range was already a heap and only the final element is new. Pop_heap places the maximum at the end; pop_back then removes it. Calling pop_back first would discard an arbitrary last heap element instead of the root.

## Recipe 6. Prefixes and differences

```cpp
#include <numeric>
#include <vector>
std::vector<int> a{2,5,9};
std::vector<long long> prefix(a.size());
// Transform to a wide owning sequence so partial_sum accumulates widely.
std::vector<long long> wide(a.begin(),a.end());
std::partial_sum(wide.begin(),wide.end(),prefix.begin());
std::vector<long long> delta(prefix.size());
std::adjacent_difference(prefix.begin(),prefix.end(),delta.begin());
```

Prefixes are `[2,7,16]`; differences recover `[2,5,9]`. Merely making the destination wide does not necessarily widen an algorithm's internal accumulator; read its contract. This example widens the input explicitly.

## Recipe 7. Randomized cross-checking

```cpp
#include <random>
#include <vector>
std::mt19937 engine(12345);
std::uniform_int_distribution<int> length(0,8), value(-5,5);
std::vector<int> sample(length(engine));
for (int& x:sample) x=value(engine);
```

Use these small arrays to compare an optimized subarray algorithm with exhaustive enumeration. Record the seed and failing sample. A fixed seed supports repeatability within the chosen implementation; distribution mapping details should not be treated as a universal cross-library byte-for-byte test oracle.

## Recipe 8. Visiting a closed command set

```cpp
#include <iostream>
#include <type_traits>
#include <variant>
struct Add { int value; };
struct Quit {};
std::variant<Add,Quit> command=Add{7};
std::visit([](const auto& item) {
    using T=std::decay_t<decltype(item)>;
    if constexpr (std::is_same_v<T,Add>) std::cout << item.value << '\n';
    else std::cout << "Quit\n";
},command);
```

The discarded branch need not access a nonexistent member of Quit. For a larger command set, an explicit overload visitor can make exhaustive handling clearer; a generic else could accidentally treat a newly added command as Quit.

## Recipe 9. Bit flags with an explicit domain

```cpp
#include <bitset>
std::bitset<8> permissions;
permissions.set(1);
permissions.set(3);
bool allowed=permissions.test(3);
auto enabled=permissions.count();
permissions.flip(1);
```

Initially two flags are set; after flipping bit 1 only bit 3 remains. Test checks a position and can report an invalid index through its checked contract. Document what every bit means. A bitset cannot by itself represent how many times an action occurred.

## Final lab

Build a report pipeline that parses records, rejects duplicates, filters by a predicate, sorts with deterministic ties, groups totals, and prints output. Include one empty input, one malformed record, one repeated key, and one tied rank. Explain how changing the requirement from sorted output to first-seen output changes the pipeline.
