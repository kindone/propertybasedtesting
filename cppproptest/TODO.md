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
- **[x] PROP_ASSERT uses single-eval refs** — `PROP_ASSERT_EQ/NE/LT/GT/LE/GE` and `PROP_ASSERT_STREQ/STRNE` variants now bind args as `const auto&` (no extra copies, single evaluation). Includes validation test `Property.prop_assert_macros_evaluate_arguments_once`.
- **[x] PROP_EXPECT/PROP_ASSERT display original expressions** — failure messages now show source expressions (`x == y`) rather than internal helper names (`__prop_a`). Implemented via `displayCondition` in `PROP_EXPECT_STREAM` / `PROP_ASSERT_STREAM`; validated by dedicated tests.
- **[x] EXPECT_FOR_ALL failure output cleanup** — migrated wrappers to public GTest predicate-format assertions (`EXPECT_PRED_FORMAT1` / `ASSERT_PRED_FORMAT1`) with custom `AssertionResult` messages; avoids lambda body dumps and internal variable names.
- **[x] Optional config members for Property/StatefulProperty** — replaced sentinel values with `optional<T>` in config paths (`seed`, run limits, shrink retry settings), removing sentinel collisions and clarifying unset/default semantics.
- **[x] Seed type + unbiased seed generator** — added `Seed` wrapper type and `gen::seed()` generator (uniform `uint64_t`, no shrinking, no boundary bias from integral generators). Output now renders as `Seed(<n>)` in argument sets; shrink-retry tests migrated from `gen::uint64().noShrink()`.

---

## In Progress

(none)

---

## Future Enhancements (by suggested urgency: quick wins → medium → larger)

### [ ] Chainable property tests with Google Test integration
- **Problem**: EXPECT_FOR_ALL, ASSERT_FOR_ALL, EXPECT_MATRIX, ASSERT_MATRIX wrap `forAll`/`matrix` in EXPECT_TRUE/ASSERT_TRUE. The macros are inherently not chainable: you cannot chain `.example(...)`, `.matrix(...)`, or another `.forAll(...)` after them. The expansion `EXPECT_TRUE(forAll(...))` consumes the result; the outer expression is EXPECT_TRUE, so `.example()` etc. cannot be appended. Verbosity of `EXPECT_TRUE(property(callable).setSeed(0).forAll(gen))` is acceptable, but that form still doesn't allow `.forAll(gen1).example(list).matrix(...)` — i.e. chaining multiple execution modes.
- **Goal**: Integrate with Google Test (EXPECT/ASSERT, failure format) while allowing chaining of forAll/example/matrix (e.g. `prop.forAll(gen1).example(list).forAll(gen2)` or similar).
- **Possible directions**: (1) Lazy wrapper that collects a chain of forAll/example/matrix; when evaluated (or `.run()`), executes the chain and feeds final result to EXPECT_TRUE; (2) Macro `EXPECT_PROPERTY(expr)` that accepts a full chain: `EXPECT_PROPERTY(property(callable).forAll(gen1).example(list))`; (3) Builder that returns a type with `.expect()` / `.assert()` methods, each returning a chainable builder.
- **Location**: Property.hpp (macros, forAll/matrix), PropertyAPI.md.

### [ ] Add throw-on-failure execution mode for `forAll` / `matrix` (for better GTest integration)
- **Problem**: Current `forAll`/`matrix` return `bool` and print failure details to stderr. `EXPECT_FOR_ALL` can only assert the bool result, so Google Test cannot directly receive the property failure as an exception-driven failure flow.
- **Goal**: Provide an execution mode that throws on first property failure (e.g. `forAllThrow(...)` / option flag), so `EXPECT_FOR_ALL`-style helpers can use `EXPECT_NO_THROW` / `ASSERT_NO_THROW` (or equivalent) and rely on GTest-native failure propagation/reporting.
- **Possible API**: (1) New helpers `forAllThrow(...)` and `matrixThrow(...)`; (2) Config flag in `ForAllConfig`/Property config such as `.throwOnFailure = true`; (3) `property(...).setThrowOnFailure(true).forAll(...)`.
- **Error model**: Throw a typed exception containing failure metadata (condition, file, line, serialized args, maybe shrink/reproduction stats), so callers can format or rethrow consistently.
- **EXPECT/ASSERT macros**: Add experimental `EXPECT_FOR_ALL_THROW` / `ASSERT_FOR_ALL_THROW` (temporary naming) to validate viability before replacing existing macros.
- **Validation**: Add tests confirming thrown exception includes accurate file/line and args, and that GTest output is clearer than bool-based wrappers for inline lambdas.
- **Location**: `PropertyBase` run/shrink path, `Property.hpp` API/macros, docs in `PropertyAPI.md`.

### [x] Skip "simplest args found by shrinking" when shrunk args are identical to original
- **Problem**: When shrinking finds nothing simpler (e.g. gen::uint64().noShrink(), or shrink stream empty), we print both "with args: { X }" and "simplest args found by shrinking: { X }" — redundant.
- **Fix**: Track whether any shrink step succeeded; omit the "simplest args found by shrinking" line when no shrink occurred.
- **Location**: PropertyBase.cpp shrink loop; add `anyShrinkFound` flag.
- **Completed**: Added `anyShrinkFound` tracking in `PropertyBase::shrink()` and print the "simplest args found by shrinking" line only when at least one shrink step succeeds.
- **Validation**: Added `Property.shrink_output_omits_simplest_line_when_no_shrink_happened` in `proptest/test/test_property.cpp` to verify the line is omitted for `noShrink` failure cases.

### [ ] assessFailureForRetry: respect shrink phase timeout
- **Problem**: `assessFailureForRetry` runs `kShrinkAssessmentRuns` (10) unconditionally and does not check `shrinkTimeoutMs`. For slow tests (e.g. 300ms per run), assessment alone takes 3s and can exceed the shrink phase budget (e.g. shrinkTimeoutMs=2000). The shrink loop then exits immediately; assessment dominates and ignores the configured limit.
- **Fix**: Pass `phaseStart` and `shrinkTimeoutMs` to `assessFailureForRetry`; check `isShrinkPhaseTimedOut(phaseStart, shrinkTimeoutMs)` before each assessment run. Abort early when timed out, reporting partial stats (failCount, totalRuns so far).
- **Location**: PropertyBase.cpp `assessFailureForRetry`, `shrink()` (call site).
- **Edge case**: If we abort after 0 runs, ReproductionStats would have totalRuns=0; consider whether that's valid or we need a minimum (e.g. at least 1 run before reporting).

### [ ] Improve test signal/noise and timeout-test runtime
- **Problem**: Current shrink-retry/stateful tests produce excessive log output and timeout-focused cases take longer than needed, making CI diagnosis noisy and slower.
- **Goal**: Reduce non-essential test output (while preserving actionable failure diagnostics) and shorten timeout-related tests without reducing coverage intent.
- **Possible directions**: Lower default verbosity in test-only paths, gate detailed messages behind a debug flag, tune timeout test parameters/runs, and prefer deterministic assertions over broad retries where feasible.
- **Location**: `proptest/test/test_shrink_retry.cpp`, related test utilities, and CI expectations/docs.

### [ ] Document recent behavior and API updates
- **Problem**: Several recent improvements are implemented but not yet fully documented together, which makes behavior harder to understand for contributors/users.
- **Scope (user-facing only)**: Document public behavior/API only: nested property failure/reporting behavior visible to users (`forAll` + `EXPECT_FOR_ALL` interactions), `Seed` wrapper and `gen::seed()` semantics, and user-visible shrink-retry reporting interpretation (`numReproduced` in output/API).
- **Goal**: Add/refresh docs so users can quickly understand expected public behavior, output, and recommended usage patterns.
- **Location**: `docs/PropertyAPI.md`, `docs/Shrinking.md`.

### [x] Shrinking with retry + timeout for forAll/stateful (non-deterministic tests)
- **Problem**: Shrink assumed determinism (single run per candidate), which was insufficient for flaky behavior.
- **Completed scope**: Implemented retry-based shrink flow in `PropertyBase::shrink` with timeout/config support, then propagated through stateful path (stateful uses forAll). Added tests and docs for this scope.
- **Config**: `shrinkMaxRetries`, `shrinkTimeoutMs` (total phase), `shrinkRetryTimeoutMs` (per-candidate); adaptive retry behavior enabled by default.
- **Reporting**: Reproduction-rate reporting is included (e.g. `reproduction: X/Y in Zs`) for initial failure / successful shrink assessments.

### [ ] Shrinking with retry + timeout for concurrency path
- **Problem**: Concurrency shrink path is separate and still incomplete; `handleShrink` remains the missing integration point.
- **Goal**: Apply the same retry/timeout approach in concurrency shrinking, with concurrency-specific handling for failure-point truncation and memory-order effects.
- **Next**: Implement in `handleShrink`; reuse the same config surface and align behavior with PropertyBase shrink retry flow.

### [ ] Expose reproduction stats / result access for tests
- **Problem**: Shrink retry tests currently capture stdout/stderr and parse output (e.g. regex for `reproduction: X/Y in Zs`). This is fragile (format changes break tests), not thread-safe, and couples tests to presentation.
- **Goal**: Provide programmatic access to shrink/failure stats so tests can assert on structured data instead of parsing output.
- **Rationale**: Fits the existing pull-style API: `prop.forAll()` returns a result; tests inspect it. No callbacks, no output capture.
- **Design details** (to be refined):
  - **What to expose**:
    - `ReproductionStats`: `{ reproductions (X), totalRuns (Y), timeSec, argsAsString }` — from assessment phase
    - `argsAsString`: serialized args (e.g. `"{ 0 }"`) — simple, matches current output format; for re-run tests, caller can parse or use as-is for assertions
  - **Single + callback compromise**:
    - **Single**: `optional<ReproductionStats> getLastReproductionStats() const` — latest only, no storage overhead when unused.
    - **Callback**: `setOnReproductionStats(std::function<void(ReproductionStats)>)` — called upon each assessment (initial failure, each successful shrink if `kShrinkAssessOnEachSuccess`). Consumer can accumulate into a list if needed; framework does not store a list.
  - **Per-failure callback** (for detailed error collection):
    - `setOnFailureReproduction(std::function<void(int assessmentIndex, const vector<Any>& args, const string& errorMsg)>)` — called on each individual failure during assessment (e.g. 5 times if X=5 in "5/10"). `assessmentIndex`: 0 = initial failure, 1 = first successful shrink, etc. `args`: the actual args used for that run. Enables collecting all error messages, stack traces, etc. Optional; no cost when not set.
  - **When populated**: Only after a failure that triggered shrink; only when `shrinkMaxRetries > 0` (retry mode). Deterministic shrink has no assessment; `getLastReproductionStats()` returns `nullopt`.
  - **Stateful**: Propagate through same mechanism; stateful uses PropertyBase internally.
- **Test impact**: Tests in `test_shrink_retry.cpp` could replace `captureOutput` + regex with `EXPECT_GE(result.getLastReproductionStats()->reproductions, 1)` etc.
- **Open questions**:
  - Backward compatibility: keep printing to cout/cerr; result access is additive only.
- **Next**: Finalize struct design; implement; refactor tests.

### [ ] Concurrency testing shrinking
- **Structure**: Tuple of 3: `(object, front, rears)` where `rears` is `list<list<Action>>`. No additional shrinker needed — list shrinker composes (outer list = fewer threads, inner lists = fewer actions per thread).
- **Shrink order**: Reverse dependency order — rears → front → object, so use tuple order `(rears, front, object)`.
- **Phases**: (1) **Assessment phase** — measure reproduction rate; probe relaxed/release/seq_cst, pick mode that reproduces (prefer seq_cst if it does, for total ordering). (2) **Shrinking phase** — use chosen mode.
- **Failure-point truncation**: Reliable for front (sequential), failed rear thread; unreliable for other rear threads. For other threads: log can lag behind true completion (CPU/compiler reordering). With `memory_order_release`, lag bounded to at most 1 action; with relaxed, unbounded. Use partial truncation where reliable; rely on shrink candidates for other rears.
- **ConcurrentTestDump**: Log tracks `markActionStart`/`markActionEnd` per thread for total ordering of enter/exit. Configurable memory order: relaxed (best reproducibility, unbounded lag), release (bounded lag), seq_cst (total ordering, may reduce reproducibility). Use relaxed by default; avoid default seq_cst.
- **Next**: Implement in `handleShrink`; same pattern as PropertyBase::shrink with retry/timeout; reuse same config.

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
