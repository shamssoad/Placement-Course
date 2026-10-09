# Volume 6 — Trees, heaps, tries, and dynamic range queries

[Contents](../TEXTBOOK.md) · [Introductory module](../06-trees-range-structures.md)

## Chapter 1. Choose what a subtree returns

A recursive tree solution is easiest when the returned value has a precise meaning. Height returns one number. Balance checking can return height plus a Boolean, or a failure sentinel. Diameter returns height upward while combining two child heights into a candidate path through the current node.

For maximum path sum, a parent can extend only one downward child branch, while the best complete path through a node may use both. Return `node.value + max(0,leftGain,rightGain)` upward and update a global best with `node.value + max(0,leftGain) + max(0,rightGain)`. Initialize the best so an all-negative nonempty tree returns its least-negative node rather than an empty path.

This distinction between “answer usable by parent” and “best answer anywhere below” appears in many tree DP problems. Combining them into one ambiguous variable causes incorrect branching paths.

## Chapter 2. Traversal without recursion

Preorder uses a stack of nodes, pushing right before left. Inorder repeatedly pushes the left chain, processes the top, then explores its right child. Postorder can use frames containing a node and a visited flag, or track the last processed child. An explicit stack makes depth storage visible but does not inherently reduce its asymptotic size.

Level-order traversal uses a queue. Recording the queue size at each level gives the current frontier count; process exactly that many nodes before moving to the next level. Do not let newly inserted children expand the current level accidentally.

## Chapter 3. LCA and repeated ancestor queries

For one ordinary binary-tree query with known-present targets, recursive LCA combines child results. For many queries on a fixed tree, preprocess depth and binary ancestors: `up[v][k]` is the 2^k-th ancestor. Lift the deeper node to equal depth, then lift both from large powers down while their ancestors differ; their parents then coincide at LCA.

Preprocessing takes O(n log n) space/time, queries O(log n). Root-parent conventions must be explicit. In a forest, also compare component IDs. For distance in edges, use `depth[u]+depth[v]-2*depth[lca]`.

## Chapter 4. Search trees and rotations

BST correctness is a global ordering property. Deletion with two children replaces the node's key with its inorder successor or predecessor, then removes that replacement from its old location. If values carry identity beyond the key, moving entire records or relinking nodes may be the appropriate design.

A right rotation around y with left child x makes x the parent, y its right child, and x's old right subtree y's new left subtree. The inorder sequence is unchanged, which proves ordering preservation. AVL trees rebalance using single/double rotations according to child-height imbalance. Red-black trees maintain coloring constraints that bound path-length ratios. Library ordered containers expose complexity guarantees; they do not require you to depend on a particular tree implementation.

## Chapter 5. Heap construction and top-k

Sift-down chooses the more extreme child and swaps until heap order is restored. Bottom-up build starts at the last internal node. Most nodes have small subtree height, so total sift work is O(n), unlike inserting n values individually in O(n log n).

For k-way merging, place each sorted stream's first item in a min-heap, repeatedly emit the minimum, and replace it with the next item from that stream. With N total items and k streams, time is O(N log k), heap storage O(k). Store source identity with each heap item to know where to advance.

## Chapter 6. Trie implementation model

Each trie node represents the prefix along its root path. A terminal flag means that exact prefix is a stored word. Children can be fixed slots for a small alphabet or sparse maps. A count of words passing through nodes enables prefix counts and carefully managed deletion.

For autocomplete, first follow the requested prefix, then traverse descendants in the required order. Returning the first k results still may require substantial traversal without additional ranking metadata. A trie lookup bound alone does not prove every autocomplete query is O(prefix length+k).

A bitwise trie for maximum XOR greedily selects the opposite high bit when available because that bit outweighs every lower bit combined. If queries impose an upper bound on eligible values, process values and queries offline by that bound while incrementally inserting candidates.

## Chapter 7. Segment trees as an algebra

A range structure stores an aggregate with an associative merge and an identity. Sum merges by addition, minimum by min, and a maximum-subarray structure can store total, best prefix, best suffix, and best internal sum.

For children L and R:

- total = L.total + R.total;
- prefix = max(L.prefix, L.total + R.prefix);
- suffix = max(R.suffix, R.total + L.suffix);
- best = max(L.best, R.best, L.suffix + R.prefix).

These equations enumerate whether an optimal segment lies entirely in one child or crosses the boundary. Define empty-range identity carefully, especially when nonempty answers are required.

Lazy propagation also needs an update-composition rule. For range addition and sum, delta changes aggregate by delta*length; two additions combine by addition. For assignment, a new assignment overwrites previous pending updates. For a general affine update `x -> ax+b`, compose in the actual application order, and transform sum as `a*sum+b*length`.

The [reference implementation](../examples/algorithm_lab.cpp) implements range addition with range sums. Follow [the trace](../WORKED-LABS.md) before attempting mixed assignment/addition.

## Chapter 8. Fenwick, sparse table, and compression

Fenwick trees are compact for invertible prefix aggregates such as sums with point additions. Segment trees support more general merge operations and updates. For a static range-minimum problem, a sparse table preprocesses power-of-two intervals and answers queries using two overlapping blocks because minimum is idempotent. The same overlapping trick does not work for sums, which would double-count.

Coordinate compression preserves order and equality by mapping sorted distinct values to ranks. It does not preserve gaps. For geometric lengths or area, retain original coordinate differences rather than treating every compressed neighbor gap as one unit.

## Exercises with answers

1. Why return one branch for maximum path gain? **Answer:** A path extended through the parent cannot branch into both children and still remain a simple path.
2. Why does a rotation preserve BST order? **Answer:** It rearranges parent-child links without changing inorder key sequence.
3. Why can sparse-table minimum use overlapping blocks? **Answer:** Repeated elements do not change a min, unlike a sum.
4. How does range assignment compose with earlier addition? **Answer:** The new assignment replaces the earlier value transformation for that segment.
5. Why retain original coordinates after compression? **Answer:** Rank distances do not equal geometric distances.

**Practical gate:** Implement one recursive tree aggregation, binary lifting, k-way merge, and a dynamic range structure. For each, document the state invariant and compare with a small brute-force baseline.
