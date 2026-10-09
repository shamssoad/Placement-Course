# Module 5 — Strings and linear data structures

**Expanded textbook:** [Strings, lists, stacks, queues, and deques](textbook/05-linear-structures.md). This file is the introductory/revision version.

Prerequisite: Module 4. Outcome: maintain textual and structural invariants.

## Lesson 5.1 — Text representation and frequency

For lowercase English letters, an array of 26 counts is enough. For arbitrary byte values, use a wider table and convert `char` to `unsigned char` before indexing, because plain char can be signed. UTF-8 text needs a different understanding: one user-perceived character may span multiple bytes or code points. Interview solutions should state their alphabet assumption.

Anagrams have equal symbol counts. Palindromes compare mirrored positions. A substring is contiguous; a subsequence may skip positions. Repeatedly constructing substrings inside nested loops can add hidden copying costs. Use indexes when ownership of a new substring is unnecessary.

**Exercise:** Check whether a lowercase string's letters can be rearranged into a palindrome. **Answer:** At most one letter count may be odd. This is about rearrangement, not whether the original string is a palindrome.

## Lesson 5.2 — KMP from reusable prefix information

When matching fails after several characters, naive matching restarts from scratch. KMP remembers the longest proper prefix of the matched text that is also a suffix. That suffix can become the start of the next candidate without rechecking known matches.

For a pattern `ababaca`, the prefix-function values are `0,0,1,2,3,0,1`. At each index, try extending the preceding border; if it fails, jump to the border's own shorter border.

```cpp
#include <string>
#include <vector>
std::vector<int> prefixFunction(const std::string& pattern) {
    std::vector<int> pi(pattern.size(), 0);
    for (int i = 1; i < static_cast<int>(pattern.size()); ++i) {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[j]) j = pi[j - 1];
        if (pattern[i] == pattern[j]) ++j;
        pi[i] = j;
    }
    return pi;
}
std::vector<int> findMatches(const std::string& text,
                             const std::string& pattern) {
    if (pattern.empty()) return {0}; // Explicit API convention.
    auto pi = prefixFunction(pattern);
    std::vector<int> matches;
    int j = 0;
    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        while (j > 0 && text[i] != pattern[j]) j = pi[j - 1];
        if (text[i] == pattern[j]) ++j;
        if (j == static_cast<int>(pattern.size())) {
            matches.push_back(i - j + 1);
            j = pi[j - 1]; // Keep overlapping matches possible.
        }
    }
    return matches;
}
```

Assume lengths fit `int`. The scan is linear because j advances at most once per text character and every fallback decreases it. Total work is O(n+m); output storage is additional to the O(m) prefix table.

Rolling hashes summarize substrings with polynomial arithmetic. Equal strings have equal hashes, but equal hashes do not prove equal strings. Verify candidate matches for deterministic correctness or clearly state a probabilistic collision model. The Z-function is an alternative: it stores how much of the whole string's prefix matches from each position.

## Lesson 5.3 — Linked lists and pointer order

A singly linked node stores a value and a next pointer. Removing a node requires changing the pointer that leads to it. A dummy node makes head removal look like every other removal. A doubly linked list stores a previous pointer too; update both directions. A circular list does not end at null, so traversal needs an explicit stopping condition.

```cpp
struct Node { int value; Node* next = nullptr; };
// Rewires existing nodes. It neither allocates nor deletes them.
Node* reverse(Node* head) {
    Node* previous = nullptr;
    while (head != nullptr) {
        Node* next = head->next;
        head->next = previous;
        previous = head;
        head = next;
    }
    return previous;
}
```

Save the next pointer before changing the link. Otherwise the unprocessed suffix may become inaccessible. Ownership is external in this teaching representation: the caller must arrange cleanup, and must not double-delete nodes after rewiring.

Floyd's cycle detection advances one pointer one step and another two steps. Inside a cycle their relative position changes by one per iteration, so they eventually meet. To locate the entry, reset one pointer to the head and advance both by one until they meet again. State the distance equations to justify this second phase rather than guessing it.

## Lesson 5.4 — Monotonic stacks

For next-greater-element queries, scan left to right and keep indexes whose answer is unresolved. If the current value is greater than the value at the stack top, it resolves that index; pop and continue. Every index is pushed and popped at most once, so the total work is O(n), despite the nested-looking loop.

```cpp
#include <vector>
std::vector<int> nextGreaterIndex(const std::vector<int>& a) {
    std::vector<int> answer(a.size(), -1), pending;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        while (!pending.empty() && a[pending.back()] < a[i]) {
            answer[pending.back()] = i;
            pending.pop_back();
        }
        pending.push_back(i);
    }
    return answer;
}
```

Strict versus nonstrict comparison depends on whether equal values qualify. For histogram area, each bar needs the extent over which no smaller bar blocks its height. Equal-height handling must be consistent to avoid duplicate counting mistakes.

A minimum stack can store each pushed value with the minimum at that depth. Pop restores the preceding minimum automatically. Balanced-delimiter checking needs matching delimiter types, not only equal total counts.

## Lesson 5.5 — Queues and deques

A circular queue uses an array and wraps indexes modulo capacity. Track a count or reserve an unused slot to distinguish empty from full. Specify overflow behavior instead of silently overwriting data.

A queue built from two stacks puts new items on an input stack and transfers them to an output stack only when the output is empty. Each item moves a bounded number of times, giving amortized O(1) queue operations.

For sliding-window maximum, maintain a deque of candidate indexes. Remove expired indexes from the front. Remove dominated values from the back before adding the new index. A newer value at least as large survives longer and is never worse than the old candidate, so the old one is safe to discard. The front then holds the window maximum.

## Exercises and answers

1. Find matches of `aaa` in `aaaaa`. **Answer:** Starts 0, 1, and 2; preserve overlapping borders after a match.
2. Reverse an empty list. **Answer:** Return null with no special allocation.
3. Implement stack-based delimiter checking. **Answer:** Push opening types; require a matching top for each closer; require an empty stack at the end.
4. Why store indexes in the maximum deque? **Answer:** Expiry depends on position, and indexes also allow value lookup.
5. Merge two sorted lists without allocating nodes. **Answer:** Maintain a result tail and attach the smaller current node, then attach the remaining suffix. Preserve external ownership.

**Exit ticket:** Trace the KMP fallback and list reversal on paper. Solve one monotonic-stack question and explain why each discarded candidate is unnecessary.
