# Projects — implement, test, and defend

Use the reference programs after attempting the first milestone. They are learning implementations, not substitutes for the acceptance criteria. Their code has been supplied for review; it has not been executed in this authoring session because the terminal runtime was unavailable.

## Project A — Persistent expense tracker

**User story:** I want to record expenses in integer minor units, list them, calculate a total, and restore them after restarting.

**Start:** [Reference C++ program](examples/expense_tracker.cpp). Compile it as a standalone C++17 program. Run with an optional data-file path, for example `expense_tracker expenses.txt`.

Commands:

```text
add 1250 "Lunch"
add 300 "Bus ride"
list
total
save
quit
```

The reference uses quoted descriptions and stores one expense per line. It does not save automatically on exit; explicitly use `save`. Its file overwrite is not crash-atomic. This is a deliberate persistence-hardening assignment, not a guarantee that partial writes are recoverable.

### Milestones

1. Implement in-memory add, list, and total. Keep total logic independent of console handling.
2. Reject zero/negative amounts, malformed commands, empty descriptions, and totals outside supported range.
3. Save and load records. Reject malformed files without silently accepting a partial dataset.
4. Add categories and filtered totals without duplicating total-calculation code.
5. Harden saving: write a temporary file, validate it, and investigate the platform's replace/rename guarantees. Document what is guaranteed and what is not.

### Acceptance tests

- Empty dataset totals zero.
- Add 1250 and 300; total is 1550.
- Save, restart, and verify both records and total.
- Reject `add -20 "Refund"` under this expense-only contract.
- Reject `add 20 ""` and trailing command garbage.
- Reject an incomplete record in the input file.
- A failed save reports failure and does not claim success.

**Review questions:** Why integer money? Who owns the stream? Which operation is O(n)? What happens if power is lost during save?

## Project B — Seat booking with competing clients

**User story:** I want to reserve and confirm a seat without another customer obtaining the same active seat.

Build a C++ domain layer first, then connect a relational database. Begin with one show and five seats. Use an injected clock so expiry tests do not depend on waiting in real time.

### Domain contract

- A seat is available, held by one reservation, or confirmed by one booking.
- A hold has a unique ID and an expiry instant.
- Confirmation requires a matching, unexpired hold.
- A cancelled or expired hold releases its seat.
- Repeating a successful confirmation with the same request ID returns the original outcome.
- A different request cannot confirm a seat already confirmed by someone else.

At exactly the expiry instant, define the hold as expired. Keeping this boundary explicit prevents inconsistent tests.

### Suggested responsibilities

- `BookingService`: transitions and business rules.
- `Clock`: current time, with a controllable test implementation.
- `Repository`: persistence contract.
- `PricingPolicy`: quote calculation.
- `Reservation`: identity, seat, owner, state, and expiry.

Do not begin with a large class hierarchy. Add a separate responsibility when a change or test demonstrates its value.

### Database milestone

Store booking history separately from active seat allocations. Enforce uniqueness of `(show_id, seat_code)` in allocations. Persist the allocation and booking transition in one transaction. Catch duplicate-allocation conflicts and return a documented “seat unavailable” outcome. Do not rely only on reading availability first.

### Acceptance tests

1. Hold an available seat; another hold fails.
2. Confirm before expiry; the seat becomes confirmed.
3. Advance the fake clock to expiry; confirmation fails.
4. Expire a hold; a new reservation can acquire the seat.
5. Repeat the same confirmation; no second booking is created.
6. Simulate persistence failure; no partial successful transition is reported.
7. Run two clients racing for the same allocation; exactly one active allocation exists after completion.

**Stretch:** Cancellation policy, multiple shows, payment-result reconciliation, and an operation-status endpoint for uncertain responses.

**Review questions:** Which rule is enforced in code, which in the database, and why? Can expiry cleanup race with confirmation? What does a timeout tell the caller?

## Project C — Concurrent worker pool

**User story:** I want to submit computation tasks, collect their results, and shut down after accepted tasks finish.

**Start:** [Reference C++ program](examples/thread_pool.cpp). This includes a worker pool, result futures, and demonstration of a task exception. Compile with C++17 and your toolchain's thread support options.

### Invariants

- The task queue and stopping flag are protected by one mutex.
- A worker takes a task under the lock but executes it outside the lock.
- Shutdown wakes all workers.
- Workers exit only after shutdown is requested and the queue is empty.
- Task exceptions travel through futures instead of terminating the worker.
- The owner arranges that the object is not destroyed while another thread is entering its public methods.

### Milestones

1. Trace two workers processing four tasks.
2. Add a bounded queue and define whether full submissions block or fail.
3. Measure throughput for CPU-bound work with different worker counts.
4. Add cancellation for tasks not yet started; document running-task behavior.
5. Record queue length and execution timing without introducing data races.

### Acceptance tests

- Reject zero workers.
- Every accepted task executes exactly once.
- Results match direct calculation.
- A thrown task exception is rethrown by its future.
- Destroying the pool drains already accepted work.
- No worker is left waiting after shutdown.

**Design warning to understand:** If every worker waits for another task queued to the same exhausted pool, none may remain to execute that task. Explain why “thread-safe queue” does not mean “deadlock-free application.” Do not destroy the pool from one of its own tasks.

## Project submission package

For two completed projects, submit source, build instructions, tests, example usage, a responsibility diagram, one performance measurement where meaningful, and known limits. Your README must distinguish implemented features from planned ones.

Grade each out of 20: correctness 8, failure handling 4, design clarity 3, tests 3, and explanation 2. A project with an untested central invariant requires revision regardless of its appearance.
