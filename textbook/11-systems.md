# Volume 11 — Operating systems, concurrency, and networks

[Contents](../TEXTBOOK.md) · [Introductory module](../11-os-networks.md)

## Chapter 1. What a process needs to execute

A process has an address space and operating-system-managed resources. A thread has execution state such as an instruction position, registers, and stack. Threads in the same process usually share memory, which makes communication inexpensive but requires synchronization.

A program requests protected services through system calls. User/kernel mode is a privilege distinction, not a statement that all kernel work belongs to a separate process. Interrupts and traps transfer control for hardware events and synchronous conditions. A context switch saves and restores execution state; its performance effects can include cache and translation disruption.

Processes communicate using mechanisms such as pipes, shared memory, sockets, and message facilities. Shared memory avoids copying some data but requires an agreed synchronization protocol. A communication channel does not automatically define message framing, ownership, or recovery.

## Chapter 2. Scheduling and metrics

Turnaround is completion minus arrival. Waiting is time spent ready but not executing under the simplified CPU-burst model. Response time is time until first service, not necessarily completion. Policies optimize different combinations of fairness, throughput, and latency.

Shortest-job-first minimizes average waiting in an idealized setting with known burst lengths, but real systems do not know future work perfectly. Round-robin limits monopolization; quantum choice balances responsiveness and switching overhead. Priority scheduling needs a starvation policy, such as aging, when low-priority work must eventually progress.

For jobs A=6, B=2, C=1 arriving together, FCFS A/B/C waiting times are 0,6,8, average 14/3. Ordering C/B/A gives 0,1,3, average 4/3. That benefit relies on the known-size simplified model; it is not a universal production scheduler prescription.

## Chapter 3. Virtual memory, paging, and caches

Virtual pages map to physical frames through page tables. A TLB caches translations. A page fault may load data, allocate a page, enforce copy-on-write, or reject access; it does not always mean a disk read.

Copy-on-write lets mappings initially share physical data until a write requires a private copy. Memory-mapped files connect file regions to address-space pages. Page replacement under pressure chooses what to evict, and locality strongly influences performance.

Internal fragmentation wastes space inside allocation units; external fragmentation leaves separated free regions that cannot satisfy a contiguous request. Paging changes which forms matter at different layers. Excessive paging can produce thrashing, where the system spends much of its effort moving pages rather than useful computation.

CPU caches exploit temporal and spatial locality. Two threads repeatedly writing separate variables on the same cache line can suffer false sharing. That is a performance issue even when the program has no data race.

## Chapter 4. The C++ concurrency contract

A conflicting unsynchronized access to a non-atomic object can create a data race and undefined behavior. “It worked in my run” is not evidence of a valid memory-order contract. Mutex unlock/lock synchronization and thread joining establish ordering relationships that make communication well-defined.

```cpp
#include <mutex>
#include <thread>
#include <vector>
// Fragment in a driver; count stays within int range.
int count=0;
std::mutex lock;
std::vector<std::thread> workers;
for (int t=0;t<4;++t) {
    workers.emplace_back([&] {
        for (int i=0;i<1000;++i) {
            std::lock_guard<std::mutex> guard(lock);
            ++count;
        }
    });
}
for (auto& worker:workers) worker.join();
// count is now 4000; all workers completed before observation.
```

This demonstration is inefficient for such tiny work because each increment locks; it teaches correctness. Production work often benefits from local accumulation and fewer shared updates. Thread-creation failure and exception-safe joining also need attention; the course's worker-pool reference handles partial construction cleanup.

## Chapter 5. Condition variables, deadlocks, and atomics

Wait on a predicate while holding the associated mutex. Waiting atomically releases that mutex and later reacquires it. Spurious wakes and other consumers mean a notification is not proof that an item remains available. Producer/consumer shutdown should wake both producers waiting for capacity and consumers waiting for work under a bounded-queue contract.

Deadlock's classic necessary conditions are mutual exclusion, hold-and-wait, lack of forced resource preemption, and circular wait. A fixed lock order breaks circular wait. `scoped_lock` can coordinate multiple mutex acquisition, but cannot repair arbitrary reentrant callback behavior or a task waiting for work that no free worker can run.

Atomics make supported operations indivisible and participate in the memory model. Sequential consistency offers a strong common starting model. Acquire/release can publish preceding writes through a synchronization relationship; relaxed operations do not provide that general publication ordering. Use the weakest order only when you can explain the proof. A single atomic flag does not automatically protect a multi-field invariant.

Futures transfer a result or exception. `packaged_task` wraps computation and connects it to a future; a promise explicitly fulfills shared state. `async` has launch-policy details, including possible deferred execution, so do not equate every async call with a new thread. The complete [worker-pool example](../examples/thread_pool.cpp) connects these concepts.

## Chapter 6. Filesystems and durable behavior

A filename maps through directory structures to stored metadata/data; an open handle can have a lifetime distinct from a pathname. Buffered writes can succeed locally before data is durable on storage. Closing, flushing language buffers, and requesting operating-system durability are distinct concerns.

Journaling and write-ahead mechanisms aim to recover consistent metadata or data according to their specific contracts. Applications still need a defined save protocol. Never infer crash-atomicity merely because one high-level output statement was used.

## Chapter 7. Network layers and addressing

Link-layer communication carries frames over a local link. IP carries packets across networks using addresses and routing. Transport protocols identify endpoints through ports and offer different delivery semantics. Application protocols define the meaning of exchanged messages.

DNS resolves names through a hierarchy and caching. An address can change; TTL influences caching but does not guarantee every observed answer updates simultaneously. Private addressing and NAT affect reachability. A server listening on a local loopback address is not automatically reachable from another machine.

Subnet prefixes identify network portions. For IPv4 /24, eight bits remain, giving 256 total addresses; common host-assignment rules reserve some, but special prefixes and contexts require their own treatment. Work in binary to understand the mask rather than memorizing only one formula.

## Chapter 8. TCP, UDP, and application framing

TCP establishes a connection, orders bytes, retransmits lost data, and manages flow/congestion behavior. It does not preserve sender write boundaries. A receive may return part of one message or several messages together. Zero bytes under the relevant stream API can indicate orderly closure; errors and nonblocking “try again” outcomes are separate cases.

UDP preserves datagram boundaries with different reliability/ordering guarantees. Applications that need retransmission, deduplication, or ordering must design it or choose a protocol that supplies it.

For a length-prefixed protocol, parse a fixed-size header, validate the advertised length before allocation, then collect exactly that payload. Retain extra bytes for the next message. Test every split point, multiple coalesced messages, early closure, and oversized lengths. This parser can be tested independently of platform-specific socket setup.

## Chapter 9. HTTP, TLS, and distributed failure

HTTP methods express request semantics, status codes summarize outcomes, and headers/content carry metadata and representation. GET is intended for retrieval; PUT's intended effect is idempotent; a POST operation may require an application idempotency key for safe retries. Idempotent does not mean the response must be byte-identical or that logging has no side effects; repeated intended state effects are the central idea.

TLS authenticates peers under configured trust rules and protects data in transit. Encryption does not make an application request authorized or validate its business logic. Validate certificates and hostname identity when using a TLS client library.

A timeout leaves uncertainty: the request may never have arrived, may still be processing, or may have committed with a lost response. Retries need bounded attempts, backoff, and operation identity where duplication matters. Caching adds freshness and invalidation questions. Load balancing distributes traffic but introduces no automatic guarantee that session state is shared consistently.

## Exercises with answers

1. Is every page fault a disk access? **Answer:** No; allocation, copy-on-write, and invalid mapping cases also exist.
2. Why can four threads be slower than one? **Answer:** Coordination, contention, cache effects, and serial dependencies can outweigh parallel work.
3. Why must a condition-variable waiter recheck state? **Answer:** Wakes can be spurious and another thread may consume the condition first.
4. Why use an operation ID after a timed-out booking? **Answer:** It identifies a potentially already committed request and prevents accidental duplicate creation.
5. Why separate framing from sockets? **Answer:** The protocol state machine becomes deterministic and easy to test with arbitrary byte chunking.

**Practical gate:** Implement the worker queue and framing parser, then explain one scheduling calculation, one page-fault path, one synchronization proof, and one uncertain network outcome.
