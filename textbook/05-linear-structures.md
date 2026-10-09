# Volume 5 — Strings and linear data structures

[Contents](../TEXTBOOK.md) · [Introductory module](../05-strings-linear-structures.md)

## Chapter 1. Separate representation from matching rules

Before processing text, define the alphabet and comparison rule. Case-sensitive ASCII, case-folded Unicode, and byte sequences are different contracts. Lowercasing bytes is not a full Unicode normalization strategy. In this course's algorithm exercises, use the stated ASCII/lowercase alphabet unless otherwise specified.

For a palindrome check, compare the first and last relevant symbols and move inward. For a longest palindromic substring, expanding around each character and each gap takes O(n²) time and O(1) extra storage. A subsequence palindrome is a different problem because characters may be skipped; it commonly uses interval DP.

String building can have hidden cost. Repeatedly prepending to a string shifts existing content. Accumulate pieces and append, or build backward and reverse when the problem allows it. Reserve can reduce reallocations but must not become a substitute for complexity reasoning.

## Chapter 2. Derive KMP's failure function

For pattern `ababaca`, suppose the current candidate matched `ababa` and the next symbol fails. The matched prefix ends with `aba`, itself a prefix. Trying that border avoids rechecking known symbols. If it also fails, follow the border of `aba`, namely `a`, and continue.

The prefix function pi[i] stores the longest proper prefix of pattern `[0,i]` that is also a suffix. At i, begin with `j=pi[i-1]`; while the next characters disagree and j>0, set `j=pi[j-1]`. If they agree, increment j. Because advances and retreats are bounded across the scan, construction is linear.

The original module contains complete KMP code. Trace it on pattern `aaa` in text `aaaaa`: after finding one match, retaining border length two permits matches at the next two positions. Resetting to zero would lose overlap efficiency and, in some incorrectly advanced implementations, skip valid answers.

The Z-function instead stores the length of the prefix matching at each starting index. Maintain a known matching interval and reuse its information; verify only beyond the known boundary. Both algorithms avoid repeated comparisons by recording previous structural knowledge.

## Chapter 3. Rolling hashes and collision discipline

A polynomial hash interprets symbols as coefficients under a base and modulus. Prefix hashes and precomputed powers permit constant-time substring-hash calculation after preprocessing. Equal substrings must produce equal hashes under identical conventions, but collisions mean the converse is not guaranteed.

Rabin–Karp scans candidate windows using hashes and verifies matching candidates for deterministic correctness. Many collisions can worsen runtime. Multiple moduli reduce accidental collision probability but do not transform a hash into a mathematical proof of equality. Always state whether a solution is deterministic or probabilistic.

## Chapter 4. Linked-list operations as link ownership

Insertion after a node sets the new node's next pointer to the old successor before redirecting the predecessor. Deletion first redirects the predecessor, then disposes of the removed node according to the ownership contract. Reading a deleted node's next pointer afterward is a use-after-free error.

A dummy head unifies head and interior operations. In merging sorted lists, maintain a tail pointer and append the smaller current node, then the remaining suffix. No new data nodes are needed, but the two original list owners must transfer rather than both retain deletion responsibility.

For group reversal, first verify k nodes remain. Reverse exactly that group, reconnect its old head as the new tail, and continue. Failing to preserve the next group's start can lose the unprocessed suffix. A doubly linked list also needs backward links updated; a circular list needs a termination condition other than null.

## Chapter 5. Fast/slow pointers and cycles

Let the distance from head to cycle entry be μ, and the cycle length λ. When slow and fast meet after t slow steps, their traveled-distance difference t is a multiple of λ. Moving one pointer from head and one from the meeting point at equal speed causes them to meet at the entry after μ steps, because their cycle offsets then agree modulo λ.

For middle finding, different initialization and stopping rules choose the first or second middle of an even-length list. Define that requirement instead of debugging by intuition. Finding the kth node from the end uses a lead gap of k and then advances both pointers together.

## Chapter 6. Stacks: unresolved work

A stack represents nested or last-started work. Parentheses parsing pushes opening types and requires a matching closer. A count alone cannot distinguish `([)]` from a valid nesting. Expression evaluation uses stacks to respect operator precedence and associativity; subtraction and division require preserving operand order when popping.

In a histogram, keep increasing-height bar indexes. When a lower bar appears, each popped bar's maximal right boundary is known. Its left boundary is after the new stack top, or zero if none. Area is height times width; use a wide type.

```cpp
#include <algorithm>
#include <vector>
long long largestRectangle(const std::vector<int>& h) {
    // Preconditions: nonnegative heights; size fits int; area fits long long.
    std::vector<int> pending;
    long long best = 0;
    for (int i=0; i<=static_cast<int>(h.size()); ++i) {
        int current = i==static_cast<int>(h.size()) ? 0 : h[i];
        while (!pending.empty() && h[pending.back()] > current) {
            int index=pending.back(); pending.pop_back();
            int left=pending.empty() ? 0 : pending.back()+1;
            best=std::max(best, 1LL*h[index]*(i-left));
        }
        if (i<static_cast<int>(h.size())) pending.push_back(i);
    }
    return best;
}
```

The zero-height sentinel flushes positive bars without reading beyond the input. Equal heights remain until a smaller value; the widest relevant rectangle is still considered. Each index is pushed/popped once.

## Chapter 7. Queues and amortized structures

A queue serves oldest pending work first. Two-stack implementation reverses the insertion stack only when the removal stack is empty. Every element transfers once, giving amortized constant cost. Transferring on every pop destroys this benefit.

A monotonic deque keeps only candidates that could win a future window. Expiry removes obsolete positions from the front; domination removes worse, older values from the back. The worked-lab file contains a complete implementation and trace. For a circular buffer, distinguish full and empty using a count or an intentionally unused slot; both require a clear capacity definition.

## Exercises with answers

1. Why can palindrome expansion start at gaps? **Answer:** Even-length palindromes have no single central character.
2. Can rolling-hash equality alone certify exact text equality? **Answer:** No, unless the problem explicitly accepts a probabilistic assumption.
3. Why preserve a node's successor before reversal? **Answer:** Redirecting its link otherwise loses the remainder's location.
4. What makes histogram processing linear? **Answer:** Each bar enters and leaves the stack once despite nested loops.
5. How do you distinguish a full circular buffer from an empty one? **Answer:** Track count or reserve a slot; equal read/write indexes alone are ambiguous.

**Practical gate:** Implement exact pattern search, list group reversal, histogram area, and a circular queue. Include overlapping matches, partial groups, equal heights, and wraparound tests.
