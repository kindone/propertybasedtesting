# Authorization Property

**Property**: Users can only access permitted resources. Access control enforced correctly.

## Concept

Test that system enforces authorization: users can only access resources they're permitted to access. Verify:
- Permitted access granted
- Forbidden access denied
- Permission checks enforced
- Access control works correctly

**Key idea**: System should enforce access control based on user permissions.

## Actions

Focus on authorization scenarios:

- **Initial state generation**: Generate initial authorization system state (users with different permission levels, resources with access controls, role hierarchies, permission inheritance). Initial state determines what authorization scenarios are possible.
- **Action sequences with parameters**: Generate access attempt sequences with specific parameters (users, resources, operations, permission levels). Action sequences test authorization across different permission scenarios.
- **Permitted access**: Authenticate user U with permissions P. Attempt access to resource R where U has permission → verify succeeds
- **Forbidden access**: Authenticate user U with permissions P. Attempt access to resource R where U lacks permission → verify denied
- **Permission scenarios**: Test various permission levels: (1) read-only, (2) read-write, (3) admin, (4) resource-specific permissions
- **Privilege escalation**: Attempt to escalate privileges: (1) modify permission token, (2) access admin endpoints with user token, (3) bypass permission checks

**Concrete plan**:
1. Generate initial authorization system state (users with permissions, resources with access controls, role hierarchies)
2. Authenticate users with different permission levels
3. Generate access attempt sequences with parameters (users, resources, operations, permission levels)
4. Generate access attempts: permitted resources, forbidden resources
5. Attempt privilege escalation: modify tokens, access admin endpoints
6. Track access results: success/failure
7. Verify access control enforcement

Verify authorization:

- **Permitted access**: For user U with permission P on resource R, attempt access → assert succeeds, operation allowed
- **Forbidden denial**: For user U without permission P on resource R, attempt access → assert denied, error returned (403 Forbidden), operation blocked
- **Permission enforcement**: Verify permission checks: (1) checked before operation, (2) enforced consistently, (3) not bypassable
- **No escalation**: Attempt privilege escalation → assert blocked: (1) modified token rejected, (2) admin endpoint requires admin permission, (3) permission checks not bypassable

**Concrete checks**:
1. Permitted: user with permission accesses resource → assert success
2. Forbidden: user without permission accesses resource → assert 403 error, access denied
3. Permission enforcement: verify permission checked before operation (not after)
4. Privilege escalation: modify token, access admin endpoint → assert blocked
5. Consistency: same permission check produces same result (deterministic)
6. Bypass attempts: try to bypass checks → assert all blocked

## Subdomains

**Initial state: system with multiple users at same level**: System starts with multiple users at same privilege level. Action sequence: User accesses another user's resources (same privilege level, different user). Test: verify horizontal privilege escalation (access control prevents cross-user access).

**Initial state: system with regular users and admin resources**: System starts with regular users and admin-only resources. Action sequence: Regular user attempts to access admin-only resources. Test: verify vertical privilege escalation (admin permissions required), escalation prevented.

**Initial state: system with RBAC configured**: System starts with role-based access control configured (roles, role hierarchies). Action sequence: User with role R attempts to access resource requiring role R'. Test: verify role-based access control (roles enforced correctly), role hierarchy respected.

**Initial state: system with ABAC configured**: System starts with attribute-based access control configured (user attributes, resource attributes, environment). Action sequence: Access decisions based on attributes. Test: verify attribute-based access control (attributes evaluated correctly), access granted/denied appropriately.

**Initial state: system with group permissions**: System starts with users in groups, group permissions configured. Action sequence: User inherits permissions from group, group permissions change. Test: verify permission inheritance (inheritance works correctly), permission changes propagate.

**Initial state: system with time-based permissions**: System starts with time-based access control configured (time windows, expiration dates). Action sequence: Access attempts at different times, permissions expire. Test: verify time-based access control (time-based restrictions enforced), expired permissions rejected.
