# C++ 1.4 — Classes, value semantics, ownership, and polymorphism

[Previous](03-memory-text.md) · [Contents](../../TEXTBOOK.md) · [Next](05-generic-modern.md)

## 1. A class defines valid states and operations

A struct and a class can both have fields, functions, constructors, and access controls. Their default access differs: struct is public, class private. Use a struct for a simple aggregate of data and a class when operations protect an invariant.

Consider a nonnegative counter capped at a chosen limit. Every operation must preserve `0 <= value <= limit`. Keeping the fields private prevents callers from bypassing that rule.

```cpp
#include <stdexcept>
class BoundedCounter {
    int value_;
    int limit_;
public:
    explicit BoundedCounter(int limit) : value_(0), limit_(limit) {
        if (limit < 0) throw std::invalid_argument("Negative limit");
    }
    bool increment() {
        if (value_ == limit_) return false;
        ++value_;
        return true;
    }
    int value() const noexcept { return value_; }
};
```

`explicit` prevents an integer from being silently treated as a counter through an implicit conversion. The member initialization list constructs members directly. Members initialize in declaration order, not the order written in that list. A `const` member function can be called on a const object and cannot modify ordinary members through `this`.

## 2. Construction and destruction

A constructor establishes an object's invariant. If construction throws, already constructed members are destroyed, but the destructor of the incompletely constructed enclosing object is not called. Owning resources in RAII members therefore matters even inside constructors.

A destructor releases resources; it should ordinarily not allow exceptions to escape. If a destructor throws while another exception is already unwinding, termination can result. Report recoverable operational failures through explicit operations such as `save`, rather than relying on a destructor to communicate them.

Static data members belong to the class rather than each instance. A static member function has no `this` pointer. Use static state only when it truly represents shared state; it is not a substitute for passing dependencies.

## 3. Copy and move express object meaning

Copy construction creates a new object from an existing one. Copy assignment changes an existing object's value. Move operations allow resources to be transferred from an expiring or explicitly moved source.

A class containing only strings and vectors can usually use the rule of zero: their own special members already manage resources. A class owning a raw allocation needs deliberate special-member design, because a default copy duplicates the pointer, not the allocation.

```cpp
#include <string>
#include <vector>
struct Report {
    std::string title;
    std::vector<int> marks;
}; // Compiler-generated copy/move/destruction are appropriate.
```

`std::move(x)` does not move by itself; it enables selection of move-aware operations. Moving a const object often still copies because a typical move constructor needs permission to modify its source. After moving, use only operations supported by the moved-from object's contract, such as destruction or assignment; do not assume its old content remains.

For a resource owner, either provide the necessary copy/move/destructor behavior, or explicitly delete unsupported operations. “Rule of five” means considering these operations together, not that every class should manually implement all five.

## 4. Operator overloads should preserve expectations

An overloaded operator is a function with special syntax. It cannot change precedence or invent a new operator. Use it when the operation naturally behaves like the corresponding built-in concept.

```cpp
struct Point {
    int x;
    int y;
};
bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}
Point operator+(const Point& a, const Point& b) {
    return {a.x + b.x, a.y + b.y}; // Inputs must keep sums in range.
}
```

Equality should behave as an equivalence relation. A sorting comparator requires a strict weak ordering, a different contract. Do not overload `+` to unexpectedly save to a file or mutate unrelated objects; surprising semantics make code difficult to reason about.

## 5. Composition and inheritance

Composition stores or uses another object. Inheritance promises that a derived object can be used where the base abstraction is expected. Prefer composition for “uses a” relationships, and inheritance when runtime substitutability is a real requirement.

```cpp
#include <memory>
#include <stdexcept>
#include <vector>
struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;
};
class Rectangle final : public Shape {
    double width_, height_;
public:
    Rectangle(double w, double h) : width_(w), height_(h) {
        if (!(w >= 0 && h >= 0)) throw std::invalid_argument("Invalid sides");
    }
    double area() const override { return width_ * height_; }
};
```

Values must be finite and within a useful numeric range if the application requires a finite result. A pure virtual function makes Shape abstract. `override` lets the compiler check that the function really overrides a base function. A virtual destructor permits deletion through the base interface. `final` prevents further derivation here.

Storing derived objects by base value slices away derived state. Store appropriate owning pointers or use borrowed references for polymorphic calls. Virtual calls made during construction/destruction do not dispatch as if the fully derived object were already or still alive; avoid relying on such dispatch to initialize derived behavior.

## 6. Exception guarantees and ownership

The basic guarantee preserves valid state and prevents leaks after failure. The strong guarantee leaves observable state unchanged if an operation fails. The no-throw guarantee promises the operation cannot throw.

To update a complex object strongly, build a valid replacement first and commit with a nonthrowing swap where the types permit it. Do not mutate half the fields and then perform an allocation that might fail unless rollback is designed. Standard container guarantees depend on operations and element capabilities; “uses vector” does not automatically make every user-defined operation strongly exception-safe.

## Exercises and answers

1. Why initialize members in the list? **Answer:** They must be constructed before the body; some, including references and const members, require direct initialization.
2. What does default copying of an owning raw pointer do? **Answer:** Copies the address, risking shared accidental ownership and double deletion.
3. Why use virtual destruction for Shape? **Answer:** An owner typed as Shape must destroy the complete derived object.
4. When is inheritance inappropriate? **Answer:** When the relationship is implementation reuse without substitutability; composition often expresses that better.
5. Implement a read-only `Report::average`. **Answer:** Use a const member, handle empty marks explicitly, and compute the sum in a suitable wider type before floating division.

**Mastery gate:** Implement a class with an invariant, a move-only owner, and two interchangeable strategy implementations. Explain each copy, borrow, transfer, and destruction.
