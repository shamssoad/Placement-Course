# Scope, references, and verification status

[Contents](../TEXTBOOK.md)

## Scope

The explanations, examples, exercises, and solutions are original teaching material. The public GeeksforGeeks page supplied the broad subject coverage, not private lessons. C++17 is the baseline; C++20 extensions are labeled. The library chapters teach named families and practical contracts, rather than reproducing every ISO overload or implementation-specific detail.

## Optional primary references

- [C++17 draft: containers](https://timsong-cpp.github.io/cppwp/n4659/containers) — versioned formal container requirements.
- [C++17 draft: algorithms](https://timsong-cpp.github.io/cppwp/n4659/algorithms) — formal algorithm contracts and complexity clauses.
- [C++17 draft: utilities](https://timsong-cpp.github.io/cppwp/n4659/utilities) — supporting library facilities.
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) — practical guidance on interfaces, ownership, and resource management.

These references were consulted during expansion. They are optional verification sources, not external tutorials required to replace the course's explanations. The draft specifies language/library behavior; the guidelines provide design recommendations.

## Verification status

The terminal execution service failed to start during initial creation and this expansion. No compilation or successful test execution is claimed for this authoring session. Supplied checks and expected outputs are learning material, not reports that the checks were run.

The original reference files are complete programs. Textbook blocks are described as complete programs, functions, classes, or fragments; fragments require a driver and indicated headers. Undefined-behavior anti-examples are for diagnosis, not execution. Arithmetic contracts must be reconsidered if you enlarge input bounds.

Database and networking labs require a chosen database engine and operating-system API. The chapters identify those boundaries rather than presenting one platform's behavior as universal.

## Student verification workflow

Compile with warnings, run deterministic cases, compare optimized algorithms with small brute-force baselines, and use available sanitizers on executed paths. Investigate every discrepancy. Proofs, lifetime reasoning, and tests complement one another; sample output alone does not establish correctness.
