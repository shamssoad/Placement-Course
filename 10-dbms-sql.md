# Module 10 — DBMS and SQL

**Expanded textbook:** [Relational theory, SQL, storage, and transactions](textbook/10-databases.md). This file is the introductory/revision version.

Prerequisite: basic programming. Outcome: model relational data, write queries, and reason about persistence.

Examples use a broadly portable SQL subset. Use any relational engine for the basic lab; window-function and transaction behavior should be checked in that engine's documentation. C++ integrates with the database, while SQL expresses the queries.

## Lesson 10.1 — Model entities and constraints

A table represents a relation. A primary key identifies a row; a foreign key refers to a valid row elsewhere; a unique constraint expresses a rule that must hold even if two clients write concurrently.

```sql
CREATE TABLE customers (
    customer_id INTEGER PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);
CREATE TABLE bookings (
    booking_id INTEGER PRIMARY KEY,
    customer_id INTEGER NOT NULL REFERENCES customers(customer_id),
    show_id INTEGER NOT NULL,
    seat_code VARCHAR(10) NOT NULL,
    amount_minor INTEGER NOT NULL CHECK (amount_minor >= 0),
    status VARCHAR(12) NOT NULL
        CHECK (status IN ('CONFIRMED', 'CANCELLED'))
);
INSERT INTO customers VALUES (1, 'Amina'), (2, 'Rafi'), (3, 'Maya');
INSERT INTO bookings VALUES
    (101, 1, 7, 'A1', 1200, 'CONFIRMED'),
    (102, 1, 8, 'B2', 1500, 'CONFIRMED'),
    (103, 2, 7, 'A2', 1200, 'CANCELLED');
```

This small query dataset does not enforce active-seat uniqueness. The later design lesson adds a separate allocation relation for that purpose.

Embedding customer names in every booking causes anomalies: changing a name requires many updates, and inconsistent copies can appear. The functional dependency `customer_id -> name` belongs in customers. Normalization separates facts according to dependencies. Third normal form removes problematic transitive dependencies; BCNF requires every nontrivial determinant to be a superkey. Decomposition should preserve information, and dependency enforcement also matters.

## Lesson 10.2 — Query order and NULL

Conceptually, a query forms rows from `FROM` and joins, filters with `WHERE`, groups, filters groups with `HAVING`, selects expressions, then orders. The optimizer may execute differently while preserving the query's meaning.

```sql
SELECT customer_id, SUM(amount_minor) AS spent
FROM bookings
WHERE status = 'CONFIRMED'
GROUP BY customer_id
HAVING SUM(amount_minor) >= 2000
ORDER BY spent DESC;
```

Only customer 1 appears, with 2700. `WHERE` removes cancelled rows before summation. `HAVING` filters the resulting customer groups.

NULL means missing or unknown, not zero or an empty string. `NULL = NULL` is unknown, so use `IS NULL`. `COUNT(*)` counts rows; `COUNT(column)` counts nonnull values. `NOT IN` can surprise when its subquery contains NULL: comparisons become unknown. A correctly correlated `NOT EXISTS` often expresses absence more clearly.

## Lesson 10.3 — Joins and cardinality

```sql
SELECT c.customer_id, c.name, COUNT(b.booking_id) AS confirmed_count
FROM customers c
LEFT JOIN bookings b
  ON b.customer_id = c.customer_id AND b.status = 'CONFIRMED'
GROUP BY c.customer_id, c.name
ORDER BY c.customer_id;
```

The counts are 2, 0, 0. The status condition is in the join so customers without matching bookings remain. Moving it to `WHERE b.status='CONFIRMED'` removes null-extended rows and loses those customers.

Joining two separate one-to-many relations can multiply rows. If a customer has two bookings and three reviews, joining both produces six combined rows. Summing booking amounts then overcounts unless you preaggregate at the appropriate grain. `DISTINCT` is not a universal fix: two legitimate bookings can have the same amount.

## Lesson 10.4 — Window functions and ties

```sql
SELECT booking_id, customer_id, amount_minor,
       ROW_NUMBER() OVER (
           PARTITION BY customer_id
           ORDER BY amount_minor DESC, booking_id ASC
       ) AS position
FROM bookings
WHERE status = 'CONFIRMED';
```

A window function computes across related rows without collapsing them into one group. `ROW_NUMBER` gives unique positions; `RANK` leaves gaps after ties; `DENSE_RANK` does not. Decide whether “top two” means two rows or two distinct price ranks.

A running total needs deterministic ordering and a defined frame. Use `ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW` when you mean successive rows; a range-based frame can group equal ordering values. A CTE names an intermediate query; it does not inherently guarantee materialization or better performance.

## Lesson 10.5 — Transactions and concurrent booking

A transaction groups operations into one committed outcome. Atomicity means all or none; isolation addresses interaction with concurrent work; durability concerns committed persistence. Consistency depends partly on correct constraints and application rules—it is not an automatic guarantee of every business requirement.

To avoid two active owners of one seat, make a table whose primary key is `(show_id, seat_code)`. Within a transaction, insert that allocation and the associated booking. A competing insert for the same key must not create a second allocation. If either part fails, roll back. Cancelled booking history stays in its own table while the allocation row is removed when appropriate.

A pre-check followed by an insert is insufficient by itself: two transactions can both see availability before either writes. Use a database-enforced constraint and handle the losing operation. For temporary holds, add ownership and expiry and define the atomic replacement rule.

Dirty reads observe uncommitted work; nonrepeatable reads observe a changed row on a later read; phantoms concern rows appearing or disappearing from a repeated predicate. Exact isolation behavior varies by engine. Serialization failures and deadlocks can require retrying the whole transaction, not only its last statement.

## Lesson 10.6 — Indexes and plans

An index trades storage and write cost for faster access. A composite index `(customer_id, show_id)` commonly supports searches beginning with customer_id, but may not serve a show-only filter efficiently. Selectivity describes how much a condition narrows the rows. A low-selectivity lookup can make a full scan cheaper.

Read a query plan by asking how many rows are expected, which access paths are chosen, and where joins or sorts happen. Estimated rows can differ sharply from actual rows if statistics are stale or predicates are correlated. Adding indexes without measuring can slow writes and waste space.

## Lesson 10.7 — C++ integration contracts

Use parameter binding instead of concatenating user input into SQL. Manage connections, prepared statements, and transactions with RAII. Keep a transaction as short as the business operation permits; do not hold a seat lock while waiting for a user to type payment information.

Represent database failures explicitly. A timeout may leave a client uncertain whether a request committed, so repeated operations need a stable request identity or idempotency rule. Do not blindly create a second booking after a lost response.

## Exercises and answers

1. List customers with no confirmed bookings. **Answer:** Use `NOT EXISTS` correlated on customer ID and confirmed status.
2. Why count `b.booking_id` in the left join? **Answer:** Null-extended rows should contribute zero, while `COUNT(*)` would count them.
3. Find each customer's largest confirmed booking. **Answer:** Partition by customer and rank amounts; define whether to include tied maxima.
4. Prevent duplicate active seats. **Answer:** Enforce one allocation per `(show_id,seat_code)` and use a transaction.
5. Why parameterize queries? **Answer:** Values remain values rather than being interpreted as SQL syntax.

**Exit ticket:** Load the dataset, predict results before executing, write the no-booking query, and describe a concurrent double-booking schedule and its constraint-based prevention.
