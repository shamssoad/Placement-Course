# Module 1 — C++ foundations

**Expanded textbook:** Start with [C++ language basics](textbook/01-cpp/01-language-basics.md), then follow the six linked chapters. This file is the introductory/revision version.

Prerequisite: none. Outcome: you can turn a small requirement into a working program and explain how its data lives.

## Lesson 1.1 — What happens when you run a program?

A source file is text. The compiler translates it into object code; the linker connects that code to referenced functions; the operating system loads the executable. A syntax error stops compilation. A declaration with no matching definition can produce a link error. Reading an invalid address may compile successfully but fail at runtime. These are different failure stages, so first identify the stage before changing code.

Start with this complete program:

```cpp
#include <iostream>
int main() {
    int count = 0;
    std::cout << "How many items? ";
    if (!(std::cin >> count) || count < 0) {
        std::cerr << "Expected a nonnegative integer\n";
        return 1;
    }
    std::cout << "Accepted " << count << " items\n";
}
```

`main` is the entry point. The stream extraction attempts a conversion and sets a failure state when it cannot read an integer. Check that state before using the value. Returning a nonzero status signals failure to the surrounding environment.

**Lab:** Compile the program, enter `3`, `-1`, and `hello`, and predict each result before running it. Remove a semicolon, then misspell `std::cout`. Read the first compiler error before the later errors, which may be consequences.

## Lesson 1.2 — Values, types, and arithmetic

A type determines valid operations and representation. Integer division discards the fractional part: `7 / 2` is `3`, while `7 / 2.0` is `3.5`. Assigning the first expression to a `double` does not restore the lost fraction because division happened first.

Choose a type from the largest intermediate result, not merely the input. If two `int` values are multiplied, the multiplication occurs as `int` even if the answer is assigned to `long long`. Use `1LL * a * b` when the product must be computed as `long long`. That type still has a finite range. Signed overflow is undefined behavior, so overflow is not a reliable wraparound strategy.

Unsigned values cannot represent negative numbers. Mixing signed and unsigned comparisons can convert a negative operand into a large unsigned value. Container sizes use an unsigned type; handle reverse loops carefully.

`const` communicates that a particular object should not be modified. Initialize variables before reading them. For equality, use `==`; assignment uses `=`. The condition `x = 5` assigns and then tests the assigned value.

**Exercise:** Explain why `double x = 5 / 2;` produces `2`. Repair a calculation of `100000 * 100000`.

**Answer:** Use `5.0 / 2` for floating division. Compute the product with `1LL * 100000 * 100000`, assuming the target range is sufficient.

## Lesson 1.3 — Loops as invariants

A useful loop has a statement that remains true before every iteration. To sum the first `n` values, maintain: “Before iteration `i`, `sum` equals the total of indexes `[0, i)`.” Initially the range is empty and `sum` is zero. Adding element `i` makes the statement true for `i + 1`. When `i == n`, the whole array has been processed.

```cpp
#include <vector>
long long total(const std::vector<int>& values) {
    long long sum = 0;
    for (int x : values) sum += x;
    return sum;
}
```

This takes O(n) time and O(1) auxiliary space. The input vector is not copied because it is passed by const reference. The result assumes the total fits in `long long`.

An off-by-one error often means the invariant and boundary disagree. For `n` elements, valid indexes are `0` through `n - 1`; `i <= n` visits one invalid index. Always test `n = 0` and `n = 1`.

**Lab:** Implement the maximum of a nonempty vector. Decide whether an empty vector should throw, return an optional value, or violate a documented precondition. Do not silently return zero: all valid elements might be negative.

## Lesson 1.4 — Functions and contracts

A function should state what it accepts, what it produces, and what it may change. Pass small values by value. Pass large read-only objects by const reference. Use a nonconst reference when mutation is part of the contract.

```cpp
void swapValues(int& a, int& b) {
    int temporary = a;
    a = b;
    b = temporary;
}
```

References here refer to the caller's objects. If the parameters were `int a, int b`, only local copies would change. A function returning a reference to a local variable is invalid because the local object dies when the function returns.

Separate calculation from user interaction. A function that computes a balance should return the balance; the caller decides how to print it. This makes it reusable and testable.

**Exercise:** Write `bool isPalindrome(const std::string&)` with no console input/output. Test an empty string, one character, and mixed case. State whether case matters.

## Lesson 1.5 — Objects, addresses, and lifetime

A pointer stores an address; dereferencing accesses the object at that address. A reference is an alias established during initialization. Neither automatically proves that the referred object remains alive.

```cpp
int value = 7;
int* p = &value;
*p = 9;           // value is now 9
int& alias = value;
alias = 11;       // value is now 11
```

Automatic objects normally die at the end of their scope. Dynamic objects can outlive the scope that allocated them, so someone must own their cleanup. Prefer containers and RAII wrappers instead of scattering `new` and `delete`.

```cpp
#include <memory>
auto owner = std::make_unique<int>(42);
int* borrowed = owner.get();
// borrowed is valid only while owner still owns this object.
```

RAII means the lifetime of an owning object controls a resource. The resource could be memory, a file, or a lock. A smart pointer does not make all borrowing safe: storing `borrowed` after the owner is destroyed still creates a dangling pointer.

## Lesson 1.6 — Files and failure

```cpp
#include <fstream>
#include <stdexcept>
#include <string>
void appendRecord(const std::string& path, const std::string& record) {
    std::ofstream out(path, std::ios::app);
    if (!out) throw std::runtime_error("Cannot open output file");
    out << record << '\n';
    out.flush();
    if (!out) throw std::runtime_error("Write failed");
}
```

Opening a file can fail, and a write can fail after opening succeeds. The stream closes when its owning object leaves scope, including during exception unwinding. This simple example is not a transactional storage system; a crash could still leave an incomplete record.

An assertion checks a programmer's internal assumption. Invalid user input is expected behavior and needs ordinary handling; do not rely on assertions, which may be disabled.

## Exercise set and answers

1. Count positive, negative, and zero values in a vector. **Answer:** One pass, three counters, mutually exclusive branches; O(n) time.
2. Reverse digits of a nonnegative integer. **Answer:** Repeatedly take `% 10`, append to the result, and divide by 10. Check the result range before `result * 10 + digit`.
3. Return the largest of three numbers. **Answer:** Start with the first number and compare the other two. This also handles all-negative values.
4. Explain a dangling reference. **Answer:** It refers to an object whose lifetime has ended; using it is invalid even if the old bytes remain.
5. Why is `vector` preferable to a manually allocated array here? **Answer:** It stores size and owns cleanup, including on exceptional exits.

## Module lab

Build an expense tracker with add, list, total, and save commands. Store monetary values as integer minor units, not binary floating point. Reject negative amounts and malformed input. Keep parsing, calculation, and persistence in separate functions. The project chapter contains acceptance criteria.

**Exit ticket:** Explain compilation versus linking; implement a function without copying its vector; identify an out-of-bounds access; explain who owns each resource in your tracker.
