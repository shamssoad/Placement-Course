# C++ 1.3 — Arrays, strings, pointers, references, and lifetime

[Previous](02-functions-programs.md) · [Contents](../../TEXTBOOK.md) · [Next](04-classes-ownership.md)

## 1. A sequence is storage plus boundaries

An array holds a fixed number of same-typed elements. `int a[3]{10,20,30};` has valid indexes 0,1,2. Index 3 is outside the array even if adjacent memory is accessible. Raw array expressions often decay to pointers, losing size information; this is why a parameter written `int a[]` does not carry the caller's array length.

`std::array<int,3>` preserves fixed size as part of its type and supports library iteration. `std::vector<int>` owns a dynamically sized contiguous sequence. In both, unchecked `operator[]` requires a valid index; `at()` performs bounds checking and throws when invalid.

```cpp
#include <array>
#include <vector>
std::array<int, 3> fixed{10,20,30};
std::vector<int> dynamic{10,20,30};
dynamic.push_back(40);
```

Capacity and size differ. `reserve(100)` prepares storage but does not create 100 elements. `resize(100)` changes the number of elements, initializing new ones. A vector can move its elements into new storage during growth; stored references can become invalid.

## 2. Multidimensional data

A nested vector is a collection of separately owned row vectors. Rows can have different lengths, and their elements do not form one guaranteed contiguous rectangular buffer. For a rectangular matrix, a flat vector with index `row*columns+column` can give contiguous storage. Check multiplication bounds before allocating very large dimensions.

Choose representation from operations. A grid algorithm benefits from clear row/column bounds; a numeric kernel may benefit from predictable contiguous layout. Representation influences constants and memory use even when asymptotic complexity remains the same.

## 3. Strings own character sequences

`std::string` stores character code units and manages its own memory. `size()` counts code units, not necessarily human-perceived characters. For the ASCII exercises here, one char represents one relevant symbol; Unicode processing needs decoding and normalization rules beyond byte indexing.

Common operations include append, `push_back`, `find`, `substr`, `erase`, and comparison. `find` returns `std::string::npos` when absent; check before using the position. Repeated `substr` calls create owning copies. A view avoids copying only when the original storage outlives every use.

```cpp
#include <string>
std::string name = "Amina Soad";
auto space = name.find(' ');
if (space != std::string::npos) {
    std::string first = name.substr(0, space);
}
```

A C-style string is a sequence terminated by a zero character. A pointer alone does not say how much valid storage it reaches or whether a terminator exists. `std::string` can contain embedded zero characters, so its size may differ from what a C string function observes through `c_str()`.

## 4. References and pointers

A reference is an alias initialized to an object; a pointer is a value storing an address and can be reassigned or null. Neither is inherently an owner. `const int* p` prevents modification of the pointed-to int through p; `int* const p` prevents reassigning p while permitting modification of the int.

```cpp
int value = 5;
int other = 9;
const int* readOnly = &value;
readOnly = &other; // Allowed: the pointer is not const.
int* const fixedAddress = &value;
*fixedAddress = 7; // Allowed: the pointed-to int is not const.
```

Pointer arithmetic is meaningful within one array object and its one-past endpoint. The one-past pointer can act as a boundary but cannot be dereferenced. Subtracting unrelated pointers is not a portable way to measure arbitrary addresses.

## 5. Lifetime is the central safety rule

Storage duration concerns how storage is managed; lifetime concerns when a particular object exists. An automatic object normally ends at block exit. A dynamically allocated object survives until its owning mechanism destroys it. Static objects have a different lifetime extending across ordinary function calls.

A dangling pointer refers to an object whose lifetime ended. The bytes may still look correct, which makes occasional successful runs misleading. A null pointer is a recognizable “no object” value, but a dangling pointer need not be null.

```cpp
// Wrong: the reference outlives the local object.
const std::string& broken() {
    std::string local = "temporary";
    return local;
}
// Correct: return an owning value.
std::string safe() { return "owned result"; }
```

The wrong function is a teaching anti-example; do not execute code that reads its result. Returning by value is the natural ownership transfer here.

## 6. Dynamic allocation and RAII

`new T(...)` allocates storage and constructs an object; `delete` destroys and releases it. Arrays allocated by `new T[n]` require `delete[]`. Mixing forms is invalid. Manual allocation easily leaks on early returns or exceptions, so learn its semantics and prefer RAII owners in application code.

```cpp
#include <memory>
#include <utility>
auto owner = std::make_unique<std::string>("hello");
std::string* borrowed = owner.get();
auto nextOwner = std::move(owner);
// nextOwner owns the same object; borrowed still refers to it while it lives.
```

Moving the owner transfers responsibility, not necessarily the underlying object's address. Destroying or resetting the final owner invalidates borrowed pointers. `shared_ptr` extends lifetime while any shared owner remains; cycles of shared ownership can leak. Use `weak_ptr` for an observing relationship that must not keep the object alive.

## 7. Borrowed views

`std::string_view` stores a pointer and length. It is useful for read-only parsing without copying. It does not add a terminator or extend the source lifetime. Removing a prefix from a view adjusts the view; it does not erase source characters.

```cpp
#include <string_view>
bool startsWith(std::string_view text, std::string_view prefix) {
    return text.size() >= prefix.size() && text.substr(0, prefix.size()) == prefix;
}
```

This function uses views only during the call, so passing a temporary owning string for a parameter can be safe during that call. Returning or storing a view into such a temporary is the dangerous step. C++20 `std::span` similarly borrows a contiguous sequence with its extent.

## Worked problem: in-place stable filtering

Remove negative values from a vector while preserving the order of retained values. Keep a write index. Before visiting index i, `[0,write)` contains all retained earlier values. If `a[i]>=0`, write it at `a[write++]`. After the scan, resize to write. Because write never exceeds i, copying forward does not destroy unprocessed values. Time O(n), additional space O(1).

## Exercises with answers

1. Why does a vector's pointer sometimes survive append and sometimes not? **Answer:** Reallocation moves storage; without it, references to existing elements survive an end append, though the past-end iterator changes.
2. Why is `string_view v = std::string("abc");` invalid for later use? **Answer:** The temporary owner dies at the full expression's end.
3. Does a raw pointer require deletion? **Answer:** Only if its contract gives it ownership of a matching allocation; borrowed pointers must not delete their target.
4. Why can nested vectors have unequal row lengths? **Answer:** Each row is an independent vector with its own size.
5. How does a weak observer access an object safely? **Answer:** Lock its weak_ptr to obtain a shared_ptr, then check whether acquisition succeeded.

**Mastery gate:** Draw owner/borrower diagrams for a vector, a unique pointer, a shared cycle, and a string view. Identify exactly which operation can invalidate each borrow.
