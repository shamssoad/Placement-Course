# Volume 9 — Object-oriented design and complete case studies

[Contents](../TEXTBOOK.md) · [Introductory module](../09-oop-design.md)

## Chapter 1. Requirements become observable contracts

Start with actions and outcomes rather than class names. For parking: enter a compatible vehicle, allocate a spot, issue a ticket, calculate a charge, exit, and reject impossible operations. Ask whether multiple gates act concurrently, whether pricing changes over time, and whether a ticket can be reused.

Separate functional behavior from quality constraints. “Allocate a spot” is functional. “Allocation remains correct under two simultaneous entries” is a concurrency requirement. “Return within a target latency under a measured workload” is a performance requirement. A diagram alone proves none of these.

Write success and failure examples. If a car arrives when only motorcycle spots remain, what result appears? If charge calculation fails, is the spot released? The contract determines the order of state changes.

## Chapter 2. Responsibilities and dependencies

An entity represents identity and state; a value object represents a value such as Money or SeatId; a service coordinates operations; a repository abstracts persistence when that boundary is useful. These are roles, not mandatory suffixes for every class.

Keep invariants near their owning state. A reservation should not expose public setters that allow Confirmed with no seat or expiry earlier than creation. A service can orchestrate multiple entities, but a database transaction is still needed when persistence must change atomically.

Use constructor injection for required collaborators. A reference expresses a borrowed collaborator that must outlive the service; unique_ptr expresses owned sole lifetime. Choosing an interface without deciding ownership leaves the design incomplete.

## Chapter 3. Patterns through specific changes

When pricing varies, Strategy isolates the calculation. When construction depends on configuration, Factory selects and builds a valid concrete implementation. Builder helps when many optional settings make construction unreadable, but must reject invalid combinations before producing the object.

Adapter translates an external payment API into an internal contract. Facade offers a simpler entry operation that coordinates reservation, pricing, and persistence. Observer publishes booking events, but its contract must state whether notifications are synchronous, whether subscriber failure rolls back booking, and how subscriber lifetime is managed.

State can express behavior that changes with lifecycle. Do not create one subclass per state merely because a pattern name exists; a scoped enum and a central transition function may be clearer for a small state machine. Apply patterns where they reduce a concrete source of complexity.

## Chapter 4. Case study: a seat reservation lifecycle

States: Available, Held, Confirmed. A hold includes reservation ID, owner ID, and expiry. At the exact expiry instant it is expired. Confirming requires matching identity, valid ownership, and time strictly before expiry. Cancellation and expiry release only the allocation belonging to that reservation.

Successful sequence:

1. Client submits hold request with a stable request ID.
2. Service reads time from an injected clock.
3. Repository atomically attempts active allocation under a uniqueness constraint.
4. On success it stores the reservation and returns its ID/expiry.
5. Confirmation checks state and identity inside the appropriate transaction.
6. Commit precedes the success response.

Failure sequence: commit succeeds but the response is lost. The retry uses the same request ID and returns the stored outcome. A fresh request ID might represent a distinct operation, so the server cannot assume every similar payload is a duplicate.

An expiry worker must not release a newer hold after reading an older expired one. Its deletion/update should include reservation identity or version, not merely seat ID. This is an example of a race that a visually correct state diagram alone misses.

## Chapter 5. A transition function you can test

```cpp
#include <string>
enum class ReservationState { Held, Confirmed, Cancelled, Expired };
enum class ConfirmResult { Confirmed, AlreadyConfirmed, Expired, WrongOwner, InvalidState };
struct Reservation {
    std::string owner;
    ReservationState state;
    long long expiresAt;
};
ConfirmResult confirm(Reservation& r, const std::string& owner, long long now) {
    if (r.owner != owner) return ConfirmResult::WrongOwner;
    if (r.state == ReservationState::Confirmed) return ConfirmResult::AlreadyConfirmed;
    if (r.state != ReservationState::Held) return ConfirmResult::InvalidState;
    if (now >= r.expiresAt) {
        r.state = ReservationState::Expired;
        return ConfirmResult::Expired;
    }
    r.state = ReservationState::Confirmed;
    return ConfirmResult::Confirmed;
}
```

This local function teaches transition logic. It does not by itself provide thread safety, persistent atomicity, payment reconciliation, or full request-level idempotency. The surrounding service/repository must supply those contracts. Tests can use explicit integer times instead of sleeping.

## Chapter 6. Case study: elevator simulation

Separate physical motion from request selection. Motion knows floor, direction, and door state. A scheduler chooses the next stop based on requests. Requests from inside the car and hall requests may have different semantics. Define whether new requests can change a target mid-trip.

Use discrete ticks or an injected clock for a deterministic simulation. A transition like OpeningDoor must not coexist with moving between floors. Validate floor bounds. A policy can serve requests in the current direction before reversing, but starvation and fairness require analysis under continuing arrivals.

Test a trace with simultaneous requests, an empty queue, a request for the current floor, and invalid floors. Assert state invariants after every tick, not just the final destination.

## Chapter 7. Case study: expense sharing

Store amounts in integer minor units. For equal sharing, divide by participant count and distribute remainder units in a documented deterministic order. Splitting 100 among three yields 34,33,33, preserving total exactly. Weighted sharing requires explicit rounding and remainder allocation.

Track each user's paid amount minus owed amount. The sum of net balances must be zero. A greedy settlement matching debtors and creditors produces a valid settlement, but do not automatically claim the minimum possible number of transactions under every formulation; that is a separate optimization problem.

## Chapter 8. Design review and testing

Review invariants, ownership, dependency direction, failure behavior, and concurrency boundaries. Unit-test transition rules; integration-test database constraints and transactions; use deterministic fakes for clocks and failure injection. A mock should represent a collaborator's contract, not force the exact internal call sequence unless that sequence is itself required.

The LRU lab demonstrates another complete invariant: one map entry per list node, matching keys and stable iterators. Its copy semantics and allocation-failure rollback are part of design, not incidental implementation details.

## Exercises with answers

1. Why must expiry removal include reservation identity? **Answer:** Otherwise an old cleanup action can delete a newer allocation of the same seat.
2. Does Strategy require inheritance? **Answer:** No; a callable or variant may express the same variation more simply.
3. Why test time through injection? **Answer:** Boundary behavior becomes deterministic and fast without real waiting.
4. What invariant validates an expense split? **Answer:** Shares sum exactly to the original amount; global net balances sum to zero.
5. What is missing from the local confirm function for a database service? **Answer:** Transactional persistence, concurrency control, request identity, and external-operation reconciliation.

**Practical gate:** Complete one case study with class responsibilities, sequence diagrams, executable core behavior, and failure tests. Defend one simpler alternative you intentionally chose.
