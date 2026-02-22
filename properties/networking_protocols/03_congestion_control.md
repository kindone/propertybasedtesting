# Congestion Control Property

**Property**: System adapts to network conditions. Protocol adjusts behavior based on congestion.

## Concept

Test that protocol implements congestion control: system adapts to network congestion. Verify:
- System detects congestion
- System adapts behavior (e.g., reduces send rate)
- System recovers when congestion clears
- Adaptation is effective

**Key idea**: Protocol should adapt to network conditions to avoid congestion and maintain performance.

## Actions

Focus on congestion scenarios:

- **Initial state generation**: Generate initial protocol state (normal network conditions, high send rate, low send rate, connections established). Initial state determines what congestion scenarios are possible.
- **Action sequences with parameters**: Generate message sending sequences with specific parameters (send rates, message sizes, priorities). Action sequences test congestion control across different network conditions.
- **Congestion detection**: Simulate congestion indicators: (1) packet loss (drop packets with probability P, e.g., 5%), (2) increased latency (delay packets by T, e.g., 100ms). Monitor if system detects congestion (e.g., detects loss, measures RTT)
- **Adaptation scenarios**: After congestion detected, monitor system behavior: (1) send rate (messages per second), (2) window size (if applicable), (3) backoff behavior
- **Recovery scenarios**: After congestion clears (reduce loss probability, reduce delays), monitor system behavior: (1) send rate increases, (2) window size increases
- **Simulated network conditions**: Implement mock network layer: (1) congestion: high loss rate (10-20%), high latency (200ms+), (2) normal: low loss (1%), low latency (10ms)

**Concrete plan**:
1. Generate initial protocol state (normal network conditions, connections established)
2. Implement mock network layer with configurable loss rate and latency
3. Generate message sending sequences with parameters (continuous stream of messages with high rate, sizes, priorities)
4. Simulate congestion: set high loss rate (10%) and high latency (200ms)
5. Monitor: send rate, loss rate, latency
6. Verify: system detects congestion and adapts (reduces send rate)
7. Clear congestion: reduce loss and latency
8. Verify: system recovers (increases send rate)

## Checks

Verify congestion control:

- **Congestion detection**: During congestion (high loss/latency), monitor system → assert system detects congestion (e.g., loss rate > threshold, RTT > threshold). Check detection mechanism (e.g., ACK timeout, duplicate ACKs)
- **Adaptation**: After congestion detected, measure send rate over time → assert send rate decreases (e.g., reduces by 50% within period T). Monitor adaptation mechanism (e.g., window size reduction, rate limiting)
- **Recovery**: After congestion clears (loss/latency reduced), measure send rate over time → assert send rate increases (e.g., increases gradually, reaches normal rate)
- **Effectiveness**: Compare congestion indicators before/after adaptation → assert: (1) loss rate decreases after adaptation, (2) system maintains reasonable performance (doesn't reduce rate too aggressively)

**Concrete checks**:
1. Baseline: measure send rate R_baseline under normal conditions
2. Apply congestion: set high loss (10%), high latency (200ms)
3. Monitor: measure send rate R_congested → assert R_congested < R_baseline (rate reduced)
4. Monitor: measure loss rate → assert loss rate decreases after adaptation
5. Clear congestion: reduce loss and latency
6. Monitor: measure send rate R_recovered → assert R_recovered increases toward R_baseline
7. Assert: adaptation effective (congestion reduced, performance maintained)

## Subdomains

**Initial state: protocol in normal conditions**: Protocol starts in normal network conditions (low loss, low latency). Action sequence: Network conditions change rapidly from normal to congested (high loss, high latency). Test: verify sudden congestion onset (protocol detects congestion quickly), adapts rate promptly.

**Initial state: protocol in congested state**: Protocol starts in congested state (high loss, high latency, reduced rate). Action sequence: Network congestion clears, conditions return to normal. Test: verify congestion recovery (protocol increases rate gradually), not too aggressive, reaches optimal rate.

**Initial state: protocol in congested state**: Protocol starts in congested state. Action sequence: Network remains congested for extended period. Test: verify persistent congestion (protocol maintains reduced rate), doesn't oscillate, doesn't starve.

**Initial state: protocol with multiple flows**: Protocol starts with multiple senders/flows active. Action sequence: Multiple senders compete for bandwidth during congestion. Test: verify multiple flows competing (fair sharing: each flow gets fair share), no flow starves others.

**Initial state: protocol in congested state**: Protocol starts in congested state. Action sequence: Congestion causes packet reordering (not just loss/delay). Test: verify congestion with packet reordering (protocol distinguishes reordering from loss), adapts correctly.

**Initial state: protocol using AIMD**: Protocol starts using AIMD for congestion control. Action sequence: Network conditions vary (congestion detected, then cleared). Test: verify AIMD behavior (increases rate additively when no congestion), decreases multiplicatively when congestion detected.
