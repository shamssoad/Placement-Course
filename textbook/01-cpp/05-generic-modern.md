# C++ 1.5 — Templates, deduction, lambdas, and modern language tools

[Previous](04-classes-ownership.md) · [Contents](../../TEXTBOOK.md) · [Next](06-errors-io-application.md)

## 1. Templates describe families of code

A function template expresses one algorithm for types supporting the required operations. It is checked and instantiated for specific types; it does not make every operation meaningful for every type.

```cpp
template<class T>
T smaller(T a, T b) { return b < a ? b : a; }
```

`smaller(3,7)` deduces int. `smaller(3.0,7.0)` deduces double. `smaller(3,7.0)` cannot simply deduce one identical T from conflicting argument types. Decide whether to convert explicitly or define a more general contract. Avoid silently broadening interfaces without understanding mixed arithmetic.

A class template makes a family of types:

```cpp
template<class T>
class Box {
    T value_;
public:
    explicit Box(T value) : value_(std::move(value)) {}
    const T& value() const noexcept { return value_; }
};
```

Include `<utility>`. Each `Box<T>` is a distinct type. Returning a const reference borrows the member; it must not outlive its Box.

## 2. Type and value parameters

A non-type template parameter supplies a compile-time value, as in `std::array<int,5>`. The sizes 5 and 6 produce different array types. A runtime integer cannot ordinarily become such a template argument.

Dependent names sometimes need `typename`, for example `typename Container::value_type`, to tell the parser a nested name is a type. Most everyday generic code can be written without elaborate metaprogramming; begin with readable constraints and compile-time diagnostics.

Specialization provides a different implementation for particular template arguments. Overloading is often simpler for function customization. Do not specialize standard templates unless the standard explicitly permits the particular customization.

## 3. Deduction and value categories

An lvalue identifies an object with a persistent identity for the expression; a prvalue commonly represents a newly computed value; an xvalue is an expiring object expression that can enable resource transfer. These categories affect overload selection and binding, not merely where bytes are stored.

`auto x = reference;` usually makes a value copy. `auto& x = object;` borrows it. `const auto&` borrows read-only. `decltype` follows different rules: `decltype(name)` yields the declared type for an unparenthesized name, whereas `decltype((name))` reflects the expression's value category and commonly gives a reference.

A forwarding reference is typically `T&&` where T is deduced. It can bind both lvalues and rvalues, and `std::forward<T>` preserves the caller's category. In contrast, a concrete `Widget&&` is an rvalue reference, not a universal borrowing mechanism. Use forwarding at generic boundaries; ordinary functions often need a simpler value or reference parameter.

## 4. Lambdas carry behavior

```cpp
#include <algorithm>
#include <vector>
int threshold = 50;
std::vector<int> marks{20,65,80};
int passed = static_cast<int>(std::count_if(marks.begin(), marks.end(),
    [threshold](int mark) { return mark >= threshold; }));
```

The lambda captures its own copy of threshold. Changing the original later does not change that stored copy. `[&threshold]` instead borrows the variable, so lifetime and concurrent access must be considered. Capture only what is needed; broad reference capture can hide dangerous dependencies.

`mutable` allows modifying value-captured members inside the lambda. It does not magically mutate the original variable that was copied. Generic lambda parameters such as `auto x` produce a templated call operator. A lambda with no captures can often convert to a compatible function pointer; a capturing lambda needs an object to carry its state.

`std::function<R(Args...)>` stores compatible callable objects with type erasure. It is convenient when runtime uniformity is needed, but can introduce allocation and indirection. A template parameter avoids that fixed wrapper when compile-time generic code suffices.

## 5. Compile-time branching and parameter packs

`if constexpr` discards the untaken branch during template instantiation, allowing branches that require different type capabilities. Ordinary `if` still requires both branches to be well-formed for the instantiated type.

```cpp
#include <type_traits>
template<class T>
bool isNegative(T value) {
    if constexpr (std::is_unsigned_v<T>) return false;
    else return value < 0;
}
```

This simplified function expects an arithmetic type. A variadic template accepts a pack of arguments; a C++17 fold expression combines them with an operator. Define behavior for an empty pack and mixed types instead of assuming all pack expressions behave identically.

```cpp
template<class... Numbers>
auto sumValues(Numbers... values) { return (0 + ... + values); }
```

The leading zero provides an empty-pack result but also participates in type deduction and arithmetic. This is an illustration, not a universal numerically safe sum.

## 6. Structured bindings and attributes

Structured bindings unpack tuple-like results or suitable aggregates: `auto [position, inserted] = map.emplace(key,value);`. `auto [a,b]` can copy, while `auto& [a,b]` binds references when the source permits it. Know which you intend.

`[[nodiscard]]` asks the compiler to warn when an important returned result is ignored. `[[maybe_unused]]` documents intentional nonuse. Attributes do not replace runtime validation.

## 7. C++20 bridge

Concepts express template requirements using named predicates and `requires`; they improve interface clarity and diagnostics. A constrained template still needs a meaningful semantic contract: a syntactically available `<` does not prove a sensible ordering.

Ranges introduce range-based algorithm interfaces and lazy views. A view pipeline can filter and transform without materializing every intermediate container, but borrowing lifetimes remain important. `std::span` provides a nonowning contiguous view. `std::jthread` provides joining behavior and cooperative stop support; it does not forcibly interrupt arbitrary work. Three-way comparison can simplify consistent comparison definitions when the type's semantics fit.

Coroutines transform suspension/resumption behavior but do not automatically create threads or parallel execution. C++20 modules are a language facility for organizing interfaces; they require toolchain/build support. These advanced facilities are extensions after mastery of the C++17 baseline, not substitutes for understanding lifetime and contracts.

## Exercises and answers

1. Why can `auto item = vector.front()` copy? **Answer:** Plain auto drops the borrowed-reference nature; use `auto&` to alias.
2. Why can a stored `[&local]` lambda dangle? **Answer:** The callback may outlive local's scope.
3. How does `if constexpr` differ from `if` in templates? **Answer:** The discarded constexpr branch need not instantiate invalid type-dependent operations.
4. Does `std::move` guarantee a move? **Answer:** No; overload resolution and available operations decide.
5. When is `std::function` useful? **Answer:** When one runtime slot must hold different compatible callable types and its cost is acceptable.

**Mastery gate:** Write a generic function, a class template, a stateful comparator, and a callback whose ownership remains safe after its creating function returns.
