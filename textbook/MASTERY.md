# Expanded mastery examinations and worked solutions

[Contents](../TEXTBOOK.md)

First attempt each problem without its solution. For implementation questions, submit a contract, code, tests, and complexity. A recognized answer without a derivation does not pass the gate.

## Gate 1 — C++ language

### 1. Values, aliases, and captures

```cpp
int original=4;
int& alias=original;
auto copied=alias;
auto callback=[copied]() mutable { return ++copied; };
alias=10;
```

What are the values and the results of two callback calls?

**Solution:** original and alias designate the same integer, now 10. Plain auto created an independent copied integer equal to 4. The lambda owns another copy initialized to 4. Calls return 5 and 6; mutable changes the lambda's stored capture, not the external copied variable.

### 2. A returned view

A function returns string_view into a local string. Repair it in two ways.

**Solution:** Return an owning string, or accept caller-owned storage and return a subview under an explicit lifetime contract. The original fails because the local owner dies at return. A static backing object is only appropriate when its shared lifetime and mutation behavior match the requirements.

### 3. Checked average

Return an average for at most 100,000 marks in 0–100; empty input has no average.

```cpp
#include <optional>
#include <stdexcept>
#include <vector>
std::optional<double> average(const std::vector<int>& marks) {
    if (marks.size()>100000) throw std::invalid_argument("Too many marks");
    if (marks.empty()) return std::nullopt;
    long long sum=0;
    for (int mark:marks) {
        if (mark<0 || mark>100) throw std::invalid_argument("Invalid mark");
        sum+=mark;
    }
    return static_cast<double>(sum)/static_cast<double>(marks.size());
}
```

**Explanation:** Sum is at most ten million. Conversion occurs before division. Optional distinguishes absence from valid zero. Const reference avoids copying. Test empty, one zero, one hundred, `[0,100]`, an invalid mark, and oversized input.

### 4. Header guards

Why can an ordinary function definition in a guarded header cause a link error when included by two source files?

**Solution:** Guards operate within each translation unit. Both can still define the same external function. Put its ordinary definition in one source file, or use an appropriate inline definition when a header definition is intended.

### 5. Failed loading

A loader clears current records and fails halfway through parsing. Preserve the old dataset on failure.

**Solution:** Parse and validate into a temporary owning collection, including cross-record rules. Commit only after full success using an operation with the needed exception guarantee. RAII destroys the temporary on failure.

## Gate 2 — Standard library

### 6. Structure selection

Choose structures for indexing with append, ordered predecessor, expected-fast key lookup, moving known nodes, and repeated smallest extraction.

**Solution:** Vector, ordered set/map, unordered set/map, list, and minimum-first priority_queue. Explain qualifications: finding a list node can remain linear, hashing has a worse worst case, and priority_queue does not expose arbitrary lookup.

### 7. Erase iteration

Repair a vector loop that increments an iterator after erasing through it.

**Solution:** Use the iterator returned by erase and increment only on the non-erasing path. For bulk filtering, remove_if plus one erase avoids repeated shifts. Check other borrowed references at or beyond the erased position too.

### 8. Reserved output

Why is copying into begin of an empty vector after reserve invalid?

**Solution:** Capacity is not constructed size. Resize first or use back_inserter. An ordinary output iterator must designate valid writable elements.

### 9. Selection

Find one median without fully sorting. What is not guaranteed afterward?

**Solution:** Use nth_element for the chosen rank. Its ordinary overload offers average linear selection, but neither partition is fully sorted. Define even-length median behavior and arithmetic before implementing.

### 10. Shared ownership

Two threads hold shared_ptr to one vector and both push_back. Is this safe?

**Solution:** Shared ownership manages lifetime, not the vector's concurrent mutations. Use synchronization around the invariant or separate per-thread data and combine safely.

## Gate 3 — Algorithms

### 11. Exact sum

Count nonempty subarrays summing to k with negative values allowed.

**Solution:** An earlier prefix p pairs with current s when p=s-k. Initialize frequency[0]=1. Add frequency[s-k] before incrementing frequency[s], avoiding counting a current empty subarray for k=0. Expected O(n) time, O(n) memory, and a wide answer count.

### 12. Wrong shortest-path algorithm

Give a weighted counterexample to BFS as a cheapest-path solver.

**Solution:** S->T costs 10 while S->A->T costs 1+1. BFS minimizes edge count, choosing one edge rather than cost 2. Dijkstra fits nonnegative weights.

### 13. Knapsack direction

Why does ascending capacity reuse a 0/1 item?

**Solution:** The current item's newly updated smaller-capacity state feeds a larger capacity in the same pass. One weight-2/value-3 item with capacity 4 incorrectly yields 6. Descending updates preserve the previous-item layer.

### 14. Lazy state

Start `[2,4,6,8]`, add 3 everywhere, then -2 to `[1,3)`. Find final sum and explain partial descent.

**Solution:** Values become `[5,5,7,11]`, total 28. Push pending ancestor updates into child aggregates before partially querying/updating them; otherwise they omit changes represented only above.

### 15. State sufficiency

Can a maze with collectible keys use only visited[row][column]?

**Solution:** No. Position plus key set defines future legal moves. Include a key mask and account for the increased state-space size.

## Gate 4 — Design, databases, and systems

### 16. Concurrent booking

Both clients read an available seat before inserting. Prevent two owners.

**Solution:** Enforce a unique active show/seat allocation and use a transaction for related changes. The read precheck is advisory. Handle the losing operation as conflict and use stable request identity for uncertain retries.

### 17. Outer join filtering

Why can a WHERE condition on the right table remove unmatched left rows?

**Solution:** Right columns are NULL for unmatched rows, so the comparison is unknown and WHERE discards it. Put qualifying-match conditions in ON when preserving all left rows is intended.

### 18. Notification

After a condition-variable notification, may a consumer assume work exists?

**Solution:** No. A wake can be spurious or another consumer can take the work. Recheck the predicate under the mutex, include shutdown, and define draining versus cancellation.

### 19. Timeout

Why can retrying a timed-out booking create a duplicate?

**Solution:** The first request may have committed before its response was lost. A stable operation ID and stored result allow recovery of the same logical operation.

### 20. Project invariant

Explain one invariant and a failure test for your project.

**Solution structure:** State the rule, enforcement boundary, attempted violating sequence, and expected outcome. Example: two concurrent allocation attempts produce one committed active seat owner and one conflict. A diagram alone is not executable evidence.

## Grading and revision

Score each problem out of five: one for identifying the concept, two for reasoning, one for a concrete example/test, and one for assumptions or limitations. Aim for 80/100 and repair any failed central correctness, lifetime, or concurrency question regardless of total. Retake changed variants after a delay instead of immediately repeating remembered wording.
