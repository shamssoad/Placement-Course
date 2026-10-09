# Complete Placement Preparation — expanded textbook

This is the main reading path for the expanded course. The original twelve module files remain useful as introductions and revision notes. Read the chapters below for the full treatment, then complete the linked labs and exercises. Each chapter explains the reasoning as well as the mechanics; examples are deliberately small enough to trace.

## Scope and reading conventions

The language baseline is C++17. C++20 extensions are labeled explicitly. The standard-library volume covers every C++17 standard container and adaptor, the main iterator machinery, the named algorithm families, numeric algorithms, and practical supporting facilities. Advanced compiler implementation, exhaustive template metaprogramming, device programming, and every overload in the ISO specification are outside this placement course's scope. You do not need external tutorials for the taught concepts; primary-source links are optional verification references.

Code blocks marked as complete programs can be compiled alone. Other blocks are functions, classes, or local fragments to add to a driver with the indicated headers. Assume stated numeric limits; do not silently generalize arithmetic examples beyond those limits. Written answers are supplied for chapter exercises. Larger projects are independent assignments with acceptance criteria and reference implementations where linked.

## Volume 1 — C++ language foundations

1. [Programs, values, expressions, and control flow](textbook/01-cpp/01-language-basics.md)
2. [Functions, interfaces, translation units, and debugging](textbook/01-cpp/02-functions-programs.md)
3. [Arrays, text, pointers, references, and lifetime](textbook/01-cpp/03-memory-text.md)
4. [Classes, value semantics, ownership, and polymorphism](textbook/01-cpp/04-classes-ownership.md)
5. [Templates, lambdas, deduction, and modern C++](textbook/01-cpp/05-generic-modern.md)
6. [Exceptions, streams, files, and an integrated application](textbook/01-cpp/06-errors-io-application.md)

Workshop: [Build an owning value type, with a complete reference program](textbook/01-cpp/07-ownership-workshop.md).

## Volume 2 — Standard library

1. [Every standard container and adaptor](textbook/02-stl/01-containers.md)
2. [Iterators, invalidation, and callable objects](textbook/02-stl/02-iterators-callables.md)
3. [Algorithms and numeric operations](textbook/02-stl/03-algorithms.md)
4. [Utilities, ownership tools, time, random numbers, and workflows](textbook/02-stl/04-utilities-workflows.md)

Workshop: [Nine complete STL recipes and a report pipeline](textbook/02-stl/05-recipe-lab.md).

## Volumes 3–12

3. [Analysis, proofs, mathematics, and recursion](textbook/03-analysis.md)
4. [Arrays, searching, sorting, and hashing](textbook/04-array-patterns.md)
5. [Strings, lists, stacks, queues, and deques](textbook/05-linear-structures.md)
6. [Trees, heaps, tries, and range structures](textbook/06-trees.md)
7. [Graph modeling and algorithms](textbook/07-graphs.md)
8. [Greedy reasoning, backtracking, and dynamic programming](textbook/08-dynamic-programming.md)
9. [Object-oriented design and complete case studies](textbook/09-design.md)
10. [Relational theory, SQL, storage, and transactions](textbook/10-databases.md)
11. [Operating systems, concurrency, and networking](textbook/11-systems.md)
12. [Aptitude, interviews, portfolio, and AI-assisted work](textbook/12-placement.md)

## Practical materials

- [60-assignment workbook](PRACTICE.md): base exercises with changed-contract extensions.
- [Worked implementation labs](WORKED-LABS.md): traces and explanations of complete algorithms.
- [Project handbook](PROJECTS.md): milestones, contracts, and acceptance tests.
- [Assessments with answer keys](ASSESSMENTS.md): test understanding after each major stage.
- [Expanded mastery examinations](textbook/MASTERY.md): language, library, algorithms, and systems checkpoints.
- [Primary references and verification status](textbook/REFERENCES.md).

## How much time to allow

The earlier forty-week outline was for a shorter course. For this expanded edition, use mastery gates rather than a fixed deadline. A chapter may take several study sessions: read, trace, implement, solve, then revisit without notes. Do not read the solution immediately after feeling stuck; first write your current invariant or state definition and a concrete example that defeats it.

Keep one notebook with four sections: concepts in your own words, code traces, mistakes with failing inputs, and spaced-revision tasks. Being able to recognize a solution is not the same as being able to derive it.
