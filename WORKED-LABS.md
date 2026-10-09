# Worked labs — tracing full implementations

The [algorithm reference program](examples/algorithm_lab.cpp) contains complete implementations and deterministic checks. Compile it as a standalone C++17 file after attempting the workbook's corresponding tasks. The checks are supplied, not reported as executed during authoring.

## Lab 1 — Window maximum without rescanning

Trace values `[4,2,3,1]` with k=2. The deque stores indexes, from oldest to newest, with values in decreasing order.

| New index | Action | Candidate indexes | Output |
|---|---|---|---|
| 0, value 4 | Insert | [0] | None: window incomplete |
| 1, value 2 | Insert | [0,1] | 4 |
| 2, value 3 | Expire 0; remove dominated 1; insert 2 | [2] | 3 |
| 3, value 1 | Insert | [2,3] | 3 |

The key proof is domination: index 1 has a smaller value and expires earlier than index 2, so it cannot win any future window that also contains index 2. Each index is removed only once. Do not count the inner loop as n work for every outer iteration independently; use total pushes and pops.

**Experiment:** Change the domination comparison from `<=` to `<`. Both can return correct maxima, but the latter retains equal candidates. Explain how this affects which equal index remains and why expiry still works.

## Lab 2 — Lazy segment tree state

Start with `[1,2,3,4]`. Add 5 to `[1,3)`, producing `[1,7,8,4]` and total 20. Add -2 to the whole range, producing `[-1,5,6,2]` and total 12.

The second update can modify the root sum and root lazy value without touching all leaves. A later query of `[2,4)` must push the root's pending -2 into each child's aggregate, using child interval length. The answer is 8, not the old right-child sum 12.

The implementation uses half-open intervals everywhere. Empty queries return zero. No-overlap branches return the addition identity zero. Full-overlap branches use the stored sum. Partial branches push, recurse, and merge. On an update, recompute the parent after child changes.

**Experiment:** Comment out the `push` call before a partial query and find a failing test. Then comment it out before a partial update and find a different failure. Explain which invariant each omission violates.

**Numeric contract:** If n<=100,000, initial magnitudes<=1,000,000, at most 100,000 additions, and each delta magnitude<=1,000,000, worst-case aggregate magnitude is on the order of 10^16, within signed 64-bit range. Different input contracts need a new bound.

## Lab 3 — LRU ownership and eviction

With capacity two:

1. Put (1,10): order `[1]`.
2. Put (2,20): order `[2,1]`.
3. Get 1: order `[1,2]`.
4. Put (3,30): order becomes `[3,1]`; key 2 is evicted.

The map points into the list. `splice` changes order while preserving list iterators. Removing a node requires first removing its map entry. The implementation disables copying because copied map iterators would otherwise refer to the wrong list. Adding a correct copy operation is an advanced exercise.

New insertion first creates a list node, then inserts its map entry. If the second allocation fails, remove the list node to restore the invariant. Evict only after successful insertion, so an allocation failure does not unnecessarily lose an existing cached value.

**Experiment:** Implement `peek`, which retrieves without updating recency. Show that `peek(1)` followed by an insertion can have a different eviction outcome from `get(1)`.

## Lab 4 — Recovering an LCS

For `abcde` and `ace`, matching a/a, c/c, and e/e builds length three. Backtracking begins at the table's bottom-right. Equal final characters belong to a valid reconstructed optimum; unequal characters choose a neighboring cell preserving the optimum length. Collected characters arrive backward, so reverse them.

The table is O(nm) time and space. Keeping only two rows is sufficient for the length, but loses ordinary full-table reconstruction information. Space savings are a different contract from returning an actual subsequence.

**Experiment:** Use `abc` and `bac`, which have multiple longest answers. Test validity and optimal length rather than insisting on one arbitrary string unless you define tie-breaking.

## Lab 5 — Why knapsack runs backward

One item has weight 2 and value 3; capacity is 4. With descending updates, capacity 4 reads the old state for capacity 2, still zero, and records value 3. Later capacity 2 becomes 3. The item is used once.

With ascending updates, capacity 2 becomes 3 before capacity 4 reads it; capacity 4 becomes 6, illegally reusing the same item. A change of loop direction changes the problem being solved.

**Experiment:** Implement an unbounded version using ascending updates and compare both on the one-item example. Then reconstruct selected items for the 0/1 version using a two-dimensional table.

## Lab report

For each lab, provide a trace, the invariant, one intentionally introduced bug, a failing test, the repair, and complexity. The executable's existing checks cover representative behavior, not all possible inputs. Add randomized comparisons with simple baselines for window maximum, range sums, and tiny knapsack instances.
