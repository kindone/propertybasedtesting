# Reference Comparison Property

**Property**: New implementation produces same output as reference for all inputs. New implementation meets performance criteria.

## Concept

Use existing code (reference implementation) as test oracle. Verify:
- New implementation produces same output as reference
- New implementation meets performance requirements (time/space complexity)
- Behavior preserved during refactoring/enhancement

**Key idea**: Reference implementation serves as oracle; compare outputs and performance to ensure correctness and performance.

## Actions

Focus on operations that might differ:

- **All operations**: Any operation that produces output
- **Edge case operations**: Operations on edge cases that might break during refactoring
- **Performance-critical operations**: Operations where performance matters

**Comparison points**: Compare outputs after each operation, compare performance metrics.

## Checks

Verify correctness and performance:

- **Output correctness**: New implementation output matches reference output
- **Performance comparison**: New implementation performance comparable to reference
- **Complexity verification**: New implementation meets stated complexity bounds
- **Regression detection**: No regressions introduced (output or performance)

**Oracle comparison**: Use reference implementation as ground truth for both correctness and performance.

## Subdomains

**Normal operation**: Typical inputs, normal usage patterns. Baseline comparison.

**Edge cases**: Edge cases that might break during refactoring, boundary conditions, error paths.

**Performance stress**: Large inputs, worst-case inputs, inputs that stress performance characteristics.
