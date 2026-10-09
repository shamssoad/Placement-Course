# C++ 1.2 — Functions, interfaces, program structure, and debugging

[Previous](01-language-basics.md) · [Contents](../../TEXTBOOK.md) · [Next](03-memory-text.md)

## 1. Functions name a transformation

A function separates a meaningful operation from its caller. Its interface consists of parameter types, result type, name, and behavioral contract. A declaration tells the compiler the interface; a definition provides the implementation.

```cpp
int absoluteDifference(int a, int b); // Declaration.
int absoluteDifference(int a, int b) { // Definition; inputs limited to +/-10^6.
    return a >= b ? a - b : b - a;
}
```

Document the input limit: subtraction can overflow for arbitrary int endpoints. A function can be correct within its contract without supporting every representable value, but callers must know that contract.

Parameters are local variables initialized from arguments. Passing by value creates a separate value. Passing by reference aliases an existing object. A const reference allows reading through that reference without granting modification through it.

```cpp
#include <vector>
void addOne(int value) { ++value; }       // Does not change caller's integer.
void increment(int& value) { ++value; }   // Changes caller's integer.
long long sum(const std::vector<int>& a) {
    long long result = 0;
    for (int x : a) result += x;
    return result;
}
```

Use value parameters for small independent values, const references for substantial borrowed read-only objects, and mutable references when mutation is the documented purpose. Returning a value is often clearer than modifying an output argument.

## 2. Return paths and overloads

Every reachable path in an ordinary non-void function must produce a result or leave by throwing. Returning a reference to a local object is invalid because the object dies after the call. Returning an owned string or vector by value is normal; copy elision and moves make this practical.

Overloads share a name but differ in parameter lists. Return type alone cannot distinguish overloads. Implicit conversions participate in selection and can create ambiguity, so do not construct a large overload set with overlapping meanings.

Default arguments supply omitted trailing arguments. Put them in a visible declaration and avoid repeating them in the definition. A default is chosen at the call site; changing a header default requires recompiling callers that should use the new value.

## 3. Recursion and the call stack

Each active function call has its own parameters and locals. Recursive factorial asks a smaller instance for its answer, then multiplies. The base case terminates the chain. A recursion trace should show both descent and return:

`factorial(3) -> 3*factorial(2) -> 3*2*factorial(1) -> 6`.

Every recursive algorithm needs a decreasing measure and a bound on depth. A correct recurrence can still exhaust stack space for a very large input. Tail-call elimination is not a portable C++ guarantee. Prefer iteration when the computation is a simple linear accumulation.

## 4. Headers and translation units

A real project rarely fits in one file. Each `.cpp` file, after preprocessing, becomes a translation unit. Put shared declarations in a header and ordinary non-inline definitions in a source file.

```cpp
// math_tools.hpp
#ifndef COURSE_MATH_TOOLS_HPP
#define COURSE_MATH_TOOLS_HPP
namespace course {
    int square(int value); // Contract: -10000 <= value <= 10000.
}
#endif
```

```cpp
// math_tools.cpp
#include "math_tools.hpp"
int course::square(int value) { return value * value; }
```

```cpp
// main.cpp
#include <iostream>
#include "math_tools.hpp"
int main() { std::cout << course::square(12) << '\n'; }
```

Build both source files in one compiler command. Forgetting `math_tools.cpp` commonly causes an unresolved-symbol linker error. Defining the same ordinary function in several translation units violates the one-definition rule. Header guards prevent repeated inclusion within a translation unit; they do not make multiple non-inline definitions across translation units valid.

Templates usually need their definitions visible where instantiated, so their definitions normally live in headers. `inline` permits the required form of multiple definitions across translation units; it does not command the optimizer to substitute a function body at every call.

Namespaces group related names and avoid collisions. Avoid `using namespace std;` in headers because it changes the lookup environment of every including file. The preprocessor performs textual operations before normal compilation; prefer typed constants and functions over macros for ordinary calculations.

## 5. Interfaces should describe failure

Consider a maximum function. Empty input has no maximum. Possible contracts are “input must be nonempty,” “return optional,” or “throw for empty input.” Silently returning zero confuses absence with a real answer and breaks all-negative inputs.

```cpp
#include <optional>
#include <vector>
std::optional<int> maximum(const std::vector<int>& a) {
    if (a.empty()) return std::nullopt;
    int result = a.front();
    for (int x : a) if (x > result) result = x;
    return result;
}
```

The caller explicitly checks `if (result)` and accesses `*result` only when present. A function should not both print an error and force every caller to guess whether its return value is valid.

## 6. Debug systematically

Classify a failure first: compilation, linking, exception, crash, incorrect result, or poor performance. Reduce the input until the failure is small. State expected and actual behavior. Place a breakpoint before the first divergence, not just before the final wrong print.

Inspect variables and call frames. Step into a function when the bug may be inside it; step over when its contract is already established. A watch expression can expose an unexpected mutation. Be aware that optimized builds may reorganize variables and execution, so initial debugging is easier in a debug build.

Warnings often detect unused values, suspicious conversions, and unreachable-looking choices. Address warnings by understanding them. Sanitizers can detect some out-of-bounds accesses, lifetime errors, and undefined operations. They only examine executed paths.

## 7. Testing from contracts

For a maximum function, useful tests are empty, one element, all negative, duplicate maximum, and maximum at either boundary. These are different behaviors, not five random examples. For a sorting function, check both order and preservation of the input multiset; returning an empty sorted vector satisfies only the first property.

Compare optimized algorithms with simple baselines on small inputs. A baseline that clearly enumerates all candidates is often easier to trust than another optimized implementation. Use fixed seeds for repeatable generated tests.

## Exercises with answers

1. Why does `void f(std::vector<int> a)` fail to mutate the caller's vector? **Answer:** It mutates a copy. Use a mutable reference if mutation is intended.
2. Is a header guard enough to define a normal function in a shared header? **Answer:** No; it prevents repetition within one translation unit, not across several.
3. Why separate calculation from printing? **Answer:** The calculation becomes independently testable and reusable with other interfaces.
4. What is the first step when a 1000-element input fails? **Answer:** Preserve the failure while reducing the input, then locate the first incorrect state.
5. Write a function returning both minimum and maximum. **Answer:** Use a result struct or pair, with optional absence for empty input; traverse once.

**Mastery gate:** Build a three-file program, intentionally create a linker error and repair it, and write a contract-driven test set for one function.
