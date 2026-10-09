# C++ 1.1 — Programs, values, expressions, and control flow

[Course contents](../../TEXTBOOK.md) · Next: [Functions and programs](02-functions-programs.md)

## 1. A program transforms information

Suppose a student wants the average of three marks. There are three separate questions: how to represent the marks, what operation computes the average, and how to communicate the answer. C++ supplies types for representation, expressions for computation, and library facilities for input and output.

```cpp
// Complete C++17 program.
#include <iostream>
int main() {
    int first = 0, second = 0, third = 0;
    if (!(std::cin >> first >> second >> third)) {
        std::cerr << "Enter three whole-number marks.\n";
        return 1;
    }
    if (first < 0 || first > 100 || second < 0 || second > 100 ||
        third < 0 || third > 100) {
        std::cerr << "Marks must be between 0 and 100.\n";
        return 1;
    }
    double average = (first + second + third) / 3.0;
    std::cout << average << '\n';
}
```

`#include` makes declarations from a header available. `int main()` defines the entry function. Braces delimit a block. A semicolon ends most statements. `std::` qualifies a name in the standard-library namespace. The expression between `if` parentheses is converted to a Boolean condition. `return 1` leaves the function with a failure status. Reaching the end of `main` means successful return.

For input `60 70 80`, the sum is 210 and the average is 70. For `60 70 81`, it is about 70.3333. Dividing by integer `3` would perform integer division before storing the result in a double. The destination type does not retroactively change expression evaluation.

## 2. Object, type, value, and name

An object occupies storage and has a lifetime. Its type specifies permitted operations and interpretation. A variable name identifies an object or reference in a scope. The value can change while the type normally remains fixed.

`int count{0};` declares and initializes an integer. `count = 4;` assigns to an already existing object. Initialization and assignment are different operations, especially for classes. Braced initialization rejects many narrowing conversions: `int n{2.5};` is ill-formed. A plain uninitialized local `int n;` must not be read before assigning a valid value.

| Type family | Use | Important limitation |
|---|---|---|
| `bool` | Logical true/false | Arithmetic conversion is possible but often obscures intent |
| `char` | A character code unit | Plain char's signedness is implementation-defined |
| `int`, `long`, `long long` | Signed whole numbers | Exact widths vary; ranges are finite |
| Unsigned integer types | Bit operations and nonnegative modular arithmetic | Conversion of negatives can surprise |
| `float`, `double`, `long double` | Approximate real-number calculations | Many decimal fractions are not exact |
| `std::size_t` | Object/container sizes | Unsigned; reverse-loop subtraction needs care |

Use `<limits>` and `std::numeric_limits<T>` to inspect a type's range. `<cstdint>` provides fixed-width integer aliases such as `std::int64_t` when supported. Never assume that `long` has identical width on every platform.

## 3. Constants and deduction

`const int days = 7;` makes this object read-only after initialization. `constexpr int hours = 24;` additionally requires a constant-expression initializer. A `constexpr` function can be evaluated at compile time when its arguments and use permit it; it can also run at runtime.

`auto x = 3;` deduces int. `auto y = 3.0;` deduces double. It does not mean dynamically typed. `auto` can improve readability when the initializer makes the type clear, but use explicit types when the width or ownership contract matters.

Prefer a scoped enum for a small set of named choices:

```cpp
enum class Result { Pass, Fail, Absent };
Result status = Result::Pass;
```

Unlike a string status, this cannot accidentally contain a misspelled category. Unlike an unscoped enumeration, it does not silently mix as freely with integer arithmetic.

## 4. Expressions and conversions

Arithmetic uses the operand types after promotions and conversions. `1LL * a * b` performs the multiplication in a wider integer type; `long long answer = a*b` may overflow as int first. Signed overflow is undefined behavior. Unsigned arithmetic wraps modulo one more than the maximum value, but that is useful only when intentional.

`%` computes an integer remainder. For nonnegative values it is convenient for digit extraction and periodic positions. With negative values, a remainder can be negative; normalize a modular residue when the problem requires `[0,m)`.

Comparison operators produce Boolean results. Equality is `==`, assignment is `=`. Logical `&&` and `||` short-circuit: `p != nullptr && *p > 0` does not dereference a null pointer. Bitwise `&` and `|` do not provide that protective evaluation rule.

Parenthesize mixed operations when intent is unclear. Do not write expressions that both modify and depend on the same value in ways whose sequencing you cannot explain. Splitting work into named statements is usually clearer than trying to exploit subtle evaluation rules.

Floating point is approximate. Adding 0.1 ten times need not produce exactly 1.0. For measured quantities, compare with a tolerance appropriate to scale; there is no universally correct epsilon. For money in this course, use integer minor units and explicit rounding rules.

## 5. Branching and loops

An `if/else if/else` chain selects the first true branch. Order matters: checking “mark >= 50” before “mark >= 80” makes the higher grade unreachable if both return immediately. `switch` chooses by integral or enumeration value; `break` exits it, and accidental fallthrough can execute another case. Use `[[fallthrough]]` to document intentional C++17 fallthrough.

A `while` loop checks before running; a `do/while` checks afterward and therefore runs at least once. A `for` groups initialization, condition, and progress. `break` exits the nearest loop; `continue` skips the rest of its current iteration. In a `while`, be careful that continue does not skip the only progress update.

```cpp
// Fragment: product of 1 through n, with 0 <= n <= 20.
unsigned long long product = 1;
for (int factor = 2; factor <= n; ++factor) product *= factor;
```

The invariant is that before processing factor, product equals the product of earlier factors. At n=0 or n=1, there are no iterations and the answer is 1. The bound prevents exceeding the guaranteed useful range of this example's arithmetic.

## 6. Scope, shadowing, and readable state

A local name is visible from its declaration through the enclosing scope. An inner declaration can shadow an outer name. Although legal, using two unrelated variables called `total` in nested scopes makes reasoning harder. Prefer the smallest useful scope and names that express the quantity.

Do not use globals to avoid learning parameter passing. Global mutable state gives every function hidden input and output. A local variable that belongs to one computation is easier to test and reason about.

## Guided exercise: digit sum

Contract: accept an integer n between 0 and 10^9 and return the sum of its decimal digits. Repeatedly take `n%10`, add it, and divide n by ten. For 407: result becomes 7, then 7, then 11. Each step removes one decimal digit, so termination follows. Zero returns zero even with no iterations.

```cpp
int digitSum(int n) { // Preconditions: 0 <= n <= 1,000,000,000.
    int result = 0;
    while (n > 0) {
        result += n % 10;
        n /= 10;
    }
    return result;
}
```

## Exercises with explanations

1. Repair `double ratio = 7/4;`. **Solution:** Make an operand floating point, such as `7.0/4`; result 1.75. Casting after division gives 1.0.
2. A score 90 prints grade C because the first branch is `score>=50`. **Solution:** Check descending thresholds, or use nonoverlapping ranges.
3. Why can an unsigned countdown using `i>=0` fail? **Solution:** Every unsigned value satisfies it, and zero minus one wraps. Use `for (size_t i=n; i>0; --i)` and index `i-1`.
4. Count decimal digits of zero. **Solution:** Define it as one; handle zero explicitly or use a do/while.
5. Explain why a compiler accepting code does not prove correctness. **Solution:** Type/syntax checking cannot establish every input contract or prevent all undefined behavior.

**Mastery gate:** Write a validated mark calculator, digit processor, and multiplication-table program from a blank file. Explain every variable's type and each loop's termination measure.
