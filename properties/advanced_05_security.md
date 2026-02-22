# Security Properties

**Property**: System maintains security properties - authentication, authorization, confidentiality, integrity.

## Concept

Test security properties through various access patterns and attacks. Verify:
- Only authorized users can access
- Users can only access permitted resources
- Sensitive data not leaked
- Data not tampered with

**Key idea**: System should enforce security properties even under various access patterns and potential attacks.

## Actions

Focus on security-relevant operations:

- **Authentication**: Login, credential verification
- **Authorization**: Access control checks, permission verification
- **Data access**: Read/write operations on sensitive data
- **Attack scenarios**: Unauthorized access attempts, tampering attempts

**Security testing**: Generate access patterns, simulate attacks, vary user permissions.

## Checks

Verify security properties:

- **Authentication**: Only authorized users can access
- **Authorization**: Users can only access permitted resources
- **Confidentiality**: Sensitive data not leaked to unauthorized users
- **Integrity**: Data not tampered with, tampering detected

**Security verification**: Verify access control, check for data leaks, verify data integrity.

## Subdomains

**Authorized access**: Normal access by authorized users. Baseline behavior.

**Unauthorized attempts**: Unauthorized access attempts, permission violations, attacks that should be blocked.

**Security stress**: Many access attempts, complex permission scenarios, sophisticated attacks.
