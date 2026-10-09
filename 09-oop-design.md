# Module 9 — OOP and low-level design

**Expanded textbook:** [Design and worked case studies](textbook/09-design.md). This file is the introductory/revision version.

Prerequisite: C++ foundations and basic data structures. Outcome: turn changing requirements into testable designs.

## Lesson 9.1 — A class protects an invariant

Encapsulation is more than making fields private. A class should prevent its state from becoming invalid. For a wallet measured in minor currency units, the balance must remain nonnegative. If callers can assign its balance directly, every caller must preserve that rule. Instead, expose operations that check it.

```cpp
#include <limits>
#include <stdexcept>
class Wallet {
    long long balance_ = 0;
public:
    long long balance() const noexcept { return balance_; }
    void deposit(long long amount) {
        if (amount <= 0 || amount > std::numeric_limits<long long>::max() - balance_)
            throw std::invalid_argument("Invalid deposit");
        balance_ += amount;
    }
    bool withdraw(long long amount) {
        if (amount <= 0) throw std::invalid_argument("Invalid withdrawal");
        if (amount > balance_) return false;
        balance_ -= amount;
        return true;
    }
};
```

An insufficient balance is an ordinary declined operation here, while a nonpositive amount is invalid input. The contract distinguishes the cases. A production financial system needs much more than this teaching class: here the goal is invariant preservation and clear behavior.

Constructors establish valid state. Destructors release owned resources. Member initialization lists initialize fields directly; assigning in the constructor body happens after initialization. Destruction occurs in reverse construction order.

## Lesson 9.2 — Copying, moving, and resource ownership

Prefer the rule of zero: compose standard containers and RAII owners so compiler-generated special members do the right thing. A manually owning raw pointer requires deliberate copy, move, assignment, and destruction behavior. Default copying merely copies the address and can cause double deletion.

An owning `unique_ptr` makes a class noncopyable by default but movable when its other members permit that. If copying is a requirement, define what duplication means. Should two objects refer to one resource or own independent copies? This is a domain decision, not only a syntax choice.

Mark read-only member functions `const`. A const member function promises not to mutate ordinary object state, but does not automatically make unrelated objects or pointed-to mutable data immutable.

**Lab:** Implement a move-only file-owner wrapper, or explain why `std::fstream` already meets your needs. Do not add a custom resource wrapper simply to demonstrate more code.

## Lesson 9.3 — Composition and polymorphism

Inheritance models substitutability: clients of the base interface must be able to use the derived object without broken promises. Composition models “has a” or “uses a.” A booking service uses a pricing policy; it need not inherit from that policy.

```cpp
#include <memory>
#include <stdexcept>
#include <utility>
struct PricingPolicy {
    virtual ~PricingPolicy() = default;
    virtual long long price(int minutes) const = 0;
};
struct FlatRate final : PricingPolicy {
    long long price(int minutes) const override {
        if (minutes < 0) throw std::invalid_argument("Negative duration");
        return minutes == 0 ? 0 : 500;
    }
};
class ParkingService {
    std::unique_ptr<PricingPolicy> policy_;
public:
    explicit ParkingService(std::unique_ptr<PricingPolicy> policy)
        : policy_(std::move(policy)) {
        if (!policy_) throw std::invalid_argument("Missing policy");
    }
    long long quote(int minutes) const { return policy_->price(minutes); }
};
```

The virtual destructor allows correct derived-object destruction through the base owner. Passing a derived object by base value can slice off derived state; use references or appropriately owned pointers for runtime polymorphism.

## Lesson 9.4 — SOLID through a change request

Suppose one class handles pricing, database queries, and email delivery. A pricing change risks unrelated behavior, tests need all external dependencies, and failures are hard to isolate.

- **Single responsibility:** Separate parts that change for different reasons, such as pricing rules and persistence.
- **Open/closed:** Add a new policy through an existing contract when that variation is expected; do not build extension points for every imagined future.
- **Liskov substitution:** A derived implementation must honor the base contract, including accepted inputs and behavioral promises.
- **Interface segregation:** A read-only client should not depend on unrelated administrative operations.
- **Dependency inversion:** Business rules depend on an abstraction such as a repository, with concrete persistence provided from outside.

Dependency injection can simply be a constructor parameter. It need not require a framework. A fake repository can then simulate “record absent,” “save failed,” or “duplicate reservation” in focused tests.

## Lesson 9.5 — Patterns as solutions, not decorations

**Strategy** swaps an algorithm, as the pricing example does. **Factory** centralizes selection and construction when callers should not know concrete types. **Builder** makes complex construction explicit and validates required settings before producing the object.

**Adapter** translates an existing interface into the one your application expects. **Facade** exposes a simpler operation over several collaborators. **Observer** publishes events to subscribers; specify subscriber lifetime and whether notifications happen synchronously. A callback borrowing a destroyed object is a lifetime bug.

A singleton introduces global access and often hidden coupling. Thread-safe initialization does not automatically make all singleton operations thread-safe. Prefer explicit ownership when it makes dependencies clearer.

**Exercise:** An application supports hourly and flat parking fees. **Answer:** Strategy is useful if policies need independent replacement; a simple function or variant can be enough for a tiny fixed set.

## Lesson 9.6 — Design from a state machine

For ticket booking, identify states before classes:

`Available -> Held -> Confirmed`

`Held -> Available` on expiry or cancellation.

A payment result arriving after expiry must not confirm a seat already sold elsewhere. Represent the reservation identity and current state explicitly. Avoid a collection of unrelated Booleans such as `paid`, `reserved`, and `expired`, which allow impossible combinations.

Ask: Who owns the seat? Which operation changes availability atomically? Is a repeated confirmation safe? What happens if persistence fails? Draw the success sequence and one failure sequence. The database chapter explains the transactional part.

For an elevator, separate movement state from request scheduling. For expense sharing, define integer rounding and allocation rules so distributed shares add up exactly. For a parking lot, separate spot compatibility, allocation, and pricing.

## Lesson 9.7 — LRU as a concrete design exercise

An LRU cache combines a doubly linked ordering with a key-to-node hash map. Reading an item moves it to the most-recent end. Inserting a new item at capacity removes the least-recent end and its map entry. A list iterator allows O(1) relocation with `splice`; expected lookup is O(1).

Important contracts: capacity zero, absent key, replacement of an existing key, and iterator validity. Copying a cache whose map stores list iterators requires rebuilding those iterators; default memberwise copy would point into the original list. This illustrates how individually safe members can still require a class-level ownership invariant.

## Exercises and answers

1. Why should the policy base have a virtual destructor? **Answer:** It is deleted through a base pointer.
2. Who owns the policy in the example? **Answer:** `ParkingService`, through sole ownership; callers transfer it at construction.
3. Test a failed booking write without a real database. **Answer:** Inject a fake repository that returns or throws the specified failure.
4. Is one interface per class required by SOLID? **Answer:** No. Introduce boundaries where useful variation or isolation exists.
5. Why model expiry as a transition? **Answer:** The transition defines legal behavior and prevents contradictory state combinations.

**Exit ticket:** Submit a booking state diagram, class responsibilities, one complete sequence, one failure sequence, and tests demonstrating invariants. Implement the project incrementally.
