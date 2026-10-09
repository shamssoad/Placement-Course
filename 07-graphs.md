# Module 7 — Graphs

**Expanded textbook:** [Graph modeling and algorithms](textbook/07-graphs.md). This file is the introductory/revision version.

Prerequisite: Module 6. Outcome: derive graph models and respect algorithm assumptions.

## Lesson 7.1 — Turn the problem into vertices and edges

Vertices represent states or entities; edges represent permitted transitions or relationships. A grid cell is a vertex when it can be visited, and legal moves produce edges. A prerequisite is a directed edge. A road may be directed or undirected, and its cost is a weight.

An adjacency list uses O(V+E) storage; a matrix uses O(V²). Lists suit sparse graphs. In an undirected list, store each edge in both directions. A disconnected graph needs traversal from every unvisited vertex when the question concerns all components.

Before coding, state whether parallel edges, self-loops, and isolated vertices are permitted. These choices affect cycle detection and low-link implementations.

## Lesson 7.2 — BFS finds shortest unweighted paths

BFS processes increasing edge-distance layers. A queue ensures that every distance-d vertex is processed before distance-(d+1) vertices. Mark a vertex when enqueuing it so multiple parents cannot enqueue it repeatedly.

```cpp
#include <queue>
#include <vector>
std::vector<int> bfsDistances(const std::vector<std::vector<int>>& graph,
                              int source) {
    std::vector<int> distance(graph.size(), -1);
    std::queue<int> pending;
    distance.at(source) = 0;
    pending.push(source);
    while (!pending.empty()) {
        int u = pending.front(); pending.pop();
        for (int v : graph[u]) {
            if (distance.at(v) != -1) continue;
            distance[v] = distance[u] + 1;
            pending.push(v);
        }
    }
    return distance;
}
```

Input endpoints must be valid. The sentinel -1 means unreachable. Add a parent array to reconstruct paths, following parents from the destination and reversing the sequence. Work is O(V+E) for the reachable portion with O(V) bookkeeping.

DFS explores a branch before returning. It is useful for structural questions and component exploration, but ordinary DFS does not find shortest unweighted paths. Deep graphs may require an explicit stack.

## Lesson 7.3 — Cycles, colors, and dependencies

For undirected simple graphs, encountering a visited neighbor other than the parent detects a cycle. With parallel edges, track the parent edge ID rather than skipping all edges to the parent vertex.

For directed graphs, distinguish unseen, active, and finished vertices. An edge to an active vertex forms a directed cycle. An edge to any visited vertex is not sufficient: the vertex may belong to a completed branch.

Kahn's topological algorithm counts incoming edges, enqueues zero-indegree vertices, and removes their outgoing edges. If it processes fewer than V vertices, a cycle prevents complete ordering. The queue may contain several legal next choices; a topological order need not be unique.

A bipartite graph can be colored with two colors so every edge crosses colors. BFS each component, assign the opposite color to neighbors, and reject a same-color edge. An odd cycle makes this impossible.

## Lesson 7.4 — Dijkstra and relaxation

Relaxation asks whether reaching v through u improves v's best known cost. Dijkstra repeatedly finalizes the smallest pending distance. With nonnegative edges, an unprocessed detour cannot later reduce a finalized distance; negative edges break that proof.

```cpp
#include <functional>
#include <limits>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>
using Edge = std::pair<int, long long>;
std::vector<long long> dijkstra(const std::vector<std::vector<Edge>>& g,
                                int source) {
    const long long INF = std::numeric_limits<long long>::max() / 4;
    for (const auto& edges : g)
        for (auto [v, weight] : edges) {
            if (v < 0 || v >= static_cast<int>(g.size()) || weight < 0 || weight >= INF)
                throw std::invalid_argument("Invalid Dijkstra edge");
        }
    std::vector<long long> distance(g.size(), INF);
    using State = std::pair<long long, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> heap;
    distance.at(source) = 0;
    heap.push({0, source});
    while (!heap.empty()) {
        auto [cost, u] = heap.top(); heap.pop();
        if (cost != distance[u]) continue; // Ignore an obsolete entry.
        for (auto [v, weight] : g[u]) {
            if (weight >= INF - cost) continue;
            long long candidate = cost + weight;
            if (candidate < distance[v]) {
                distance[v] = candidate;
                heap.push({candidate, v});
            }
        }
    }
    return distance;
}
```

This API represents only path costs strictly below INF. An INF result can mean unreachable or beyond that supported bound; choose a bound guaranteed sufficient by the problem constraints. The duplicate-entry heap version uses O((V+E) log(V+E)) time as a general bound and O(V+E) extra storage.

For edge weights only 0 or 1, a deque can put zero-cost improvements at the front and one-cost improvements at the back. Bellman–Ford relaxes all edges up to V-1 times; a further reachable improvement signals a reachable negative cycle. Floyd–Warshall considers each vertex as an allowed intermediate, using O(V³) time and O(V²) space.

## Lesson 7.5 — Disjoint sets and minimum spanning trees

DSU tracks components, not route lengths. Each component has a representative. Path compression shortens parent chains; union by size attaches the smaller tree under the larger one.

```cpp
#include <numeric>
#include <utility>
#include <vector>
class DSU {
    std::vector<int> parent, size;
public:
    explicit DSU(int n) : parent(n), size(n, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) std::swap(a, b);
        parent[b] = a; size[a] += size[b];
        return true;
    }
};
```

Assume nonnegative size and valid indexes. Kruskal sorts edges by weight and accepts an edge only if it joins two components. Its correctness follows the cut property: a lightest crossing edge can be included in some minimum spanning tree. Prim grows one tree by choosing its cheapest outgoing edge. For disconnected graphs, the appropriate result is a minimum spanning forest or an explicit “no spanning tree” outcome.

An MST minimizes total selected edge weight across connectivity; it does not guarantee shortest routes from a source.

## Lesson 7.6 — Advanced clinic: low links and components

In undirected DFS, `tin[u]` is discovery time and `low[u]` is the earliest discovery reachable through descendant paths and a back edge. A tree edge u-v is a bridge when `low[v] > tin[u]`: v's subtree has no alternative route to u or an ancestor. A nonroot u is an articulation point when some child v satisfies `low[v] >= tin[u]`; the root requires more than one DFS child.

Strongly connected components are directed vertex groups with mutual reachability. Kosaraju uses finish order and a transpose traversal; Tarjan uses an active stack and low-link values. Contracting components creates a DAG, useful for further dependency reasoning.

Flow is an optional extension: define capacity constraints and residual edges before applying augmenting paths. A backward residual edge means an earlier flow decision can be undone; omitting it can prevent optimality.

## Exercises and answers

1. Find the minimum moves in an unweighted maze. **Answer:** BFS from the start, ignoring blocked cells.
2. Detect impossible course prerequisites. **Answer:** Topological processing; fewer than V processed vertices means a cycle.
3. Does a negative edge always imply a negative cycle? **Answer:** No. Bellman–Ford can handle negative edges without negative cycles.
4. Are two vertices connected after many edge insertions? **Answer:** DSU supports incremental connectivity; ordinary DSU does not directly handle deletions.
5. Why discard stale Dijkstra entries? **Answer:** A smaller distance was already found; expanding the old state repeats unnecessary work.

**Exit ticket:** Draw the graph model before solving a word problem. State why BFS, Dijkstra, DSU, or an MST is the right abstraction.
