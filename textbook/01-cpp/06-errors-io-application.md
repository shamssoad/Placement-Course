# C++ 1.6 — Errors, streams, files, and integrated application development

[Previous](05-generic-modern.md) · [Contents](../../TEXTBOOK.md) · [Next volume](../02-stl/01-containers.md)

## 1. Choose a failure contract

There are three useful categories: invalid caller input, an expected negative result, and an exceptional operational failure. A missing search result may use optional; a declined reservation may use a result enum; inability to read a required data file may throw or return a structured error. The important property is that callers can distinguish outcomes without parsing human-readable messages.

Exceptions propagate until a matching catch block. During propagation, completed automatic objects are destroyed. Catch polymorphic exception types by const reference to avoid slicing. Catch at a boundary that can meaningfully respond; catching everywhere and continuing with damaged state hides failure.

```cpp
#include <exception>
#include <iostream>
// Fragment in main, after defining runApplication().
try {
    runApplication();
} catch (const std::exception& error) {
    std::cerr << "Application failed: " << error.what() << '\n';
    return 1;
}
```

`noexcept` is a promise with consequences: an escaping exception terminates the program. Do not mark an operation noexcept merely to avoid handling errors. Assertions check internal programmer assumptions and may be disabled; they must not be the only check on untrusted input.

## 2. Streams maintain state

Formatted extraction such as `input >> number` skips leading whitespace by default and attempts conversion. A failed extraction sets state flags. EOF is a state reached during attempted input, not a reliable prediction that another complete record exists.

Use `while (input >> value)` or `while (getline(input,line))`, not `while (!input.eof())`. The latter can enter one extra iteration and process a stale value. Clearing a fail state does not remove offending characters; consume or replace the problematic input as part of recovery.

Mixing `>>` and `getline` leaves the line separator after formatted input. Decide whether to discard the remainder of that line or consume leading whitespace. `std::ws` also removes other whitespace, so it can erase deliberately blank lines.

## 3. Parse a full record, not a prefix

```cpp
#include <sstream>
#include <string>
bool parseIntegerLine(const std::string& line, int& result) {
    std::istringstream input(line);
    int value = 0;
    if (!(input >> value)) return false;
    input >> std::ws;
    if (!input.eof()) return false;
    result = value;
    return true;
}
```

For `"12abc"`, reading an integer alone succeeds on 12, but the full-record check rejects leftover text. Assigning result only after validation gives the operation an unchanged-on-failure behavior. For high-volume numeric parsing, `from_chars` offers explicit end-position and error reporting without stream formatting machinery; it does not automatically skip whitespace.

## 4. Files and serialization

An `ifstream` owns reading access; an `ofstream` owns writing access. Check opening and later operations. A successful open does not prove all writes will succeed. `ios::app` appends; `ios::trunc` discards existing content on opening for the intended overwrite operation.

A serialization format needs boundaries, escaping, and versioning. Quoted strings can represent spaces and quotes; a delimiter-only format breaks if user text contains the delimiter unless escaping is defined. Avoid writing an arbitrary object's raw bytes as a portable format: padding, pointers, byte order, and ABI differences can make them meaningless elsewhere.

For safer replacement, write a separate temporary file, validate/flush according to the required durability model, then use a supported replacement mechanism. Exact atomicity and crash durability depend on the filesystem and platform; renaming a file is not a universal proof that data has reached persistent hardware.

## 5. Filesystem paths and errors

`std::filesystem::path` represents paths using platform-aware machinery. Use path composition rather than concatenating separators manually. Filesystem operations can fail due to missing paths, access rights, or concurrent changes. Some APIs provide throwing and error-code overloads.

Checking existence and then opening is not atomic: another process can change the path between those steps. For a classroom loader, the check may support a friendly “start empty if absent” policy; critical operations must handle the actual open result regardless of the earlier check.

## 6. Integrated gradebook

Build a gradebook with add, list, average, save, and load. Store a student ID, name, and marks. Keep three layers: parsing converts text to validated requests; domain functions update/query records; presentation prints results. Persistence encodes/decodes records without owning grading policy.

When adding, validate the ID, name, and mark before changing the vector. When loading, parse into a temporary collection and replace current state only after the entire file passes validation. Decide what duplicate IDs mean. Return a clear conflict rather than silently replacing a student.

Use `std::vector<Record>` initially. When lookup becomes central, evaluate an index rather than prematurely adding one. Every extra index creates a consistency rule: the vector and index must agree after insertions, removals, and failed operations.

## 7. A systematic completion checklist

Your program is ready for review when it has a written command grammar, useful errors, documented bounds, reproducible build instructions, tests for empty and malformed input, and an explanation of ownership. A program that only works for the demonstration input is not complete.

Trace one command through every layer. Identify where conversion occurs, where business validation occurs, where mutation commits, and where failure is reported. This exercise reveals functions doing several unrelated jobs.

## Exercises with worked answers

1. Why is `while(!eof())` wrong? **Answer:** EOF is set after a read reaches it, so the body can run after the last valid record. Make the read itself the condition.
2. A parser reads `12x` as 12. **Answer:** Require the remaining input to contain only permitted whitespace and then end.
3. A partially loaded file leaves half the new records installed. **Answer:** Parse into a temporary dataset and commit only on complete success.
4. Why not serialize a struct using its raw memory? **Answer:** Its bytes can include padding and process-specific pointers, and representation is not a portable schema.
5. When should the top-level catch continue? **Answer:** Only when it can restore a known valid state and the application contract allows recovery; otherwise report and exit.

**Module examination:** Complete the gradebook from a blank project, then explain value/reference parameters, object lifetime, a template use, a class invariant, and error propagation. This is the gate before moving to STL in depth.
