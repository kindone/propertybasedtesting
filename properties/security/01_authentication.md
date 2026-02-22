# Authentication Property

**Property**: Only authorized users can access. Unauthorized access attempts are rejected.

## Concept

Test that system enforces authentication: only users with valid credentials can access. Verify:
- Valid credentials grant access
- Invalid credentials denied
- Unauthorized access blocked
- Authentication mechanisms work correctly

**Key idea**: System should verify user identity before granting access.

## Actions

Focus on authentication scenarios:

- **Initial state generation**: Generate initial authentication system state (no users, users with valid credentials, users with expired credentials, rate limiting state). Initial state determines what authentication scenarios are possible.
- **Action sequences with parameters**: Generate authentication attempt sequences with specific parameters (usernames, passwords, tokens, credential formats). Action sequences test authentication across different credential types and attack scenarios.
- **Valid credentials**: Generate valid credentials (username/password pairs from known set). Attempt login → verify succeeds
- **Invalid credentials**: Generate invalid credentials: (1) wrong password, (2) non-existent username, (3) malformed credentials (empty, special characters)
- **Unauthorized attempts**: Attempt access without authentication: (1) no credentials provided, (2) invalid token, (3) expired token
- **Credential scenarios**: Test various formats: (1) different password lengths, (2) special characters, (3) unicode characters, (4) SQL injection attempts, (5) XSS attempts

**Concrete plan**:
1. Generate initial authentication system state (no users, users with credentials, rate limiting state)
2. Generate authentication attempt sequences with parameters (usernames, passwords, tokens, formats)
3. Generate credentials: valid (from known set), invalid (wrong password, non-existent user), malformed (empty, special chars)
4. Attempt authentication with each credential type
5. Track authentication results: success/failure
6. Verify access: after authentication, attempt protected operation
7. Test edge cases: SQL injection, XSS, long passwords, unicode

## Checks

Verify authentication:

- **Valid access**: For valid credentials, assert: (1) authentication succeeds (returns success/token), (2) access to protected resources granted, (3) session established (if applicable)
- **Invalid denial**: For invalid credentials, assert: (1) authentication fails (returns error), (2) access denied, (3) no session created, (4) error message doesn't leak information (e.g., "user exists" vs "wrong password")
- **Unauthorized blocked**: For unauthorized attempts (no credentials, invalid token), assert: (1) access denied, (2) error returned (401 Unauthorized), (3) no information leaked
- **Mechanism correctness**: Verify authentication mechanism: (1) passwords hashed (not stored plaintext), (2) tokens signed/encrypted, (3) rate limiting (prevent brute force), (4) secure storage

**Concrete checks**:
1. Valid credentials: authenticate → assert success, token returned, access granted
2. Invalid credentials: authenticate → assert failure, error returned, access denied
3. Unauthorized: access without auth → assert 401 error, access denied
4. Security: verify passwords hashed, tokens signed, rate limiting active
5. Information leakage: check error messages → assert don't reveal user existence
6. Edge cases: SQL injection, XSS → assert blocked, no injection successful

## Subdomains

**Initial state: system with users and rate limiting**: System starts with users registered and rate limiting configured. Action sequence: Many failed login attempts with different passwords. Test: verify brute force attack resistance (rate limiting prevents brute force), account locked after threshold.

**Initial state: system with users**: System starts with users registered. Action sequence: Attempts using leaked credentials from other systems. Test: verify credential stuffing attacks (system rejects invalid credentials), doesn't leak information about user existence.

**Initial state: system with active sessions**: System starts with active sessions (valid tokens). Action sequence: Attacker intercepts session token, attempts to use it. Test: verify session hijacking prevention (tokens invalidated on suspicious activity), session security maintained.

**Initial state: system with password policy**: System starts with password policy configured. Action sequence: Attempts with weak passwords, common passwords, dictionary words. Test: verify password policy enforcement (password policy enforced), weak passwords rejected.

**Initial state: system with MFA enabled**: System starts with MFA enabled for users. Action sequence: Attacker has password but not second factor. Test: verify multi-factor authentication (MFA enforced), single factor insufficient.

**Initial state: system with tokens near expiration**: System starts with tokens near expiration. Action sequence: Valid token expires, refresh token used. Test: verify token expiration and refresh (expired tokens rejected), refresh mechanism works securely.
