# Volume 3 — Analysis, correctness, mathematics, and recursion

[Contents](../TEXTBOOK.md) · [Introductory module](../03-analysis-math-recursion.md)

## Chapter 1. Model the input before counting work

For a graph, one size n is often insufficient: vertex count V and edge count E can grow differently. For an integer algorithm, the magnitude N and the number of bits log N are different measures. A dynamic program taking O(N) work in an amount N is pseudopolynomial when N is encoded in binary.

Count the dominant repeated operations under a stated model. Comparing two fixed-size integers is ordinarily treated as O(1); comparing two long strings may inspect many characters. A map with string keys can therefore have costs beyond a simplistic “log n.” Include key-processing costs when they matter.

For `for(i=1;i<=n;i*=2)`, after k iterations i is 2^k, so k is logarithmic. For an inner loop starting at i and advancing by i up to n, total work across i is `n/1+n/2+...+n/n`, which is Θ(n log n). It is neither automatically n² because loops are nested nor automatically linear because each inner loop skips positions.

Worst-case describes the most expensive input of a given size. Average-case needs a distribution assumption. Amortized bounds apply over a sequence of operations without assuming random inputs. Keep these three claims separate.

## Chapter 2. Invariants, induction, and termination

An invariant must be strong enough to imply the final result. “The loop has processed some elements” is too weak. For insertion sort, a useful statement is: before iteration i, prefix `[0,i)` is sorted and contains exactly the original prefix values. Inserting element i into its proper prefix position preserves both ordering and membership. At i=n, the whole array is sorted with no lost values.

Correctness has two parts: partial correctness says termination gives the right answer; termination says execution actually finishes. A decreasing nonnegative measure proves many loops terminate. In binary search, the interval length shrinks. In DFS, each previously unseen vertex is marked once.

For recursion, prove a base case and a smaller-instance implication. Merge sort assumes recursive halves are sorted, then proves that repeatedly choosing the smaller remaining front produces a sorted combined sequence preserving all values. A proof should also account for empty ranges and duplicates.

## Chapter 3. Recurrences and amortization

`T(n)=T(n-1)+Θ(1)` gives Θ(n). `T(n)=T(n/2)+Θ(1)` gives Θ(log n). `T(n)=2T(n/2)+Θ(n)` gives Θ(n log n). Derive these using recursion depth and work per level instead of selecting a theorem by superficial resemblance.

For dynamic-array growth by doubling, the expensive copies form a geometric sum. Over n appends they sum to O(n), so the average cost per operation in the sequence is O(1). If capacity increased by one each time, copies sum to Θ(n²). Growth policy creates the guarantee.

For a monotonic stack, charge each push and pop to its element. An element can be popped once, so a nested while does not imply quadratic work. This accounting argument recurs in sliding-window deques and path-compression intuition.

## Chapter 4. Number theory as reusable identities

Euclid works because replacing a by `a-qb` preserves common divisors. For 84 and 30: remainders are 24,6,0, so gcd is 6. A fraction 84/30 reduces to 14/5. LCM is obtained by dividing before multiplying, with explicit overflow checking if arbitrary input ranges are supported.

Trial division only needs factors up to sqrt(n): if both factors exceeded it, their product would exceed n. To factor n, repeatedly divide by each candidate and reduce the remaining n. Any remainder above one after trial division is prime. For many queries, a smallest-prime-factor sieve trades preprocessing and storage for fast repeated factorization.

In modular arithmetic, addition and multiplication respect congruence. Division is different: solving `b*x ≡ a (mod m)` by an inverse requires gcd(b,m)=1. For example 2 has no inverse modulo 6 because every multiple remains even modulo 6; it cannot equal 1. Extended Euclid finds coefficients x,y with `ax+by=gcd(a,b)`, yielding an inverse when gcd is one.

## Chapter 5. Combinatorics and numeric bounds

Choosing r positions from n gives `C(n,r)`. The recurrence `C(n,r)=C(n-1,r-1)+C(n-1,r)` separates selections including the last item from those excluding it. Pascal's triangle computes small values without factorial division. Factorial formulas can overflow intermediate products even when the final result fits.

```cpp
#include <algorithm>
#include <vector>
// Small bound: 0 <= r <= n <= 60, with all results fitting unsigned long long.
unsigned long long choose(int n, int r) {
    std::vector<unsigned long long> row(r + 1, 0);
    row[0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = std::min(i, r); j >= 1; --j)
            row[j] += row[j - 1];
    return row[r];
}
```

The backward update preserves the preceding row values. An upward update would combine already updated values and change the recurrence. This is the same dependency-order issue later seen in knapsack.

## Chapter 6. Bit reasoning beyond tricks

Bit masks encode subsets, flags, and parity. XOR is addition modulo two on each bit, so pairing equal values cancels. A mask does not encode order or multiplicity unless extra structure is added. If the problem needs “item used twice,” one membership bit is insufficient.

For all nonzero submasks of a mask, iterate `sub=(sub-1)&mask`, starting at mask and stopping before cycling through zero. Subtracting one changes the lowest selected bit and lower bits; masking removes bits outside the original set. Include zero separately when required. Enumerating all masks and all their submasks has total 3^n combinations, because each item is absent from mask, in mask but not submask, or in both.

## Chapter 7. Recursive design clinic

For permutations, the state is a partially filled arrangement plus unused choices. For combinations, enforce increasing chosen indexes to avoid representing the same subset in different orders. For Tower of Hanoi, moving n disks uses move n-1 aside, move largest, move n-1 back; the recurrence is `T(n)=2T(n-1)+1`, giving `2^n-1` moves.

Memoization helps only when equivalent states recur and the cached result fully captures the subproblem. Caching by recursion depth alone is wrong when different partial sums at that depth lead to different answers. State design is a semantic decision, not merely adding a map to recursion.

## Worked exercises

1. Analyze n iterations each doing a binary search on n values. **Solution:** O(n log n), assuming random-access search and constant-time comparisons.
2. Explain why factorial is a poor way to calculate a large binomial coefficient. **Solution:** Intermediate factorials can overflow; use bounded Pascal DP, cancellation, or a justified modular method.
3. Find inverse of 3 modulo 7. **Solution:** 5, because 3*5=15≡1. The gcd condition holds.
4. Why does a recursion with two calls not always take exponential time? **Solution:** Calls on disjoint halves have a different recurrence from two calls reducing n by one; memoization can also merge repeated states.
5. Prove subset generation has 2^n leaves. **Solution:** Every position independently chooses include/exclude, producing a unique length-n binary choice sequence.

**Practical gate:** For five solved problems, write an input-size definition, correctness argument, time bound, space bound, and numeric bound. Then find one counterexample to a plausible incorrect solution for each.
