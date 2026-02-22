# Consistency Property

**Property**: Transactions preserve database invariants. Database remains in consistent state after transactions.

## Concept

Test that transactions maintain database consistency: invariants hold before and after transactions. Verify:
- Database invariants preserved
- Constraints not violated
- Referential integrity maintained
- Business rules enforced

**Key idea**: Database should remain consistent (invariants hold) throughout transaction execution.

## Actions

Focus on transactions that might affect invariants:

- **Initial state generation**: Generate initial database state (empty, with data at constraint boundaries, with referential relationships, with business rule states). Initial state determines what consistency scenarios are possible.
- **Action sequences with parameters**: Generate transaction sequences with operations that have specific parameters (values that push constraints, operations that affect relationships, operations that test business rules). Action sequences test consistency preservation.
- **Invariant-affecting operations**: Identify specific invariants (e.g., "account balance ≥ 0", "foreign key exists", "sum of parts equals total"). Generate operations that might violate them
- **Constraint-violating operations**: Generate operations that violate declared constraints (e.g., insert NULL in NOT NULL column, insert duplicate in UNIQUE column)
- **Referential integrity operations**: Generate operations affecting foreign keys (e.g., delete parent record, insert child with invalid foreign key)
- **Business rule operations**: Generate operations affecting business logic (e.g., transfer that would make balance negative, order that exceeds inventory)

**Concrete plan**:
1. Generate initial database state (empty, at constraint boundaries, with relationships, with business rule states)
2. Identify specific invariants/constraints to test (from schema, documentation, or code)
3. Generate transaction sequence with operations (with parameters) that would violate invariant if executed atomically
4. Execute transaction sequence
5. Check if violation prevented (transaction rejected) or corrected (invariant maintained after commit)

Verify consistency:

- **Invariant preservation**: After transaction, evaluate each invariant predicate: assert all invariants hold (e.g., assert account_balance >= 0 for all accounts)
- **Constraint enforcement**: Attempt constraint violation: assert transaction rejected with error OR constraint automatically corrected (e.g., default value inserted)
- **Referential integrity**: After foreign key operation, assert: (1) all foreign keys reference existing records, (2) cascade operations executed correctly
- **Business rules**: After business rule operation, assert business logic holds (e.g., assert inventory >= 0, assert order_total = sum(line_items))

**Concrete checks**:
1. Before transaction: evaluate all invariant predicates → all must be true
2. Execute transaction (may be rejected)
3. After transaction (if committed): re-evaluate all invariant predicates → all must still be true
4. For constraint violations: assert transaction rejected OR constraint corrected
5. For referential integrity: query all foreign keys, assert all reference existing records

## Subdomains

**Initial state: database at constraint boundaries**: Database starts with data at constraint limits (e.g., balance = 0, inventory = 0, foreign key = NULL). Action sequence: Operations that push constraints to limits. Test: verify constraint violations at boundaries (constraints enforced even at boundaries).

**Initial state: database with parent-child relationships**: Database starts with parent-child relationships established. Action sequence: Transaction that violates constraint, triggering cascade of constraint checks (e.g., delete parent triggers child constraint checks). Test: verify cascading constraint violations (all constraints checked), violations prevented.

**Initial state: database with multi-table data**: Database starts with data across multiple tables (e.g., orders and order_items). Action sequence: Transactions affecting multiple tables where invariant spans tables (e.g., sum of order items = order total). Test: verify multi-table invariant preservation (invariant preserved across all tables).

**Initial state: database with referential relationships**: Database starts with referential relationships (parent-child, foreign keys). Action sequence: Delete parent with children (CASCADE vs RESTRICT), insert child before parent exists, update foreign key to non-existent value. Test: verify referential integrity edge cases (referential integrity maintained).

**Initial state: database with business rule states**: Database starts with business rule states (e.g., accounts with balances, inventory levels, reservations). Action sequence: Transactions that create logical conflicts (e.g., transfer making balance negative, order exceeding inventory, overlapping reservations). Test: verify business rule conflicts (business rules enforced), conflicts prevented.

**Initial state: database with temporal data**: Database starts with temporal data (dates, timestamps, expiration dates). Action sequence: Transactions that depend on time-based invariants (e.g., start_date < end_date, expiration checks). Test: verify temporal consistency (temporal constraints enforced correctly).
