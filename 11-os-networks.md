# Module 11 — Operating systems and networks

**Expanded textbook:** [Operating systems, concurrency, and networking](textbook/11-systems.md). This file is the introductory/revision version.

Prerequisite: C++ foundations. Outcome: explain what surrounds a running application.

## Lesson 11.1 — Processes, threads, and scheduling

A process provides an execution and resource boundary, including a virtual address space. Threads in one process normally share that address space but have their own execution state and stacks. This makes communication convenient and accidental data races possible.

User mode restricts direct access to protected resources. A system call requests operating-system work, such as file access. A context switch saves one execution context and restores another; its cost includes more than merely choosing a new thread because caches and address-translation state can be affected.

Scheduling balances responsiveness, throughput, fairness, and overhead. First-come-first-served can make short jobs wait behind long ones. Round-robin gives turns but a tiny quantum raises switching overhead. Priority scheduling can starve low-priority work unless the policy addresses it.

**Worked exercise:** Jobs A and B arrive at time 0 with durations 5 and 2. FCFS in A-then-B order gives waiting times 0 and 5, average 2.5. B-then-A gives 0 and 2, average 1. Different objectives can prefer different policies.

## Lesson 11.2 — Virtual memory and locality

A virtual address is translated to a physical location through page mappings. The TLB caches recent translations. A page fault indicates that normal translation cannot complete; it may require loading a page, establishing a mapping, or rejecting an invalid access.

Virtual memory is not unlimited free RAM. Memory pressure can trigger paging and large latency changes. Sequentially scanning a vector tends to use caches effectively because nearby elements are fetched together. Following scattered linked nodes can cause more cache misses even when big-O traversal costs match.

The stack/heap distinction is a useful implementation model, but C++ lifetime rules are the language-level contract. An object with dynamic storage can still be owned by an automatic RAII wrapper.

## Lesson 11.3 — Races, locks, and happens-before

`++counter` is not one indivisible operation: it reads, computes, and writes. Two threads can overwrite one another's increments. In C++, conflicting unsynchronized non-atomic accesses constitute a data race and cause undefined behavior, beyond simply producing an occasional wrong count.

```cpp
#include <mutex>
class Counter {
    mutable std::mutex mutex_;
    int value_ = 0;
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
    }
    int value() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }
};
```

The mutex protects both writes and reads. Scoped locking releases the lock even when an exception leaves the function. Joining worker threads establishes that their execution has completed before subsequent observation. Ensure a thread is joined or otherwise deliberately managed before its `std::thread` wrapper is destroyed.

Atomics provide defined concurrent access for a single atomic object. They do not automatically protect an invariant spanning several variables. Relaxed ordering can be sufficient for independent statistics but is not a universal replacement for lock-based coordination.

## Lesson 11.4 — Condition variables and shutdown

A condition variable is used with a predicate protected by a mutex. Waiting releases the mutex and later reacquires it. Use `wait(lock, predicate)` or a predicate loop because wakes can be spurious and a different thread may consume the available work first.

For a producer/consumer queue, the predicate is “work exists or shutdown was requested.” A notified consumer checks it under the lock. Decide whether shutdown drains accepted tasks or cancels them. If draining, the consumer exits only when shutdown is true and the queue is empty.

Deadlock can arise when two threads acquire two locks in opposite orders. Necessary conditions include mutual exclusion, hold-and-wait, no forced preemption of resources, and circular wait. Use a consistent lock order or coordinated multi-lock acquisition where appropriate. Avoid calling arbitrary callbacks while holding a lock: they can acquire other locks or reenter your object.

The project folder contains a complete worker-pool example for this lesson.

## Lesson 11.5 — From hostname to remote service

An application first needs an address for a hostname, often through DNS and caches. It then contacts an IP address and port. IP provides packet addressing and routing; transport protocols add different communication semantics.

TCP provides a reliable ordered byte stream. It does not preserve application message boundaries. A sender writing 100 bytes once does not imply the receiver reads exactly 100 bytes once. The receiver must accumulate bytes until its framing rule says a message is complete.

UDP sends datagrams, with different reliability and ordering guarantees. Applications using it must decide how to handle loss, reordering, and duplication if those matter.

Subnetting divides an address into network and host portions. For ordinary IPv4 networks, a /24 leaves eight host bits and 256 total addresses; address usability depends on network conventions and context, so avoid blindly applying one host-count rule to every prefix.

## Lesson 11.6 — HTTP, TLS, and failure

HTTP describes request/response semantics: method, target, headers, status, and content. TCP is one transport used by HTTP versions; newer protocol stacks can use other transport arrangements. TLS provides encrypted and authenticated communication when configured and validated properly.

A timeout does not prove that the server performed no action. A booking request might commit, then its response could be lost. Retrying safely requires an idempotency key or a queryable operation identity. GET-like reads and state-changing writes need different retry reasoning.

Latency is delay; bandwidth is transfer capacity. Sending one tiny request many times can be dominated by latency rather than bandwidth. Connection reuse and batching can help, but their effects should be measured.

## Lesson 11.7 — Message-framing lab

Define a protocol: four bytes encode an unsigned big-endian payload length, followed by that many bytes of payload. Reject payload lengths over a documented limit, such as 64 KiB. The receiver accumulates partial headers and partial payloads. It may also receive two whole messages in one read.

Socket calls and setup are platform-specific; choose the Windows or POSIX API for your environment. Separate the portable framing parser from those calls. Test the parser with every possible split point in a short message before using a live socket.

## Exercises and answers

1. Why can a read-only accessor still require a lock? **Answer:** It may run concurrently with a write to the same ordinary data.
2. Why wait on a predicate? **Answer:** Notifications are not persistent stored state, and wakes do not guarantee the needed state holds.
3. Why is one TCP read not one message? **Answer:** TCP exposes a byte stream, not application record boundaries.
4. Is more threading always faster? **Answer:** No; contention, overhead, dependencies, and limited parallel hardware can dominate.
5. Why can retrying a timed-out write duplicate work? **Answer:** The original write might already have committed.

**Exit ticket:** Explain a request from process execution through DNS, transport, server processing, and response. Identify three distinct failure points and appropriate behavior.
