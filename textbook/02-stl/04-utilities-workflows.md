# STL 2.4 — Supporting utilities and complete workflows

[Previous](03-algorithms.md) · [Contents](../../TEXTBOOK.md) · [Next volume](../03-analysis.md)

## 1. Pair, tuple, and meaningful result types

`pair<A,B>` holds two potentially different types; `tuple<Ts...>` generalizes to several. Use `get`, `tie`, structured bindings, or `apply` to access/invoke. `tie` binds references, so assigning to a tied tuple writes existing variables. A named struct is often clearer than tuple positions when fields have domain meaning.

Comparison of pairs/tuples is lexicographic when their element comparisons support it: first differing field determines the result. This makes `(distance,vertex)` useful in heaps, but also imposes a tie-breaker that may or may not match your intended priority.

## 2. Optional, variant, and any

`optional<T>` represents a value that may be absent. `has_value` or Boolean conversion checks presence; `value()` throws on absence, while unchecked dereference requires presence. `value_or` supplies a fallback, but its argument is evaluated as part of the call even if the optional already has a value.

`variant<A,B,...>` represents exactly one of a known set of alternatives, with a possible exceptional valueless state in some operations. Use `visit` for type-directed behavior or `get_if` for a checked pointer. It is useful for a parsed command with a finite set of forms.

`any` stores a copy-constructible value whose type is erased. Recovering it requires a compatible `any_cast`. Prefer variant when the possible types are known, because exhaustive handling is clearer.

```cpp
#include <string>
#include <variant>
struct Add { int amount; };
struct Quit {};
using Command = std::variant<Add, Quit>;
```

This representation prevents a Quit command from accidentally carrying an unrelated numeric field. The parser returns a valid command or a separate parse error.

## 3. Ownership and allocation tools

`unique_ptr` owns one object or an array specialization and can use a custom deleter for an external resource. `make_unique` creates the common form safely. `shared_ptr` maintains a control block and shared lifetime; `make_shared` can combine allocations. `weak_ptr` observes that control block without extending the managed object's lifetime; `lock` attempts to acquire a shared owner.

Shared ownership is not object synchronization. Two threads can own the same object safely while racing on its fields. Similarly, two independently created shared_ptr objects from the same raw owning pointer are not one shared ownership group and can double-delete.

Allocators determine how containers obtain storage. Most students should use defaults until measurement establishes a need. C++17 polymorphic allocators and memory resources can direct allocations through a runtime-selected resource. A monotonic resource releases groups of allocations together; it does not make individual object lifetimes irrelevant. The resource must outlive containers using it.

## 4. Time and random numbers

Chrono separates durations from time points. A duration has a count and period; a time point belongs to a clock. Use steady_clock for elapsed-time measurement because it is intended to advance monotonically. Use system_clock for wall-clock timestamps, understanding that clock adjustments can occur. Convert duration units explicitly.

```cpp
#include <chrono>
auto start = std::chrono::steady_clock::now();
// perform work
auto elapsed = std::chrono::steady_clock::now() - start;
auto micros = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
```

Random facilities separate engines from distributions. A seeded `mt19937` generates a repeatable pseudorandom sequence; a uniform_int_distribution maps it to an integer range. Repeatedly using `% range` can introduce bias. Use a fixed seed for reproducible tests; these ordinary engines are not cryptographic generators.

## 5. More standard-library facilities

`numeric_limits` describes type properties; `type_traits` supports compile-time questions; `ratio` represents compile-time rational values; `complex` supports complex arithmetic. Mathematical functions such as sqrt and log have domain and floating-point considerations. Regex can express pattern matching, but regex syntax and potentially expensive matches should be tested; simple parsing often needs only direct scanning.

Filesystem handles paths and directory operations; streams handle formatted or byte-oriented transfer. Locale can influence character classification and formatting. Character classification functions from `<cctype>` require EOF or an unsigned-char-representable argument, so cast an arbitrary char to unsigned char before calling them. The concurrency library belongs to Volume 11, where mutexes, condition variables, threads, futures, and atomics are taught with their memory model.

## 6. Complete workflow: student ranking report

Requirements: accept valid `(id,name,score)` records; reject duplicate IDs; rank by score descending, then ID ascending; show each student's rank and the number above a threshold.

Use a vector as the owning dataset, a set or unordered_set to validate uniqueness during parsing, sort with a strict comparator, and count_if for the threshold. Keep names owned as strings. Structured bindings can inspect map insertion outcomes. If parsing fails, discard the temporary dataset so the old one remains valid.

Complexity: with hashing, uniqueness checks are expected O(n), sorting O(n log n), threshold counting O(n), storage O(n) plus sorting's implementation requirements. If sorted order is already available and many threshold queries are needed, use a correctly ordered boundary search instead of repeated scans.

The same problem teaches composition of library tools. No single container “solves” the whole workflow; contracts must align between stages.

## 7. Module assessment with answers

1. Optional or sentinel -1 for an arbitrary integer result? **Answer:** Optional avoids excluding a valid integer value or overloading its meaning.
2. Variant or any for three known command types? **Answer:** Variant permits explicit handling of the closed set.
3. Why is steady_clock preferable for a benchmark interval? **Answer:** Wall-clock adjustments should not alter elapsed-time reasoning.
4. Does a shared_ptr protect a mutable vector from races? **Answer:** No; it manages lifetime, not the vector's concurrent operations.
5. Why seed a test generator predictably? **Answer:** A failing generated input can be reproduced and reduced.
6. Why not preserve unordered-map iteration order in a golden report? **Answer:** The contract does not provide the needed presentation order; sort it explicitly.

**Mastery gate:** Implement the ranking workflow and explain every container, algorithm, borrow, complexity bound, and failure transition. Then solve the standard-library examination in [Mastery](../MASTERY.md).
