# Observability Property

**Property**: State queries return correct values. Query methods (size, empty, contains, etc.) return values consistent with actual state.

## Concept

Execute actions and query component state at various points. Verify:
- Query results match actual component state
- Queries are consistent with each other
- Queries work correctly after all operations
- Queries reflect state changes correctly

**Key idea**: Component state should be queryable correctly; queries should return values that match the actual state.

## Actions

Focus on query operations at different points:

- **After each modification**: Query immediately after insert/delete/update
- **After sequences**: Query after multiple operations
- **After errors**: Query after operations that fail
- **Query types**: Size, empty, contains, get, iteration, etc.

**Model comparison**: Maintain simplified model of component state, compare query results against model.

## Checks

Verify query correctness:

- **Query accuracy**: Query results match actual state (e.g., size matches actual elements)
- **Query consistency**: Queries are consistent (e.g., empty() == (size() == 0))
- **Model match**: Query results match simplified model
- **Query validity**: Queries don't crash, return reasonable values

**State verification**: Compare query results against ground truth (e.g., count elements directly, verify against model).

## Subdomains

**Normal queries**: Queries after normal operations, typical usage patterns.

**Query edge cases**: Queries on empty component, queries on full component, queries after errors, queries during state transitions.

**Query stress**: Many queries, rapid query sequences, queries after many operations.
