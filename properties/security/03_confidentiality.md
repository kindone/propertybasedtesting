# Confidentiality Property

**Property**: Sensitive data not leaked. Unauthorized users cannot access sensitive data.

## Concept

Test that system maintains confidentiality: sensitive data is protected from unauthorized access. Verify:
- Sensitive data not accessible to unauthorized users
- Data leaks prevented
- Encryption works correctly
- Confidentiality maintained

**Key idea**: System should protect sensitive data from unauthorized access.

## Actions

Focus on data access scenarios:

- **Initial state generation**: Generate initial system state (sensitive data stored, encryption configured, keys managed, logging configured). Initial state determines what confidentiality scenarios are possible.
- **Action sequences with parameters**: Generate data access sequences with specific parameters (users, data types, access methods, error scenarios). Action sequences test confidentiality across different access patterns.
- **Sensitive data access**: Authenticate authorized user U. Attempt access to sensitive data D (user has permission) → verify data returned, properly encrypted if transmitted
- **Unauthorized access**: Authenticate unauthorized user U (no permission). Attempt access to sensitive data D → verify access denied, data not returned
- **Data leakage scenarios**: Test scenarios that might leak data: (1) error messages (do they reveal data?), (2) logs (is sensitive data logged?), (3) side channels (timing, error codes)
- **Encryption scenarios**: Test encryption: (1) data encrypted at rest, (2) data encrypted in transit, (3) keys managed securely

**Concrete plan**:
1. Generate initial system state (sensitive data stored, encryption configured, keys managed, logging configured)
2. Authenticate users: authorized (has permission), unauthorized (no permission)
3. Generate data access sequences with parameters (users, data types, access methods, error scenarios)
4. Attempt access to sensitive data with each user type
5. Check data leakage: error messages, logs, side channels
6. Verify encryption: data at rest, data in transit
7. Test key management: key rotation, key storage

Verify confidentiality:

- **No unauthorized access**: Unauthorized user attempts access → assert denied, no data returned, error returned (403 Forbidden)
- **No leaks**: Check for data leakage: (1) error messages don't contain sensitive data, (2) logs don't contain sensitive data (or are redacted), (3) side channels don't reveal data (timing attacks, error codes)
- **Encryption works**: Verify encryption: (1) data at rest encrypted (check storage, verify encrypted), (2) data in transit encrypted (check protocol, verify TLS/SSL), (3) keys secure (not hardcoded, properly stored)
- **Confidentiality maintained**: Verify sensitive data protected: (1) only authorized users access, (2) data encrypted, (3) no leaks detected

**Concrete checks**:
1. Unauthorized access: unauthorized user accesses sensitive data → assert denied, no data returned
2. Error messages: check error messages → assert don't contain sensitive data
3. Logs: check logs → assert sensitive data redacted or not logged
4. Encryption at rest: check storage → assert data encrypted (not plaintext)
5. Encryption in transit: check protocol → assert TLS/SSL used
6. Key management: verify keys not hardcoded, properly stored
7. Side channels: test timing attacks → assert don't reveal data

## Subdomains

**Initial state: system with error handling configured**: System starts with error handling configured. Action sequence: System errors occur that might reveal sensitive data (user IDs, file paths, SQL queries). Test: verify information disclosure in error messages (error messages sanitized), no sensitive data leaked.

**Initial state: system with logging enabled**: System starts with logging enabled. Action sequence: Operations that might log sensitive data (passwords, tokens, PII). Test: verify log file confidentiality (logs redact sensitive data), or sensitive data not logged.

**Initial state: system with operations that might leak timing**: System starts with operations that might have timing differences. Action sequence: Timing attacks, error code differences that reveal information about data existence. Test: verify side-channel information leakage (side channels don't leak information), constant-time operations, uniform error messages.

**Initial state: system with encryption keys**: System starts with encryption keys configured. Action sequence: Check key storage (hardcoded, in logs, in version control). Test: verify encryption key exposure (keys stored securely), not exposed.

**Initial state: system with network communication**: System starts with network communication configured. Action sequence: Attacker intercepts network traffic. Test: verify data in transit interception (data encrypted in transit: TLS/SSL), interception doesn't reveal plaintext.

**Initial state: system with data at rest**: System starts with data stored (database, files). Action sequence: Attacker gains access to storage. Test: verify data at rest exposure (data encrypted at rest), plaintext not accessible.
