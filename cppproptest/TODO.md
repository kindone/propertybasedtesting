# TODO: cppproptest2 Development Tasks

Tracks development tasks and feature enhancements for the C++ property-based testing framework (`cppproptest2`).

---

## Completed

- **[x] GitHub CI integration** — GitHub Actions CI for cppproptest2: multi-platform (Linux, macOS, Windows), multiple compilers (GCC, Clang, MSVC), C++20/C++23, property-based tests in CI. See `.github/workflows/ci.yml` in cppproptest2 repo.
- **[x] forAll config + Property::setConfig()** — C++20 designated-initializer config for `forAll()`, batch config via `Property::setConfig()`. Details: [archive/forAll_improvement_plan.md](archive/forAll_improvement_plan.md)
- **[x] EXPECT/ASSERT macros for .matrix()** — `EXPECT_MATRIX`, `ASSERT_MATRIX` in Property.hpp (callable + lists, like `forAll`); doc in PropertyAPI.md; tests in test_property.cpp.
- **[x] EXPECT/ASSERT macros for .example() and .matrix()** — `EXPECT_EXAMPLE`, `ASSERT_EXAMPLE`, `EXPECT_MATRIX`, `ASSERT_MATRIX` in Property.hpp; doc in PropertyAPI.md; tests in test_property.cpp.
- **[x] Chainable forAll / example / matrix with bool coercion** — `forAll`, `example`, `matrix` return `Property` by value; `operator bool()`; failure propagation; short-circuiting; config preserved; Chaining section in PropertyAPI.md.
- **[x] Generator::noShrink() / gen::noShrink()** — GeneratorBase::noShrink; gen::noShrink combinator; tests in test_combinator2.cpp; docs in Combinators.md and Generators.md.
- **[x] gen::oneOf — accept raw values (implicit just)** — GenToWeighted overload; OneOfArg concept; tests; docs in Combinators.md.
- **[x] StatefulProperty::setOnActionStart / setOnActionEnd** — Callbacks run before/after each action. Use `setOnActionEnd` for invariant checks (∀ ∀ ¬∃). Tests in test_stateful_function.cpp; doc in StatefulTesting.md.
- **[x] Stat assertion macros** — `PROP_STAT_ASSERT_GE`, `PROP_STAT_ASSERT_LE`, `PROP_STAT_ASSERT_IN_RANGE`; bounds ratio [0,1]; failure includes source location. Tests in test_property.cpp; doc in PropertyAPI.md.
- **[x] Generator named params Phase 1 (floating)** — `FloatGenConfig` for `Arbi<float>`, `Arbi<double>`; `Arbi<float>({.nanProb = 0.1})`; tests in test_floating.cpp; docs in Generators.md, Arbitrary.md.
- **[x] Generator named params Phase 2 (container)** — `ContainerGenConfig<T>` for `Arbi<vector<T>>`, `Arbi<list<T>>`, `Arbi<set<T>>`; `gen::vector<int>({.minSize = 5, .maxSize = 20})`; tests in test_arbitrary.cpp; docs in Generators.md, Arbitrary.md.
- **[x] Generator named params Phase 3 (string)** — `ContainerGenConfig<char>` for `Arbi<std::string>`, `ContainerGenConfig<uint32_t>` for `Arbi<UTF8String>`, `Arbi<UTF16BEString>`, `Arbi<UTF16LEString>`, `Arbi<CESU8String>`; tests in test_arbitrary.cpp; docs in Generators.md, Arbitrary.md.
- **[x] Generator named params Phase 4 (map)** — `MapGenConfig<K,V>` with `keyGen`, `valueGen`, `minSize`, `maxSize` for `Arbi<map<K,V>>`; `gen::map<int,int>({.keyGen = gen::int32(), .valueGen = gen::int32(), .minSize = 5, .maxSize = 20})`; tests in test_arbitrary.cpp; docs in Generators.md, Arbitrary.md.
- **[x] gen::weightedGen — accept raw values; unify with weightedVal** — `gen::weighted(value, prob)` and `gen::weighted(gen, prob)` as primary API; `weightedVal` and `weightedGen` as explicit alternatives; unified `weighted.hpp`; elementOf rejects `weighted(gen)` with clear static_assert; docs in Combinators.md, Generators.md, StatefulTesting.md.

---

## In Progress

(none)

---

## Future Enhancements (by suggested urgency: quick wins → medium → larger)

### [ ] Shrinking with retry + timeout (non-deterministic tests)
- **Problem**: Shrink assumes determinism; one run per candidate. Flaky/concurrent tests need retries and per-run timeout.
- **Proposed**: Configurable retries per candidate; policy (e.g. fail iff fails ≥ K of N); **timeout per shrink retry** (e.g. `shrinkRetryTimeoutMs`) so a single run doesn’t block. Config: Property/ForAllConfig or dedicated shrink-config (`shrinkRetries`, `shrinkTolerance`, `shrinkRetryTimeoutMs`).
- **Next**: Find shrink loop; design config + timeout behavior; retry loop + policy; flaky-property tests; Shrinking.md.

### [ ] Regex-based string generator
- `gen::regex(pattern)` (optional size/constraints). Use cases: email, phone, UUID, IP, custom formats.
- **Choices**: Engine (std::regex vs RE2/PCRE2 vs custom), generation (AST walk), shrinking (regex-aware), Unicode.
- **Phases**: Basic (classes, quantifiers, alternation) → groups/anchors → optim → advanced (lookahead etc.).
- **Next**: Research other PBT libs; pick engine; design algo + shrinking; prototype; tests.

### [ ] Fully property-based tested project + property derivation framework
- **Bold goal**: Make cppproptest2 itself fully property-based tested, replacing traditional example-based tests. Demonstrate that PBT can replace example-based testing.
- **Motivation**: If we can’t test our own PBT framework with properties, how can we claim PBT is superior? This is meta-testing: the framework tests itself.
- **Property derivation framework**: Build a model/methodology for deriving properties from:
  - **Software design** (invariants, pre/post conditions, design contracts)
  - **Documentation** (API docs, README, guides — extract stated behaviors)
  - **Specification** (formal/informal specs, requirements — derive properties)
  - **Comments** (inline docs, TODOs, design notes — mine for properties)
  - **References** (papers, other PBT libs, standards — cross-reference properties)
- **Approach**:
  - Audit existing example-based tests; identify what they verify; derive properties.
  - Document property derivation process (guidelines, patterns, examples).
  - **Subdomain exploration**: While properties can be extremely general, identify and test meaningful subdomains (edge cases, boundary conditions, specific patterns, error paths) to make tests more effective and efficient at finding issues and validating components.
    - **Weighted probabilities**: Instead of discrete subdomains, use weighted generators (e.g. `oneOf` with weights) to control probability distributions. Corner cases that appear rarely in general ranges can be given higher weights to appear more frequently, improving bug-finding efficiency.
    - **Hyperparameters**: Randomize the probability distributions themselves (e.g. randomly choose weights for subdomains). This explores different ways of exploring subdomains, making the test framework itself more robust and adaptive.
  - Create property test suite that covers same/more ground than examples.
  - Showcase: “This property came from the doc comment here; this one from the design doc.”
- **Phases**:
  1. Framework/methodology doc (how to derive properties from sources).
  2. Audit + convert core tests (Property, generators, shrinking).
  3. Expand to all components; document derivation sources.
  4. Showcase: side-by-side comparison (old example vs new property + source).
- **Next**: Draft property derivation framework doc; audit test_property.cpp; identify first conversion candidates; prototype methodology.
