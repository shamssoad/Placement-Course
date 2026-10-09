# Module 6 — Trees, heaps, tries, and range structures

**Expanded textbook:** [Trees and dynamic range queries](textbook/06-trees.md). This file is the introductory/revision version.

Prerequisite: Modules 3–5. Outcome: select and maintain hierarchical invariants.

## Lesson 6.1 — Traversal is a choice of information order

A tree has one simple path between each pair of connected nodes. A rooted tree gives each node a parent and children. In preorder, process a node before its children; in postorder, process children before the node; in binary-tree inorder, process left, node, right. Choose postorder when a parent's answer depends on child answers.

```cpp
#include <algorithm>
struct TreeNode {
    int value;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};
// Height counts nodes; an empty tree has height zero.
int height(const TreeNode* node) {
    if (!node) return 0;
    return 1 + std::max(height(node->left), height(node->right));
}
```

There is one visit per node: O(n) time. Active recursion depth is O(h), where h can equal n in a chain. Breadth-first traversal instead uses a queue; its memory is proportional to the widest level.

For diameter measured in edges, the longest path through a node is `leftHeight + rightHeight`. Return the height upward and update one shared best diameter. Recomputing height separately at every node can make a chain take O(n²).

**Lab:** Implement iterative preorder with a stack. Push right before left so the left subtree is processed first. Explain how your explicit stack replaces recursion frames.

## Lesson 6.2 — LCA, reconstruction, and serialization

In a binary tree, an LCA routine returns a matching node or combines results from both subtrees. If both sides find a target, the current node is their lowest common ancestor. A simple version assumes both target nodes actually exist; an API that cannot assume this must track existence too.

Reconstruction from preorder and inorder uses the next preorder value as root and its inorder position to split the subtrees. Unique values allow a value-to-index map. Duplicate values require extra disambiguation; the traversals may not uniquely identify a tree.

Serialization must preserve missing-child structure. Preorder values without null markers cannot distinguish several different trees. Store explicit null tokens, then consume them recursively in the same order. Malformed input should fail rather than build a partial tree silently.

## Lesson 6.3 — BST invariants

A strict binary search tree requires every left-subtree value to be smaller and every right-subtree value to be larger. Checking only immediate children is insufficient. Validate with bounds inherited from all ancestors.

Search follows one child per level. Deleting a node has three cases: no children, one child, or two children. In the two-child case, replace its value with its inorder successor and delete that successor from the right subtree. Specify the policy for duplicate keys.

Costs are O(h), not automatically O(log n). Sorted insertion into an unbalanced BST produces a chain. Balanced trees use rotations to change shape while preserving inorder order. AVL trees track height balance; red-black trees use coloring invariants to bound height. Learn rotations and guarantees even when using a library ordered map in practical code.

## Lesson 6.4 — Heaps and streaming decisions

A binary heap is a complete tree stored in an array. For zero-based index i, children are `2i+1` and `2i+2`. Its order invariant relates parents to children; it does not sort every pair of array positions.

Bottom-up heap construction is O(n): most nodes are near the leaves and can move only a short distance. Repeated insertion gives O(n log n), so these are different construction methods.

For the largest k values, keep a min-heap of size k: its top is the weakest retained candidate. For a streaming median, maintain a max-heap for the lower half and a min-heap for the upper half. Keep sizes within one and ensure every lower value is no larger than every upper value. Insert by value, then rebalance. Compute an even-count median without overflowing an integer sum.

## Lesson 6.5 — Tries as shared prefixes

A trie follows one edge per symbol. Each node represents a prefix; an end marker distinguishes a complete word from a mere prefix. For a lowercase alphabet, 26 child slots are simple but can waste memory. Sparse maps reduce unused edges at the cost of lookup and allocation overhead.

To delete `cat` when `car` exists, clear the terminal marker and remove only nodes with no children and no terminal word. Shared prefix nodes must survive. Prefix lookup costs O(length), independent of the total number of words except through representation costs.

A bitwise trie stores bits from most significant to least. To maximize XOR, prefer the opposite bit at each level; a higher bit dominates any combination of lower bits. State the unsigned width and whether the candidate set can be empty.

## Lesson 6.6 — Fenwick trees

A Fenwick tree stores selected suffixes of prefixes. In one-based indexing, `i & -i` identifies the size of the block represented by index i. To query a prefix, subtract this block length repeatedly. To update a point, add the block length repeatedly to reach every aggregate containing it.

```cpp
#include <stdexcept>
#include <vector>
class Fenwick {
    std::vector<long long> bit;
public:
    explicit Fenwick(int n) : bit(n >= 0 ? n + 1 : 1, 0) {
        if (n < 0) throw std::invalid_argument("Negative size");
    }
    void add(int index, long long delta) {
        int n = static_cast<int>(bit.size()) - 1;
        if (index < 0 || index >= n) throw std::out_of_range("index");
        for (int i = index + 1; i <= n; i += i & -i) bit[i] += delta;
    }
    long long prefix(int end) const { // Sum over [0,end).
        int n = static_cast<int>(bit.size()) - 1;
        if (end < 0 || end > n) throw std::out_of_range("end");
        long long result = 0;
        for (int i = end; i > 0; i -= i & -i) result += bit[i];
        return result;
    }
    long long range(int left, int right) const {
        if (left > right) throw std::invalid_argument("Reversed range");
        return prefix(right) - prefix(left);
    }
};
```

Updates and queries are O(log n), storage O(n). This version stores additive deltas. To assign a new value, calculate the difference from its old value. Sizes and accumulated values must remain within the chosen numeric types.

## Lesson 6.7 — Segment trees and lazy propagation

A segment tree partitions a range in half until reaching individual positions. Each node stores a merged answer for its interval. A sum node merges by addition with identity zero; a minimum node merges by minimum with an infinity identity. The merge operation must be associative, so grouping does not change the answer.

A query returns the identity for no overlap, the stored aggregate for full overlap, and merged child results for partial overlap. Point updates modify the leaf and recompute ancestors. Both take O(log n).

Lazy propagation postpones work for range updates. For a range addition d on a sum node of length L, increase its sum by `d*L` and its pending addition by d. Before descending, push that pending addition to the children and clear it locally. A pending assignment composes differently from an addition: a later assignment replaces earlier assignments and additions. Implementing both needs an explicit composition rule.

Coordinate compression sorts distinct values and replaces each by its rank. It preserves ordering, not distances: compressed difference 1 does not mean original difference 1.

## Exercises and answers

1. Validate a BST where a left grandchild exceeds the root. **Answer:** Propagated upper bounds reject it; checking local parent-child relationships does not.
2. Find kth smallest in a BST. **Answer:** Inorder traversal with a counter; stop at k. For frequent queries, store subtree sizes.
3. Update position 2 by +4, then query `[1,4)` in a Fenwick tree. **Answer:** Add the delta through responsible blocks; the range answer rises by 4.
4. Why cannot range-add simply add d to a sum node? **Answer:** Every one of L elements changes, so the aggregate changes by dL.
5. Serialize a one-node tree. **Answer:** Include its value and both missing-child markers.

**Exit ticket:** Implement diameter, a Fenwick tree driver, and a range-sum segment tree. Compare range answers with direct summation on small random arrays.
