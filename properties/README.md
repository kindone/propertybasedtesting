# Fundamental Properties for Stateful Testing

This directory contains documentation for fundamental reusable properties that apply across many stateful components.

## Fundamental Properties

0. ✅ **[General Guidelines](00_general_guidelines.md)** - Prefer general, abstraction-level properties over implementation-specific ones
1. ✅ **[Algorithms / Data Structures](01_algorithms_data_structures.md)** - Correctness, complexity bounds, invariants
2. ✅ **[Reference Comparison](02_reference_comparison.md)** - Correctness by comparison, performance criteria
3. ✅ **[Consistency](03_consistency.md)** - Internal invariants maintained
4. ✅ **[Observability](04_observability.md)** - State queries return correct values
5. ✅ **[Resource management](05_resource_management.md)** - Resources acquired are released
6. ✅ **[Exception-safety](06_exception_safety.md)** - Exceptions don't leave component in invalid state
7. ✅ **[Memory safety](07_memory_safety.md)** - No leaks, double-free, use-after-free
8. ✅ **[Thread safety](08_thread_safety.md)** - Safe concurrent access
9. ✅ **[Malfunction Tolerance](09_malfunction_tolerance.md)** - Component handles erroneous responses from dependencies gracefully
10. ✅ **[Crash Tolerance](10_crash_tolerance.md)** - Component doesn't crash on any action sequence

## Advanced Properties

1. ✅ **[Database / Transactional (ACID)](advanced_01_database_transactional.md)** - Atomicity, Consistency, Isolation, Durability
2. ✅ **[Distributed Systems](advanced_02_distributed_systems.md)** - Eventual consistency, causal consistency, linearizability
3. ✅ **[Concurrency / Parallel Computing](advanced_03_concurrency_advanced.md)** - Serializability, liveness, safety, wait-freedom
4. ✅ **[Networking / Protocols](advanced_04_networking_protocols.md)** - Reliability, ordering, congestion control
5. ✅ **[Security](advanced_05_security.md)** - Authentication, authorization, confidentiality, integrity

## Structure

Each property file contains:
- **Property statement**: What the property asserts
- **Concept**: What to test, how to think about it, why it matters
- **Actions**: Description of actions/operations to generate (domain-specific)
- **Checks**: Description of what to verify (domain-specific)
- **Subdomains**: Practical categorization of subdomains to explore

## Usage

These properties serve as templates for deriving property-based tests. Use them to:
- Understand what properties to test for stateful components
- Identify what actions to generate (domain-specific)
- Determine what to verify (domain-specific)
- Explore relevant subdomains

See [REQUIREMENTS.md](REQUIREMENTS.md) for format requirements and [../propertyderivation.md](../propertyderivation.md) for the methodology behind these properties. The **quantifier view** (for all / for some / for no, explicit domain D, generator strategy by quantifier) is described in propertyderivation.md § The Quantifier View of Properties.
