# Subdomain Exploration

While properties can be extremely general, identifying and testing meaningful subdomains makes tests more effective and efficient at finding issues.

## Overview

Property-based testing generates random inputs from a domain, but not all parts of the domain are equally important. **Subdomain exploration** focuses testing effort on meaningful subsets:
- Edge cases (boundaries, empty inputs, extreme values)
- Error paths (invalid inputs, exception cases)
- Common patterns (typical usage scenarios)
- Rare but important cases (corner cases that matter)

## Discrete Subdomains

Traditional approach: divide domain into separate sets:

```cpp
// Separate generators for different subdomains
auto edgeCases = gen::oneOf(
    gen::just(0),
    gen::just(INT_MAX),
    gen::just(INT_MIN)
);
auto normalCases = gen::interval(1, INT_MAX - 1);
```

**Limitations**:
- Requires separate test runs for each subdomain
- May miss interactions between subdomains
- Less realistic than natural distributions

## Weighted Probabilities

Instead of discrete subdomains, use weighted generators to control probability distributions:

```cpp
// Corner cases appear more frequently than natural distribution
auto weightedGen = gen::oneOf(
    gen::weighted(0.3, gen::just(0)),           // 30% chance
    gen::weighted(0.2, gen::just(INT_MAX)),     // 20% chance
    gen::weighted(0.1, gen::just(INT_MIN)),     // 10% chance
    gen::weighted(0.4, gen::interval(1, 100))   // 40% chance
);
```

**Benefits**:
- Corner cases that appear rarely in general ranges get higher weights
- Improves bug-finding efficiency without completely excluding normal cases
- More realistic than pure discrete subdomains
- Single test run covers all subdomains with appropriate frequency

**Example**:
```cpp
// Test Property::forAll with weighted subdomains
property([](int val) {
    // Edge cases (0, INT_MAX, INT_MIN) appear more often
    PROP_EXPECT(val >= INT_MIN && val <= INT_MAX);
    return true;
}, gen::oneOf(
    gen::weighted(0.3, gen::just(0)),
    gen::weighted(0.2, gen::just(INT_MAX)),
    gen::weighted(0.1, gen::just(INT_MIN)),
    gen::weighted(0.4, gen::interval(1, 1000))
));
```

**Choosing weights**:
- Start with equal weights, then adjust based on bug-finding effectiveness
- Increase weights for subdomains where bugs are more likely
- Balance: too high weights exclude normal cases, too low miss corner cases
- Typical range: 10-30% for edge cases, 40-70% for normal cases

## Hyperparameters: Randomized Probability Distributions

Randomize the probability distributions themselves. This explores different ways of exploring subdomains, making the test framework more robust and adaptive:

```cpp
// Hyperparameter: randomly choose weights for subdomains
property([](double edgeWeight, double normalWeight) {
    // Normalize weights
    double total = edgeWeight + normalWeight;
    edgeWeight /= total;
    normalWeight /= total;

    auto gen = gen::oneOf(
        gen::weighted(edgeWeight, gen::just(0)),
        gen::weighted(normalWeight, gen::interval(1, 100))
    );

    // Test with this weight distribution
    auto val = gen.generate(Random());
    PROP_EXPECT(val >= 0 && val <= 100);
    return true;
}, gen::uniform(0.1, 0.9), gen::uniform(0.1, 0.9));
```

**Benefits**:
- Tests different exploration strategies automatically
- Discovers which probability distributions are most effective
- Makes the test framework itself more robust
- Can be used for automatic generator tuning
- Reduces manual weight tuning effort

**Advanced example**:
```cpp
// Hyperparameter exploration for generator tuning
property([](map<string, double> weights) {
    // Normalize weights
    double sum = 0;
    for (auto& [key, val] : weights) sum += val;
    for (auto& [key, val] : weights) val /= sum;

    // Create weighted generator
    auto gen = createWeightedGenerator(weights);

    // Test property with this distribution
    auto result = forAll([](int x) { return x >= 0; }, gen);
    PROP_EXPECT(result);
    return true;
}, gen::map(gen::string(), gen::uniform(0.0, 1.0)));
```

## Practical Guidelines

### When to Use Discrete Subdomains

- When subdomains are completely separate (e.g., valid vs invalid inputs)
- When you need guaranteed coverage of each subdomain
- When subdomains have incompatible types or constraints

### When to Use Weighted Probabilities

- When subdomains overlap or interact
- When you want realistic distributions with boosted corner cases
- When single test runs should cover multiple subdomains
- **Recommended default approach**

### When to Use Hyperparameters

- For critical properties that need thorough exploration
- When optimal weights are unknown
- For framework-level testing (testing the test framework itself)
- When you want adaptive, self-tuning tests

### Identifying Subdomains

For each property, identify meaningful subdomains:

1. **Edge cases**: Boundaries, empty inputs, extreme values
   - `0`, `INT_MIN`, `INT_MAX`, `nullptr`, empty containers
   
2. **Error paths**: Invalid inputs, exception cases
   - Negative values where positive expected
   - Out-of-range indices
   - Invalid state transitions

3. **Common patterns**: Typical usage scenarios
   - Small positive integers
   - Typical string lengths
   - Normal-sized containers

4. **Rare but important**: Corner cases that matter
   - Specific values that trigger special logic
   - Boundary conditions in algorithms
   - Race conditions in concurrent code

## Examples

### Example 1: Integer Range Testing

```cpp
// Discrete approach (less flexible)
auto edgeGen = gen::oneOf(gen::just(0), gen::just(INT_MAX), gen::just(INT_MIN));
auto normalGen = gen::interval(1, 1000);

// Weighted approach (recommended)
auto weightedGen = gen::oneOf(
    gen::weighted(0.2, gen::just(0)),
    gen::weighted(0.1, gen::just(INT_MAX)),
    gen::weighted(0.1, gen::just(INT_MIN)),
    gen::weighted(0.6, gen::interval(1, 1000))
);
```

### Example 2: Container Testing

```cpp
// Weighted subdomains for containers
auto containerGen = gen::vector(gen::int32(), gen::oneOf(
    gen::weighted(0.3, gen::just(0)),      // Empty containers
    gen::weighted(0.2, gen::just(1)),      // Single element
    gen::weighted(0.1, gen::just(1000)),   // Large containers
    gen::weighted(0.4, gen::interval(2, 100))  // Normal sizes
));
```

### Example 3: String Testing

```cpp
// Weighted subdomains for strings
auto stringGen = gen::oneOf(
    gen::weighted(0.2, gen::just("")),           // Empty string
    gen::weighted(0.1, gen::string(1000)),        // Long string
    gen::weighted(0.1, gen::string(gen::just('\0'))),  // Null bytes
    gen::weighted(0.6, gen::string(gen::interval(1, 100)))  // Normal strings
);
```

## Research References

- **"Tuning Random Generators: Property-Based Testing as Probabilistic Programming"**: Automatic generator tuning and weight optimization
- **"Loaded Dice"**: Automatic weight learning for generators
- **Etna platform**: Empirical evaluation of different PBT techniques and generator strategies

## Conclusion

Subdomain exploration is crucial for effective property-based testing. While discrete subdomains have their place, **weighted probabilities** provide the best balance of bug-finding efficiency and realistic testing. **Hyperparameters** take this further by automatically exploring different probability distributions, making tests more robust and adaptive.

The key is identifying meaningful subdomains and using appropriate techniques to ensure they're tested with the right frequency.
