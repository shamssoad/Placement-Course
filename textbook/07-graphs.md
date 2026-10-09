# Volume 7 — Graph modeling and algorithms

[Contents](../TEXTBOOK.md) · [Introductory module](../07-graphs.md)

## Chapter 1. The state graph may be larger than the map

A vertex should contain everything that affects future legal moves. In a maze with keys, position alone is insufficient: reaching the same cell with a key differs from reaching it without one. A state `(row,column,keyMask)` preserves that information. The graph's size is cells times possible masks, which can be exponential in the number of key types.

Edges represent legal transitions, not merely visual adjacency. A teleport, prerequisite, currency conversion, or word transformation can all be edges under different contracts. First define directedness, weights, source/target conditions, and whether states can repeat.

Represent sparse graphs with adjacency lists and dense all-pairs problems with matrices when justified. For multigraphs, give each edge an identity; two edges between the same vertices matter in bridge detection and path counting.

## Chapter 2. BFS layers and multi-source propagation

BFS discovers vertices in nondecreasing number of edges from its initial frontier. Enqueue all sources at distance zero for a multi-source problem such as nearest facility. The first discovery distance is minimal because all earlier layers were already explored.

For a grid with obstacles, verify bounds and obstacle status before enqueuing. Mark on enqueue, not dequeue, to avoid duplicate pending copies. A parent array reconstructs one path; counting shortest paths additionally sums ways from predecessors at the same optimal layer. Counts need a bound or modulus.

Bidirectional BFS can reduce explored states when both ends and reverse moves are well-defined. Expand complete layers and use a correct stopping condition; stopping at the first arbitrary frontier contact can be wrong under inconsistent expansion accounting.

## Chapter 3. DFS states and dependency order

DFS records entry and exit. In directed graphs, an edge to an active recursion ancestor identifies a cycle. Finished vertices do not imply a cycle merely because they are visited. In undirected traversal, skip the exact parent edge, not every edge to the parent vertex when parallel edges are possible.

Topological order exists only for a DAG. Kahn's algorithm processes zero-indegree vertices; DFS can produce reverse finishing order after verifying no directed cycle. Once topologically ordered, a DAG supports shortest or longest path DP even with negative edges, because no cycle creates repeated improvement dependencies.

For a longest prerequisite chain, `length[v]=max(length[v],length[u]+1)` for each u->v in topological order. Define whether length counts tasks or edges and initialize accordingly.

## Chapter 4. Shortest-path algorithms and assumptions

| Edge model | Method | Reason |
|---|---|---|
| Equal unit cost | BFS | Layers correspond to cost |
| Costs only 0 or 1 | 0–1 BFS | Deque maintains two adjacent cost levels |
| Nonnegative costs | Dijkstra | Minimum pending distance can be finalized |
| Negative edges, no relevant negative cycle | Bellman–Ford | Repeated relaxation propagates path improvements |
| DAG | Topological relaxation | Dependencies have an acyclic order |
| Small dense all-pairs problem | Floyd–Warshall | Systematically adds allowed intermediate vertices |

For Bellman–Ford, after k full passes, shortest paths using at most k edges have been accounted for. Any simple path has at most V-1 edges. A reachable improvement on an additional pass reveals a reachable negative cycle affecting some reachable outcomes. Not every vertex necessarily has an unbounded-below distance; propagate from affected vertices to identify which answers are invalid.

Floyd–Warshall uses `d[i][j]=min(d[i][j],d[i][k]+d[k][j])` with k as the outer phase. Check reachability before adding infinity sentinels. A negative diagonal afterward signals a negative cycle; determine which source-target pairs can reach and leave it before claiming every answer is affected.

## Chapter 5. Dijkstra trace

Edges: S->A cost 4, S->B cost 1, B->A cost 2, A->T cost 1, B->T cost 7. Begin S=0 and others infinity. Expanding S proposes A=4, B=1. Expanding B proposes A=3 and T=8. Expanding A at 3 proposes T=4. The old A=4 heap entry is stale; skip it. T's final answer is 4 through S,B,A,T.

The priority queue may contain several entries for one vertex in a simple implementation. The distance table is authoritative. Checking `savedDistance != currentDistance` avoids expanding obsolete states. Reconstruct by storing the predecessor whenever a strict improvement occurs.

The [introductory module](../07-graphs.md) provides complete BFS, Dijkstra, and DSU code. Use its numeric contract rather than adding arbitrary weights to an infinity sentinel.

## Chapter 6. Spanning trees and disjoint sets

An MST connects all vertices with minimum total edge weight, using V-1 edges in a connected undirected graph. It does not minimize every route distance. The cut property says a lightest edge crossing a cut can belong to an MST; exchange it with the crossing edge of a candidate tree to justify greedy selection.

Kruskal sorts edges and uses DSU to reject cycle-forming connections. Prim maintains the cheapest way to attach an outside vertex to the growing tree. Negative edge weights do not invalidate these MST algorithms, unlike Dijkstra's shortest-path proof.

DSU supports merge-only connectivity. Path compression and union by size/rank give very small amortized operation costs, conventionally expressed through the inverse Ackermann function. It does not retain explicit paths and cannot directly split a component after deleting an edge. Offline reversal or rollback techniques solve some deletion-related variants with additional design.

## Chapter 7. Low-link values and strongly connected components

For an undirected DFS tree, low[u] records the earliest discovered ancestor reachable from u's subtree using tree edges and a back edge. A child subtree with `low[child] > tin[parent]` cannot escape above its parent edge, so that edge is a bridge. Articulation conditions use `>=` and a separate root-child count rule.

In directed graphs, strongly connected components group mutually reachable vertices. Kosaraju performs a finish-order traversal, reverses edges, and traverses in reverse finishing order. Contracting each SCC creates a DAG. This can simplify “which groups can eventually reach which others?” problems before applying DAG reasoning.

## Chapter 8. Flow and matching extension

Flow assigns amounts to directed edges without exceeding capacities and conserves flow at nonterminal vertices. A residual graph records how much more can be sent forward and how much existing flow can be undone backward. Omitting reverse residual edges prevents revising early decisions.

An augmenting path carries the minimum residual capacity along it, the bottleneck. Updating forward and reverse residual capacities preserves feasible flow. When no augmenting path exists, the residual reachable set defines a cut whose capacity equals the flow under the max-flow/min-cut theorem. BFS augmenting paths give Edmonds–Karp's polynomial bound; more advanced algorithms improve practical performance.

Bipartite matching reduces to unit-capacity flow from source to left vertices, across allowed pairs, then right vertices to sink. A matched pair consumes one unit. Distinguish maximum matching from a maximal matching that merely cannot be extended by one obvious local edge.

## Exercises with answers

1. A maze has a one-use wall break. What is the state? **Answer:** Position plus whether the break has been used.
2. Is every visited directed neighbor a cycle? **Answer:** No; an active-stack neighbor indicates a back edge, while a finished neighbor can be harmless.
3. Can MST handle negative weights? **Answer:** Yes; cut/exchange reasoning still applies.
4. Why add reverse residual edges? **Answer:** To undo and reroute earlier flow choices.
5. What distinguishes unreachable from negatively unbounded? **Answer:** The first has no path; the second has reachable paths with arbitrarily decreasing cost through a relevant negative cycle.

**Practical gate:** Model five word problems before choosing algorithms. For two, explain a plausible but invalid algorithm and give a counterexample that violates its assumptions.
