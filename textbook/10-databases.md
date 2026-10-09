# Volume 10 — Relational theory, SQL, storage, and transactions

[Contents](../TEXTBOOK.md) · [Introductory module and dataset](../10-dbms-sql.md)

## Chapter 1. A database stores facts with enforceable relationships

An entity needs a stable identity; its display name rarely serves as a safe key. A candidate key uniquely identifies a row, and one is selected as primary. A foreign key relates a row to another valid row. Unique, not-null, and check constraints encode rules close to the stored data, where all clients must obey them.

For students and courses, enrollment is a many-to-many relation with a composite key `(student_id,course_id)`. Storing a comma-separated course list inside each student row makes individual relationships hard to constrain and query. Split it into one enrollment row per fact.

```sql
CREATE TABLE students (
  student_id INTEGER PRIMARY KEY,
  name VARCHAR(100) NOT NULL
);
CREATE TABLE courses (
  course_id INTEGER PRIMARY KEY,
  title VARCHAR(100) NOT NULL
);
CREATE TABLE enrollments (
  student_id INTEGER REFERENCES students(student_id),
  course_id INTEGER REFERENCES courses(course_id),
  mark INTEGER CHECK (mark BETWEEN 0 AND 100),
  PRIMARY KEY (student_id, course_id)
);
INSERT INTO students VALUES (1,'Amina'),(2,'Rafi'),(3,'Maya');
INSERT INTO courses VALUES (10,'C++'),(20,'Databases');
INSERT INTO enrollments VALUES (1,10,90),(1,20,80),(2,10,70),(3,20,NULL);
```

NULL mark represents not yet graded, not zero. Exact constraint syntax and enforcement options vary by engine, so the lab chooses an engine and verifies its behavior rather than assuming every configuration is identical.

## Chapter 2. Functional dependencies and normalization

`student_id -> name` means each student ID determines one name. In a combined enrollment table, the key `(student_id,course_id)` determines mark, while only part of that key determines student name. Duplicating names across enrollments creates update anomalies.

First normal form uses atomic values relative to the relational design rather than repeated groups. Second normal form removes partial dependencies of nonprime attributes on proper subsets of candidate keys. Third normal form constrains transitive dependencies using its formal dependency condition. BCNF is stricter: every determinant of a nontrivial dependency must be a superkey.

Decomposition should be lossless: joining the decomposed relations should reproduce the intended facts without invented combinations. Dependency preservation matters too, because otherwise enforcing a rule can require cross-table checks. Normalization improves consistency; denormalization may improve a measured read pattern at the cost of maintaining duplicate facts.

## Chapter 3. SQL's rows, bags, and unknown truth

SQL commonly preserves duplicates unless DISTINCT is requested. Joining tables combines matching rows and can multiply them. Always name the intended row grain: one row per student, enrollment, course, or booking.

WHERE filters individual rows. GROUP BY forms groups; HAVING filters groups after aggregation. Selecting an ungrouped arbitrary column is not a portable way to choose a representative row. ORDER BY is required when result order matters.

NULL comparisons usually produce unknown. WHERE keeps true rows, not false or unknown. Use IS NULL. COUNT(*) counts rows; COUNT(mark) counts graded marks. AVG(mark) ignores null marks, so its denominator differs from all enrollments when some are ungraded.

## Chapter 4. Joins and absence

```sql
SELECT s.student_id, s.name, COUNT(e.course_id) AS enrollment_count
FROM students s
LEFT JOIN enrollments e ON e.student_id=s.student_id
GROUP BY s.student_id,s.name;
```

A left join preserves students without matches using null-extended columns. Filtering a right-side condition in WHERE can remove them; place qualifying-match conditions in ON when preservation is intended.

To find students not enrolled in C++, use NOT EXISTS:

```sql
SELECT s.student_id,s.name
FROM students s
WHERE NOT EXISTS (
  SELECT 1 FROM enrollments e
  WHERE e.student_id=s.student_id AND e.course_id=10
);
```

The answer is Maya. NOT IN can behave unexpectedly if its subquery produces NULL, so reason through three-valued logic rather than treating it as ordinary set subtraction.

## Chapter 5. Subqueries, windows, and relational division

A correlated subquery refers to a row from its outer query. A CTE names a query component and can improve readability; it does not universally promise a particular execution strategy.

For per-course ranking:

```sql
SELECT course_id,student_id,mark,
       DENSE_RANK() OVER (PARTITION BY course_id ORDER BY mark DESC) AS rank_in_course
FROM enrollments
WHERE mark IS NOT NULL;
```

DENSE_RANK groups equal marks without gaps. Adding student ID to the ranking order would break those ties, so add it only to outer presentation ordering if ties should remain ties.

“Students enrolled in every course” is relational division. One approach uses a double NOT EXISTS: there must not exist a course for which the student has no enrollment. Another groups enrollments and compares distinct course count with total course count. The empty-course case and duplicate handling should be explicitly considered.

Window frames matter for running totals. A ROWS frame accumulates row positions; a RANGE frame can include peers sharing ordering values. Supply a deterministic tie-breaker and a frame matching the business question.

## Chapter 6. Transactions and anomalies

A transaction makes a unit of work commit or roll back. Atomicity does not imply full isolation from other transactions. Isolation mechanisms determine what concurrent reads and writes can observe.

Lost update example: two transactions read balance 100; one computes 110, the other 120; writing both derived values can lose one increment. An atomic SQL increment, row locking, or version check can prevent the relevant anomaly under the chosen engine's rules.

Write skew involves transactions reading overlapping conditions and writing different rows, jointly violating a rule despite no direct same-row write conflict. Snapshot-style reads alone do not eliminate every such anomaly. Serializable isolation aims to make committed outcomes equivalent to a serial ordering, but applications must handle retries when the engine aborts a conflicting transaction.

For seat allocation, enforce a unique active `(show_id,seat_code)` and make related changes transactional. A read-then-insert precheck cannot replace uniqueness because competing transactions can both observe availability. Deadlocks may still occur; use consistent access order where useful and retry the whole aborted transaction when the error contract permits it.

## Chapter 7. Storage, indexes, and recovery

Databases read and write pages, not abstract rows one at a time. B-tree-family indexes arrange keys to support ordered search and range scans with shallow page traversal. Hash indexes support different access patterns. A composite index's leading columns strongly affect which predicates can narrow the scan efficiently.

An index can cover a query when needed data is available without additional table lookups under the engine's storage model. Every extra index costs storage and write maintenance. An optimizer chooses among scans, index lookups, joins, and sorts based on statistics and cost estimates; inspect actual plans and row counts before assuming an index helps.

Write-ahead logging records recovery information before the corresponding persistent data changes under the engine's protocol. Checkpoints reduce recovery work. Backups and replicas solve different problems; a replica can reproduce an accidental deletion, so it is not automatically a historical backup.

## Chapter 8. C++ integration and failure boundaries

Bind values through prepared statements. Manage connections and statements with RAII. A transaction wrapper should roll back uncommitted work on scope exit and expose an explicit commit that can report failure. Do not concatenate user text into SQL syntax.

Connection pools limit creation overhead but introduce a finite shared resource. Return connections promptly and avoid holding them while waiting for user interaction. Timeouts can leave an uncertain outcome if commit succeeded but the response was lost; use stable operation identities for retry-safe writes.

## Worked query exercises

1. Average graded mark by course: `SELECT course_id,AVG(mark) FROM enrollments GROUP BY course_id;` gives 80 for both courses in this dataset.
2. Students averaging at least 80: group by student and use `HAVING AVG(mark)>=80`; Amina qualifies, Rafi does not, Maya's all-null average does not satisfy the condition.
3. Ungraded enrollments: `WHERE mark IS NULL`, yielding Maya's database enrollment.
4. All students with graded-count including zero: left join, group, and COUNT(e.mark).
5. Why can joining each enrollment to multiple attendance rows inflate marks? **Answer:** The join changes row grain and repeats enrollment facts; aggregate attendance separately before joining when that is the intended grain.

**Practical gate:** Create the schema, predict every query result, implement the “every course” query, and demonstrate one concurrent anomaly plus its prevention using the chosen database engine.
