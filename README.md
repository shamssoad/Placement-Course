# Complete Placement Preparation with C++

**Start with the [expanded textbook](TEXTBOOK.md).** It adds full chapters to every module. The original module notes below serve as introductions and revision summaries.

This is an original, text-based self-study course: teaching notes, worked examples, C++ implementations, exercises, answer keys, assessments, and project specifications. Begin with Module 1, not with the assessments. The material assumes no programming experience and builds toward placement interviews.

The broad subject coverage is inspired by the public [GeeksforGeeks Placement 360 page](https://www.geeksforgeeks.org/courses/placement-360-cip-complete-tech-interview), inspected on 7 October 2026. The explanations, lesson sequence, examples, exercises, and projects here are original. This is not a reproduction of private course content.

## Your classroom

| Order | Teaching material | Outcome |
|---|---|---|
| 1 | [C++ foundations](01-cpp-foundations.md) | Write and debug programs; understand values and lifetime |
| 2 | [STL and modern C++](02-stl-modern-cpp.md) | Select containers, algorithms, and ownership tools |
| 3 | [Analysis, mathematics, and recursion](03-analysis-math-recursion.md) | Prove correctness and estimate resource use |
| 4 | [Arrays, search, sort, and hashing](04-arrays-search-sort-hashing.md) | Recognize and derive common interview patterns |
| 5 | [Strings and linear structures](05-strings-linear-structures.md) | Process text and maintain stack, queue, and list invariants |
| 6 | [Trees and advanced structures](06-trees-range-structures.md) | Implement hierarchical and dynamic-query structures |
| 7 | [Graphs](07-graphs.md) | Model connectivity, dependencies, and optimal routes |
| 8 | [Greedy, backtracking, and DP](08-greedy-backtracking-dp.md) | Derive algorithms from decisions and states |
| 9 | [OOP and low-level design](09-oop-design.md) | Design testable C++ programs with safe ownership |
| 10 | [DBMS and SQL](10-dbms-sql.md) | Model data, query it, and reason about transactions |
| 11 | [Operating systems and networks](11-os-networks.md) | Explain execution, concurrency, memory, and communication |
| 12 | [Aptitude, interviews, and AI practice](12-placement-practice.md) | Prepare for assessments and communicate independently |

Also use [projects](PROJECTS.md), [worked implementation labs](WORKED-LABS.md), [assessments and answer keys](ASSESSMENTS.md), and the [60-assignment practice workbook](PRACTICE.md).

## How to take a lesson

1. Read the explanation, then close it and describe the central idea in your own words.
2. Trace the example on paper. Track indexes, state, and invariants before executing anything.
3. Type the code yourself into a small program. Avoid copying until you have tried an implementation.
4. Change one assumption: empty input, duplicate values, a disconnected graph, or a failed operation.
5. Attempt the exercises before opening the answer key.
6. Record the mistake that caused each failure, not just the final answer.

Every code block is either a complete example or a named function/class intended to be placed in a driver. The first lesson shows how to build a driver. Standard-library names use `std::`; snippets name their required headers. Code targets C++17 unless explicitly marked C++20. No example requires the nonstandard `bits/stdc++.h` header.

## Weekly routine and pacing

Use roughly 3 hours for explanations, 6 for practice, 2 for revision, and 2 for project work. These are planning estimates, not a guaranteed completion time. A beginner may need longer.

- Weeks 1–7: Modules 1–2; build the expense tracker.
- Weeks 8–16: Modules 3–5; attempt Assessment A.
- Weeks 17–28: Modules 6–8; attempt Assessment B.
- Weeks 29–35: Modules 9–10; implement the booking project.
- Weeks 36–38: Module 11; implement the worker queue.
- Weeks 39–40: Mixed revision, Assessment C, and mock interviews.
- Throughout: One aptitude set per week, project commits, and spaced revision.

Reattempt a missed problem after about 2 days, 1 week, and 3 weeks. Remove its topic label on the last attempt. Advance when you can explain the method and solve a changed version, not when you recognize the original answer.

## Build and check your code

With a GCC-compatible compiler already installed:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 -g main.cpp -o main
```

For the thread-pool project on platforms whose toolchain needs it, add `-pthread`. With MSVC, use a Developer Command Prompt and `cl /std:c++17 /W4 /EHsc main.cpp`.

On a compiler/platform supporting these sanitizers, add `-fsanitize=address,undefined -fno-omit-frame-pointer`. Tool availability varies. Sanitizers help detect mistakes; passing them does not prove correctness.

Use small deterministic tests first. For algorithms, compare optimized output against an obvious brute-force implementation on many small inputs. For concurrency, test shutdown and exceptions as well as ordinary results.

## Completion standard

Complete the core lessons, workbook, three assessments, and two projects. You should be able to justify algorithms, implement them independently, query a relational database, explain core CS behavior, and discuss project tradeoffs. Optional advanced clinics extend the course; they are not prerequisites for every interview.

This course contains written lessons and original practice prompts. It does not include recorded videos, access to paid problem banks, a certification service, or live mentoring.

Authoring verification note: the terminal runtime was unavailable while this material was created. The C++ reference programs include build instructions and, for the algorithm lab, deterministic checks, but no compilation or successful test execution is claimed here.
