# Practice workbook — 60 core assignments

These are original assignments, not links to a paid problem bank. Unless a task says otherwise, inputs use ordinary integers, invalid inputs must be rejected or explicitly excluded by a documented precondition, and returned indexes are zero-based. Attempt the base problem first. Then change the contract using the extension. The hints are answer directions, not replacement explanations: submit code, correctness reasoning, complexity, and tests.

## Set 1 — Foundations

**P01 — Stable sign grouping.** Read at most 10,000 integers and output negatives, zeros, then positives while preserving order inside each group. `[2,-1,0,-3,4] -> [-1,-3,0,2,4]`. Hint: three passes or three buffers. Extension: explain why simple swap partitioning need not preserve order.

**P02 — Integer parser.** Parse a decimal string into an int without stream conversion. Permit one optional sign, require at least one digit, and detect overflow before updating the accumulator. `"-42" -> -42`; `"42x"` fails. Hint: compare against the limit before multiplying by ten. Extension: handle the asymmetry of INT_MIN and INT_MAX.

**P03 — Summary function.** Return minimum, maximum, and total for a vector; represent an empty input explicitly. `[3,-2,5] -> (-2,5,6)`. Hint: initialize extrema from the first value. Extension: define a result struct and an optional result.

**P04 — Validated records.** Load one positive amount and one quoted description per line. Reject the entire dataset if any line is malformed. Hint: parse into a temporary collection and publish it only after validation. Extension: include a line number in errors.

**P05 — Lifetime diagnosis.** Write three short failing examples: returning a local reference, using a view of a temporary, and keeping a vector pointer across reallocation. Hint: identify the owner and the destruction/invalidation event. Extension: repair each without changing the intended result.

## Set 2 — STL

**P06 — Ranking.** Sort candidates by descending score, then ascending ID. Equal score/ID records retain arrival order. Hint: stable sort with a strict comparator. Extension: explain when arrival order is intentionally irrelevant.

**P07 — Word report.** Count lowercase words and print frequency descending, alphabetical ascending on ties. `a b a c b a -> a:3,b:2,c:1`. Hint: count first, sort records second. Extension: preserve case as a separate mode.

**P08 — Streaming top k.** For at most 100,000 values, return the k largest in descending order; require `0 <= k <= n`. Hint: a size-k min-heap. Extension: choose sorting when k is near n and justify the tradeoff.

**P09 — Duplicate range.** In a sorted vector, return `[first,last)` covering all occurrences of target; absent target gives an empty range at its insertion point. Hint: lower and upper bounds. Extension: explain non-random-access iterator costs.

**P10 — Safe callbacks.** Design a delayed callback that prints a name after the originating function returns. Hint: own the captured string rather than borrowing a local reference. Extension: explain capture by value versus reference with lifetime diagrams.

## Set 3 — Analysis and mathematics

**P11 — Sieve report.** List all primes up to 100,000 and answer repeated membership queries. `N=10 -> 2,3,5,7`. Hint: mark composites beginning at p². Extension: compute smallest prime factors for factorization queries.

**P12 — GCD reduction.** Reduce a fraction with a positive denominator, including zero numerator. `18/24 -> 3/4`. Hint: gcd and sign normalization. Extension: document a supported signed range avoiding minimum-value negation overflow.

**P13 — Modular exponent.** Compute a^b modulo m under the Module 3 limits. `3^5 mod 7 -> 5`. Hint: repeated squaring. Extension: explain why an arbitrary 64-bit modulus requires different multiplication.

**P14 — Josephus.** Return the zero-based survivor for `1 <= n <= 100000`, `1 <= k <= 100000`. `n=5,k=2 -> 2`. Hint: build the recurrence from n=1 upward. Extension: simulate small circles to cross-check it.

**P15 — Subset masks.** Enumerate all subsets of at most 20 positions, printing each selected index. Hint: bit membership. Extension: sum values in each subset and identify the output-sensitive runtime.

## Set 4 — Arrays and hashing

**P16 — Range increments.** Given n zeros and operations adding x to `[l,r)`, return the final vector. `n=4, add 2 to [1,3) -> [0,2,2,0]`. Hint: differences plus one prefix pass. Extension: explain why this does not directly support interleaved sum queries.

**P17 — Pair indexes.** Return distinct original indexes of any pair totaling target. `[4,1,4]`, target 8 -> `(0,2)`. Hint: hash previously seen values; do not reuse the same position. Extension: return every distinct index pair and account for output size.

**P18 — Maximum nonempty sum.** Return the maximum sum and one `[l,r)` achieving it. `[-5,-2,-7] -> -2,[1,2)`. Hint: Kadane with start tracking. Extension: define deterministic tie-breaking.

**P19 — Shipping capacity.** Positive package weights stay in order; find minimum daily capacity to ship in d days. `[1,2,3,4,5]`, d=2 -> 9. Hint: monotonic feasibility and binary search. Extension: reconstruct one valid schedule.

**P20 — Longest exact sum.** Values can be negative. Find the longest subarray summing to k. `[1,-1,5,-2,3]`, k=3 -> length 4. Hint: earliest occurrence of each prefix. Extension: compare with the frequency table used for counting.

## Set 5 — Strings and linear structures

**P21 — Overlapping matches.** Return every start of pattern in text. `ababa`, `aba -> [0,2]`. Hint: KMP preserves a border after a match. Extension: document empty-pattern semantics.

**P22 — List groups.** Reverse complete groups of k nodes; leave an incomplete suffix unchanged. `1,2,3,4,5`, k=2 -> `2,1,4,3,5`. Hint: confirm a complete group before rewiring. Extension: identify external ownership and cleanup.

**P23 — Histogram.** Return maximum rectangle area for nonnegative bar heights. `[2,1,5,6,2,3] -> 10`. Hint: monotonic stack and a final flush. Extension: return the chosen rectangle's bounds.

**P24 — Window maxima.** Return maxima of k-element windows, validating k. `[4,2,3,1]`, k=2 -> `[4,3,3]`. Hint: deque of candidate indexes. Extension: adapt for minima without duplicating the reasoning.

**P25 — Circular buffer.** Implement push and pop for a fixed positive capacity with explicit full/empty outcomes. Hint: wrapped indexes plus element count. Extension: test full wraparound twice.

## Set 6 — Trees and ranges

**P26 — Balanced tree.** Determine whether every node's child heights differ by at most one. Hint: return height or a failure sentinel from postorder. Extension: avoid recomputing subtree height.

**P27 — Tree round trip.** Serialize and deserialize a binary tree with null markers; reject malformed input. Hint: consume tokens in preorder. Extension: support duplicate node values.

**P28 — Prefix dictionary.** Support insert, exact search, and prefix search for lowercase words. Hint: trie terminal markers. Extension: implement deletion without removing shared prefixes.

**P29 — Dynamic sums.** Support point-add and range-sum operations for n<=100,000. Hint: Fenwick tree. Extension: add point assignment by tracking old values.

**P30 — Range additions.** Support range-add and range-sum online with n<=100,000. Hint: lazy segment tree; addition changes a sum by delta times interval length. Extension: combine assignment and addition only after specifying update composition.

## Set 7 — Graphs

**P31 — Island count.** Count orthogonally connected groups of 1s in a binary grid; diagonal contact does not connect. Hint: BFS/DFS from each unseen land cell. Extension: return component areas.

**P32 — Dependency schedule.** Return an ordering of tasks or an explicit cycle outcome. Hint: Kahn's algorithm. Extension: choose the smallest available ID with a heap for deterministic ordering.

**P33 — Cheapest route.** Directed nonnegative edges; return minimum cost and a path. Hint: Dijkstra plus parents. Extension: demonstrate a negative-edge counterexample to finalization.

**P34 — Incremental connectivity.** Process edge insertions and connectivity queries. Hint: DSU. Extension: explain why deletion changes the problem.

**P35 — Critical connections.** Return bridges in an undirected graph, permitting parallel edges. Hint: low-link DFS with edge IDs. Extension: also return articulation points using the root exception.

## Set 8 — Greedy, backtracking, and DP

**P36 — Activities.** Maximize the number of positive-length half-open intervals selected. Hint: earliest finish and an exchange argument. Extension: show a case where maximizing total value needs another algorithm.

**P37 — Queens.** Enumerate all N-Queens solutions for `1 <= n <= 10`. Hint: used columns and two diagonal sets. Extension: count instead of storing all boards.

**P38 — Coin reconstruction.** Return the minimum count and one sequence of coins totaling A<=10,000, or impossible. Hint: minimum DP plus predecessor coin. Extension: count unordered combinations separately.

**P39 — LCS reconstruction.** Return one longest common subsequence for strings of length<=500. Hint: full DP table and backward tracing. Extension: explain memory savings when only length is needed.

**P40 — Tree independent set.** Each tree node has a nonnegative reward. Select no adjacent nodes and maximize reward. Hint: take/skip states. Extension: reconstruct selected vertices.

## Set 9 — Design

**P41 — LRU.** Implement capacity-limited integer key/value caching with expected O(1) get/put. Define capacity-zero behavior. Hint: list plus hash map; consider copy semantics. Extension: implement the contract in the reference lab.

**P42 — Pricing strategies.** Implement flat and hourly parking prices with shared input validation. Hint: define policy contract first. Extension: choose inheritance or a simpler callable and explain why.

**P43 — Reservation states.** Implement hold, confirm, and expire with a fake clock. Hint: state transitions and identity checks. Extension: repeat confirmation with an idempotency key.

**P44 — Allocation cents.** Split 100 minor units among three people deterministically, with shares summing exactly to 100. Hint: quotient plus remainder distribution -> 34,33,33. Extension: support weighted shares and document rounding.

**P45 — Dependency isolation.** Test a booking persistence failure without a real database. Hint: injected fake repository. Extension: distinguish not-found, conflict, and infrastructure failure.

## Set 10 — SQL

Use the dataset in Module 10 unless the task adds tables.

**P46 — Absent bookings.** List customers without confirmed bookings. Hint: NOT EXISTS; expected Rafi and Maya. Extension: compare with a correctly formed left join.

**P47 — Totals including zero.** Show confirmed spending for every customer. Hint: outer join with status in ON and COALESCE; expected 2700,0,0. Extension: explain COUNT(*) versus COUNT(booking_id).

**P48 — Ranking ties.** Add another Amina booking of 1500 and show top price ranks. Hint: DENSE_RANK by amount, without ID in the rank ordering if price ties must remain ties. Extension: compare ROW_NUMBER.

**P49 — Join multiplication.** Create two reviews for Amina, join reviews and bookings, and demonstrate an inflated sum. Hint: preaggregate each one-to-many relation. Extension: explain why SUM(DISTINCT amount) is not a safe general fix.

**P50 — Double allocation.** Write a schema and transaction plan preventing two active owners of the same show/seat. Hint: unique allocation key. Extension: cancellation retains booking history but releases allocation.

## Set 11 — OS and networks

**P51 — Counter race.** Increment a counter from several threads and implement a defined correct version. Hint: mutex or suitable atomic. Extension: explain why an apparently correct observed run proves little.

**P52 — Bounded queue.** Implement blocking producers and consumers with a positive capacity and draining shutdown. Hint: predicate waits for not-empty/not-full or stop. Extension: define submission behavior during shutdown.

**P53 — Deadlock schedule.** Construct a two-lock circular wait on paper, then repair the acquisition protocol. Hint: consistent order. Extension: identify callbacks that can violate that order indirectly.

**P54 — Framing parser.** Parse a four-byte length header and payload from arbitrarily split byte chunks, rejecting lengths above 64 KiB. Hint: explicit header/payload state. Extension: parse two coalesced messages.

**P55 — Retry contract.** Design safe retry behavior for a timed-out create-booking request. Hint: stable operation identity and stored outcome. Extension: distinguish server rejection from unknown result.

## Set 12 — Placement and explanation

**P56 — Percentages.** Increase a price 10%, then reduce it 10%; explain the net change. Answer direction: factor 0.99, a 1% loss. Extension: derive the general equal-percentage result.

**P57 — Combined work.** A takes 12 days, B takes 18; find combined time under constant independent rates. Answer direction: 1/12+1/18=5/36, so 36/5 days. Extension: B joins after A works alone for three days.

**P58 — Structured walkthrough.** Explain your worker pool in three minutes, including queue invariant, exception behavior, and shutdown. Hint: start with observable behavior. Extension: discuss a deadlock that the pool cannot prevent.

**P59 — AI verification.** Ask an assistant to review a deliberately faulty binary search, then verify every proposed issue using a failing input or proof. Hint: count checkable claims. Extension: redo the corrected algorithm independently.

**P60 — Final mock.** Select one array, one DP, one SQL, and one core CS problem you have not solved recently. Explain and solve them under timed conditions. Hint: topic labels remain hidden until review. Extension: revise the exact reasoning mistake, then change one assumption.

## Track your work

For each assignment, record date, independent attempt, hint used, failing case, final reasoning, and next revision date. The purpose is durable understanding. Do not treat the workbook's 60 base assignments as a claim that it includes hundreds of fully authored problem statements; additional practice can be selected after these expose your weak areas.
