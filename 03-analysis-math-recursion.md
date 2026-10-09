# Module 3 — Analysis, mathematics, and recursion

**Expanded textbook:** [Analysis, proofs, mathematics, and recursion](textbook/03-analysis.md). This file is the introductory/revision version.

Prerequisite: Modules 1–2. Outcome: derive costs and defend correctness.

## Lesson 3.1 — Counting work

Complexity describes how resource use grows with input size, not how many milliseconds one machine takes. A loop visiting n values is Θ(n). Two independent n-element loops remain Θ(n), because adding n + n changes a constant factor. Nested loops can be quadratic, but inspect their bounds: a loop doubling its index runs only about log2(n) iterations.

For a loop where `j` runs from 0 through `i - 1`, total work is `0 + 1 + ... + n - 1 = n(n - 1)/2`, hence Θ(n²). Binary search halves the candidate interval each iteration, so after k iterations its size is approximately n/2^k. Setting that to 1 gives k approximately log2(n).

Big O is an asymptotic upper bound, Omega a lower bound, and Theta a tight bound. These are separate from best/average/worst-case choices. You can state the worst-case running time as Θ(n²), for example.

Auxiliary space excludes the input but includes temporary arrays and active recursion frames. A recursive O(n)-time algorithm can also consume O(n) stack space. State which convention you use.

**Exercise:** A function sorts n values, then scans them once. **Answer:** O(n log n) + O(n) = O(n log n). The scan does not dominate sorting.

## Lesson 3.2 — Amortized cost and recurrences

Suppose a dynamic array doubles when full. Appending n elements requires copying about `1 + 2 + 4 + ...`, whose total is less than 2n when capacity grows geometrically. Total work over n appends is O(n), even though a particular append can take O(n). This is amortized analysis, not an assertion about a random input distribution.

Merge sort satisfies `T(n) = 2T(n/2) + Θ(n)`. Every level performs Θ(n) merging, and there are Θ(log n) levels, giving Θ(n log n). Naive recursive Fibonacci satisfies a branching recurrence and repeats overlapping subproblems; drawing the calls reveals why it is much slower than a single n-step loop.

**Lab:** Count calls for naive Fibonacci for small n; then memoize it. Compare operation counts rather than relying only on wall-clock timing.

## Lesson 3.3 — Proving a program

A proof connects assumptions to a result. For iterative algorithms, identify an invariant and establish initialization, preservation, and termination. For recursive algorithms, use induction: assume smaller valid inputs are solved correctly and show that combining them solves the current input.

To prove binary exponentiation, maintain that `result * base^exponent` equals the original target, interpreted under the chosen arithmetic. If the exponent is odd, multiply the result by the base. Squaring the base and halving the exponent preserves the target. When the exponent reaches zero, the result is the target.

```cpp
#include <stdexcept>
// Preconditions: 0 <= exponent; 1 <= modulus <= 1,000,000,007.
long long powerMod(long long base, long long exponent, long long modulus) {
    if (exponent < 0 || modulus < 1 || modulus > 1000000007LL)
        throw std::invalid_argument("Unsupported exponent or modulus");
    base %= modulus;
    if (base < 0) base += modulus;
    long long result = 1 % modulus;
    while (exponent > 0) {
        if (exponent & 1LL) result = result * base % modulus;
        base = base * base % modulus;
        exponent /= 2;
    }
    return result;
}
```

The modulus restriction ensures these products fit in `long long`. For arbitrary 64-bit moduli, ordinary multiplication can overflow before `%` is applied; a different multiplication strategy is needed. Time is O(log exponent), auxiliary space O(1).

## Lesson 3.4 — GCD, primes, and preprocessing

Euclid uses `gcd(a, b) = gcd(b, a mod b)`. A common divisor is preserved by subtracting multiples of b, and the remainder shrinks until it is zero.

```cpp
// Nonnegative inputs only.
long long gcdNonnegative(long long a, long long b) {
    while (b != 0) {
        long long remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}
```

Define `gcd(0, 0)` as zero for this API. For positive a and b, LCM is `(a / gcd(a,b)) * b`; dividing first reduces intermediate size, but does not guarantee the final multiplication fits.

A composite number has a factor at most its square root. Test divisors while `d <= n/d` to avoid overflowing `d*d`. For many primality queries up to N, preprocess with a sieve: initially mark all candidates, then mark multiples of each surviving prime beginning at p². Smaller multiples already have a smaller prime factor. The standard sieve uses O(N) space and O(N log log N) time.

Counting combinations often involves huge intermediate factorials. Reduce using recurrence, cancellation, or modular methods. Modular division means multiplying by an inverse, which exists only when the divisor and modulus are coprime. Fermat-based inversion also requires a prime modulus and a nonzero residue.

## Lesson 3.5 — Bits as a compact set

An unsigned integer can represent membership: bit i is one when item i is selected. Use `mask | (1ULL << i)` to include an item, `mask & ~(1ULL << i)` to remove it, and `mask ^ (1ULL << i)` to toggle it. The shift index must be less than the type's bit width.

`x & (x - 1)` clears the lowest set bit when x is nonzero. Therefore a nonzero unsigned value is a power of two exactly when that expression is zero. XOR cancels equal values: `a ^ a == 0` and `a ^ 0 == a`. The “single unpaired value” trick requires all other values to occur an even number of times.

**Exercise:** Enumerate all subsets of three items. **Answer:** Masks 0 through 7; inspect each of the three bits. There are 2^n subsets, and printing each full subset can require O(n 2^n) work.

## Lesson 3.6 — Recursive state and termination

Recursion needs a base case and a decreasing measure. To generate subsets, the measure is the number of unprocessed positions. Each call chooses whether to include the current item, then advances the index. The call depth is n, although the number of leaves is 2^n.

Tail recursion is not a guarantee of constant stack space in C++. For a long chain, use an explicit loop or stack where necessary.

**Worked recurrence:** Josephus with n people and step k has zero-based survivor `J(1)=0`, `J(n)=(J(n-1)+k) mod n`. After the first elimination, renumber the smaller circle and map the answer back by shifting k positions. Use iteration to compute n successive states in O(n) time and O(1) space.

## Exercises and answer key

1. Analyze nested loops where the inner index doubles up to n. **Answer:** O(n log n), if the outer loop visits n indexes.
2. Why is a modulo operation after multiplication not an overflow fix? **Answer:** The multiplication executes first.
3. Why start sieve marking at p²? **Answer:** Smaller multiples have already been marked by smaller prime factors.
4. Prove the sum loop from Module 1. **Answer:** The processed-prefix sum is the invariant; establish the three proof steps.
5. Convert recursive factorial to iteration. **Answer:** Multiply successive factors in a loop; also document a small enough input range to avoid overflow.

**Exit ticket:** For one recursive and one iterative solution, submit assumptions, correctness reasoning, runtime, auxiliary space, and one counterexample to a plausible incorrect approach.
