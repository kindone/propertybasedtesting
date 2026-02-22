# Resource Management Property

**Property**: Resources (handles, connections, locks) acquired are released. RAII compliance, no resource leaks, cleanup happens correctly.

**Applies to**: File handles, network connections, database connections, locks, other system resources.

## Concept

Track resource acquisition and release throughout component lifetime. Verify:
- All acquired resources are released
- Resources are released in correct order
- No resource leaks occur
- Cleanup happens correctly (including on exceptions, errors)
- Component destruction releases all resources

**Key idea**: Component should manage resources correctly (RAII), releasing all acquired resources.

## Actions

Focus on resource lifecycle:

- **Acquisition**: Open file, connect to network, acquire lock, allocate handle
- **Usage**: Operations that use resources
- **Release**: Close file, disconnect, release lock, free handle
- **Exception paths**: Resources acquired but exception thrown before release

**Resource tracking**: Track each resource acquisition and release. Verify all acquired resources are eventually released.

## Checks

After sequences and destruction, verify:

- **Leak check**: All acquired resources are released (acquisitions match releases)
- **Resource state**: Resources are in correct state (e.g., file handles closed, connections closed)
- **Exception cleanup**: Resources released even when exceptions occur
- **Destruction cleanup**: Component destruction releases all resources

**Resource verification**: Check resource state directly (e.g., file handle status, connection status).

## Subdomains

**Normal lifecycle**: Acquire, use, release. Proper cleanup.

**Exception scenarios**: Resources acquired but exception thrown, cleanup on exceptions, resources not released.

**Resource stress**: Many resources, rapid acquire/release, resource exhaustion, cleanup under stress.
