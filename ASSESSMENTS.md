# Assessments and worked answer keys

Do not read a key until the assessment is complete. Use a compiler for implementation rounds, but do the derivation first without external assistance. These assessments measure the course's taught skills; they do not predict any particular employer's outcome.

## Assessment A — Foundations and linear algorithms

Time: 90 minutes. Total: 100 points.

### A1 — Explain and repair (20 points)

1. Why can `long long answer = a * b;` overflow when a and b are int?
2. A vector is reserved to capacity 10, then index zero is assigned before any resize or insertion. Explain the error.
3. Why does Kadane initialized with zero fail for a nonempty all-negative input?
4. Give a case where a variable sum window cannot rely on positive-only reasoning.

### A2 — Exact-sum count (40 points)

Write `long long countExact(const vector<int>& values, long long target)`. Values may be negative; length is at most 100,000; each value is between -1,000,000 and 1,000,000. Return the number of nonempty contiguous subarrays summing to target. Example: `[1,-1,1]`, target 1 -> 3.

Give an O(n²) baseline, derive an expected O(n) method, and include tests for zero, negative values, and an empty input.

### A3 — Window extrema (40 points)

Return the maximum of every k-element window. Require `1 <= k <= n`; reject invalid k. Example: `[1,3,-1,-3,5,3,6,7]`, k=3 -> `[3,3,5,5,6,7]`. Explain why discarded candidates are unnecessary.

### Answer key A

A1: Multiply as `long long` before assignment; reserve does not construct elements; zero admits an empty solution; negative values can break sum monotonicity. Award five points each with a concrete explanation.

A2: Maintain prefix frequencies beginning with `{0:1}`. At each position, add the frequency of `prefix-target`, then increment the current prefix. For the example, valid subarrays are `[1]` at index 0, the full array, and `[1]` at index 2. O(n) expected time, O(n) storage. Up to n(n+1)/2 results requires `long long`. Award 10 derivation, 20 correct code, 5 complexity, 5 tests.

A3: Store candidate indexes in a deque. Remove front indexes outside the current window. Remove back values no larger than the new value. Add the new index; once the first full window exists, report the front value. Each index enters and leaves once. Award 10 invariant, 20 correct code, 5 complexity, 5 validation/tests.

## Assessment B — Trees, graphs, and DP

Time: 120 minutes. Total: 100 points.

### B1 — Diameter (25 points)

Compute a binary tree's diameter in edges. Empty tree -> 0; one node -> 0; a four-node chain -> 3. Visit each node only once. Explain recursion-space behavior.

### B2 — Route model (35 points)

A directed graph has nonnegative integer edge weights. Find the minimum route cost from source to all vertices, preserving an unreachable result. Explain why BFS is insufficient, how stale heap entries are handled, and how numeric range is controlled. Add parent tracking to reconstruct one route.

### B3 — Minimum coins (25 points)

Given distinct positive denominations and an amount between 0 and 10,000, find the fewest coins using unlimited copies. `[1,3,4]`, amount 6 -> 2; `[2]`, amount 3 -> impossible. Define every DP state and give a reconstruction method.

### B4 — Proof or counterexample (15 points)

1. Does a minimum spanning tree provide shortest source routes?
2. Is earliest-finishing interval selection valid for maximizing the number of nonoverlapping intervals?
3. Can ascending one-dimensional capacity updates enforce once-per-item knapsack?

### Answer key B

B1: Postorder returns node-count height. Update the best with leftHeight+rightHeight. O(n) time, O(h) auxiliary recursion space; h can be n. Do not repeatedly recompute heights. Award 15 implementation and 10 explanation/tests.

B2: Dijkstra finalizes the smallest distance because weights are nonnegative. BFS counts edges rather than costs. Skip entries whose saved distance differs from the current table. Relax valid additions within the numeric contract and update parent on improvement. Follow parents to reconstruct. Award 10 modeling/proof, 15 code, 5 range/unreachable handling, 5 tests.

B3: `dp[s]` is the minimum coins for exact sum s; zero has count zero, others are unreachable initially. For each last coin c, try `dp[s-c]+1`. Record the chosen c on improvement, then subtract it while reconstructing. O(amount * denominations) time. Award 10 state/transitions, 10 implementation, 5 reconstruction.

B4: (1) No; MST minimizes total selected weight. (2) Yes under the stated interval contract, via the exchange argument. (3) No; it may reuse the current item. Five points each with reasoning.

## Assessment C — Design, SQL, core CS, and communication

Time: 120 minutes. Total: 100 points.

### C1 — Booking design (30 points)

Specify a seat-hold service with expiry, confirmation, cancellation, and retry-safe confirmation. Draw legal states, assign responsibilities, and prevent concurrent double allocation. Describe persistence failure behavior.

### C2 — SQL (25 points)

Using the Module 10 dataset: list customers with zero confirmed bookings; show confirmed spending per customer including zero; explain how joining another one-to-many table can corrupt a sum.

### C3 — Core CS (25 points)

Explain a data race, a condition-variable predicate, TCP message framing, and why a timed-out write might have succeeded. Give a concrete example for each.

### C4 — Project walkthrough (20 points)

Present one implemented project in five minutes. Include requirements, architecture, one failure path, one measured result or explicit unmeasured limit, and a change you would make next.

### Answer key C

C1: Available/Held/Confirmed with explicit expiry and reservation identity; database uniqueness for active seat allocations; transactional changes; retry identity; injected clock and repository for tests. No success report after a rolled-back write. Award points for consistent behavior, not the number of classes.

C2:

```sql
SELECT c.customer_id, c.name
FROM customers c
WHERE NOT EXISTS (
    SELECT 1 FROM bookings b
    WHERE b.customer_id = c.customer_id AND b.status = 'CONFIRMED'
);

SELECT c.customer_id, c.name, COALESCE(SUM(b.amount_minor), 0) AS spent
FROM customers c
LEFT JOIN bookings b
  ON b.customer_id = c.customer_id AND b.status = 'CONFIRMED'
GROUP BY c.customer_id, c.name;
```

The first result contains Rafi and Maya. Spending is Amina 2700, Rafi 0, Maya 0. Additional one-to-many joins can multiply rows; preaggregate each relation at customer grain before joining.

C3: Conflicting unsynchronized non-atomic access is undefined behavior; wait predicates account for spurious wakes and changed state; a byte stream needs explicit record boundaries; a commit can succeed before its response is lost. Award for accurate examples and consequences.

C4: Requirements 4, implemented architecture 4, failure reasoning 4, evidence/limits 4, clear ownership of contributions 4.

## Progress rule

Aim for at least 75/100 on each assessment with no unresolved central correctness failure. This is an internal learning threshold, not an industry standard. If a core algorithm fails, revise it even if explanation points raise the total. Retake with a changed input contract, not only the same remembered questions.
