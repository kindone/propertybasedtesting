# Integrity Property

**Property**: Data not tampered with. Tampering is detected and prevented.

## Concept

Test that system maintains integrity: data is not tampered with, and tampering is detected. Verify:
- Data not modified by unauthorized users
- Tampering detected
- Data integrity maintained
- Integrity checks work correctly

**Key idea**: System should detect and prevent unauthorized data modification.

## Actions

Focus on data modification scenarios:

- **Initial state generation**: Generate initial system state (data stored, integrity checks configured, signatures/checksums computed, tampering detection enabled). Initial state determines what integrity scenarios are possible.
- **Action sequences with parameters**: Generate data modification sequences with specific parameters (users, data types, modification types, tampering methods). Action sequences test integrity across different modification scenarios.
- **Authorized modification**: Authenticate authorized user U (has write permission). Modify data D → verify succeeds, modification applied
- **Unauthorized modification**: Authenticate unauthorized user U (no write permission). Attempt to modify data D → verify denied, modification rejected
- **Tampering scenarios**: Test tampering: (1) modify data without authentication, (2) modify data with invalid signature, (3) replay old modifications, (4) modify data in transit
- **Integrity checks**: Test integrity verification: (1) checksums/hashes verified, (2) signatures verified, (3) tampering detected

**Concrete plan**:
1. Generate initial system state (data stored, integrity checks configured, signatures/checksums computed)
2. Authenticate users: authorized (write permission), unauthorized (no write permission)
3. Generate data modification sequences with parameters (users, data types, modification types, tampering methods)
4. Attempt data modifications with each user type
5. Test tampering: modify without auth, invalid signatures, replay attacks
6. Verify integrity checks: checksums, signatures, tampering detection
7. Test data integrity: verify data not corrupted

Verify integrity:

- **No tampering**: Unauthorized user attempts modification → assert denied, modification rejected, data unchanged
- **Tampering detected**: Tamper with data (modify without auth, invalid signature) → assert detected, modification rejected, error returned
- **Integrity maintained**: After authorized modification, verify: (1) modification applied correctly, (2) data consistent, (3) integrity checks pass (checksums, signatures valid)
- **Checks work**: Verify integrity checks: (1) checksums/hashes computed and verified, (2) signatures verified, (3) tampering detected and rejected

**Concrete checks**:
1. Unauthorized modification: unauthorized user modifies data → assert denied, data unchanged
2. Tampering: tamper with data (modify, invalid signature) → assert detected, rejected
3. Integrity checks: verify checksums/hashes computed and verified
4. Signatures: verify signatures computed and verified
5. Data consistency: after modifications, verify data consistent (no corruption)
6. Replay attacks: replay old modification → assert rejected (if applicable)
7. Assert: integrity maintained (data not tampered, tampering detected)

## Subdomains

**Initial state: system with data and integrity checks**: System starts with data stored and integrity checks configured. Action sequence: Attacker modifies data without valid credentials. Test: verify data tampering without authentication (modifications rejected), integrity checks detect tampering.

**Initial state: system with nonces/timestamps**: System starts with replay detection configured (nonces, timestamps). Action sequence: Attacker replays old valid modification request. Test: verify replay attacks (replay detection: nonces, timestamps), old requests rejected.

**Initial state: system with data in transit**: System starts with data being transmitted. Action sequence: Attacker intercepts and modifies data in transit. Test: verify man-in-the-middle tampering (integrity checks: signatures, MACs detect tampering), modified data rejected.

**Initial state: system with hash-based integrity**: System starts with hash-based integrity checks. Action sequence: Attacker creates different data with same hash. Test: verify hash collision attacks (strong hash algorithms used), collision-resistant hashes.

**Initial state: system with signature-based integrity**: System starts with signature-based integrity checks. Action sequence: Attacker attempts to forge signatures or modify signed data. Test: verify signature forgery (signature verification), forged signatures rejected.

**Initial state: system with concurrent access**: System starts with multiple authorized users able to modify data. Action sequence: Multiple authorized users modify same data concurrently. Test: verify integrity during concurrent modifications (integrity maintained), conflicts resolved correctly, no corruption.
