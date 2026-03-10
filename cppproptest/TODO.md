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
- **[x] Expose reproduction stats / result access for tests** — added `ReproductionStats` with `getLastReproductionStats()`, `setOnReproductionStats(...)`, and `setOnFailureReproduction(...)` in property/stateful APIs; validated in `test_shrink_retry.cpp` with structured assertions.
- **[x] Document Seed API update (user-facing)** — documented `Seed`/`gen::seed()` semantics and usage in user docs, including why to use it, `Seed(<n>)` output format, and no-shrink behavior.
- **[x] Skip redundant "simplest args found by shrinking" line** — when no shrink step succeeds, do not print `simplest args found by shrinking` for the original args; validated by `Property.shrink_output_omits_simplest_line_when_no_shrink_happened`.
- **[x] Shrinking with retry + timeout for forAll/stateful (non-deterministic tests)** — implemented retry-based shrink flow in `PropertyBase::shrink` with timeout/config support and stateful propagation (stateful uses forAll), including reproduction-rate reporting (`reproduction: X/Y in Zs`).
- **[x] Improve stateful shrink order (action list first)** — stateful internal `forAll` now orders tuple as `(actionList, initialObject)`, so existing tuple shrink naturally prioritizes action-list shrinking first.
- **[x] Shrinking with retry + timeout for concurrency path** — implemented retry/timeout-aware shrink handling in concurrency `handleShrink`, added concurrency shrink config setters (`setShrinkMaxRetries`, `setShrinkTimeoutMs`, `setShrinkRetryTimeoutMs`), and validated with `concurrency_function.shrink_with_retry_timeout_smoke`.
- **[x] Lower default action-list size + add stateful/concurrency list-size config** — reduced stateful/concurrency action-list default max to 20 and added explicit API knobs (`setActionListMinSize`, `setActionListMaxSize`, `setActionListSize`) for both paths; added validation tests (`stateful_function.action_list_size_configuration`, `concurrency_function.action_list_size_configuration`).
- **[x] Replace GTest internal stdout/stderr capture dependency with output-stream options** — added configurable output/error streams on property config (`ForAllConfig.outputStream`, `ForAllConfig.errorStream`) and Property APIs (`setOutputStream`, `setErrorStream`, `setOutputStreams`), routed `PropertyBase` logs through configured streams, and migrated capture-based tests in `test_property.cpp` to `std::stringstream` injection.
- **[x] Review `gen::construct` shrink behavior for stateful wrapper use** — reviewed `gen::construct` implementation (`construct -> tupleOf -> shrinkTupleUsingVector`) and tests; shrink order follows constructor argument order (left-to-right tuple element shrinking). This is suitable for a stateful wrapper if constructor args are ordered as `(actions, initial)`.
- **[x] Add labeled shrink output for stateful path (`initial`, `actions`)** — stateful path now uses a wrapper arg generated with `gen::construct` and labeled `show(...)` output, producing labeled stateful shrink args while preserving action-list-first shrinking via constructor order `(actions, initial)`. Validated by `stateful_function.shrink_output_uses_labeled_stateful_args`.
- **[x] Add explicit action names in stateful tests (avoid `Action<?>` in logs)** — updated stateful test generators to use named actions (`PROP_ACTION_NAME("PushBack", v)`, `"FailWhenNonEmpty"`, etc.) so shrink/failure output now shows concrete action names rather than `Action<?>`.

---

## In Progress

(none)

---

## Future Enhancements (ordered by complexity: simpler -> more complex)

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

### [ ] Migrate output-dependent tests to callback-driven assertions (where feasible)
- **Status**: `CaptureStdout`/`CaptureStderr` dependency is removed (replaced with output-stream injection); this remaining item is specifically about callback-first assertions.
- **Goal**: Prefer callback-based observability for tests that currently assert formatted output text, reducing coupling to log formatting.
- **Scope**: Expand callback surface where needed (while keeping APIs minimal), then migrate eligible output-dependent tests to callback assertions; keep stream-based assertions only for formatting-specific expectations.
- **Examples**: Reproduction/shrink callbacks, per-failure callbacks, and optional lifecycle/event callbacks if needed.
- **Location**: `PropertyBase`/stateful/concurrency callback APIs and tests under `proptest/test/*`.

### [ ] Improve argument-output formatting (flatten stateful braces + optional named forAll args)
- **Problem**: Stateful wrapper output currently appears with redundant nested braces (e.g., `with args: { { initial: ..., actions: ... } }`), and generic `forAll` output renders unnamed tuple-style args (e.g., `{ Seed(...), 2, 26 }`), which is less readable.
- **Goal**: (1) Flatten stateful rendering to a single brace layer, and (2) support named argument rendering in `forAll` output (e.g., `{ seed: ..., sleepMs: ..., failDenominator: ... }`) when names are provided.
- **Scope**: Refine `writeArgs`/`show` composition for wrapper types; design an opt-in parameter-name mechanism for `Property` output (without breaking existing API behavior).
- **Compatibility**: Keep current default formatting behavior unless explicit naming metadata is supplied.
- **Location**: `proptest/Property.hpp`, `proptest/PropertyBase.cpp`, stateful wrapper rendering in `proptest/stateful/stateful_function.hpp`, and relevant output tests.

### [ ] Add thin std-abstraction layer (time wrapper as first scope)
- **Parent task**: Centralize standard-library/backend indirection in `proptest/std` so call sites avoid direct dependency on specific std headers/backends.
- **Problem**: Direct std header usage (e.g., `<memory>`, `<concepts>`, `<cstdint>`, `<chrono>`) is spread across code paths, making future adaptation to alternative/backport 3rd-party libraries harder and increasing explicit `std::` coupling/conflict at call sites.
- **Goal**: Resolve std/backend adaptation in `proptest/std` dedicated wrappers only, while keeping external behavior unchanged.
- **Child tasks**:
  - [ ] **Thin time API wrapper**: introduce `Clock`/`TimePoint` helpers and refactor timeout/shrink elapsed-time checks to use `proptest/std` time wrapper.
  - [ ] **Std include boundary policy**: allow core std headers via dedicated wrappers under `proptest/std/*` only (e.g., wrappers for memory/concepts/inttypes/chrono as needed), then migrate call sites to wrapper includes.
- **Benefit**: Improves modularity, reduces direct `std::` coupling at call sites, and makes backend/library substitution localized to `proptest/std`.
- **Location**: `proptest/std/*` wrapper headers, `PropertyBase`, stateful/concurrency timeout logic, and dependent call sites/tests.

### [ ] Add validation test for stateful shrink-order behavior
- **Problem**: We changed stateful internal tuple order to prioritize action-list shrinking first, but there is no dedicated regression test that verifies this behavior.
- **Goal**: Add a stable, deterministic test that fails if shrink priority regresses away from action-list-first.
- **Notes**: Test design is non-trivial; revisit with a minimal, robust scenario that avoids brittle output-only assertions.
- **Location**: `proptest/test/test_stateful_function.cpp` (or `proptest/test/test_shrink_retry.cpp`).

### [ ] Add throw-on-failure execution mode for `forAll` / `matrix` (for better GTest integration)
- **Problem**: Current `forAll`/`matrix` return `bool` and print failure details to stderr. `EXPECT_FOR_ALL` can only assert the bool result, so Google Test cannot directly receive the property failure as an exception-driven failure flow.
- **Goal**: Provide an execution mode that throws on first property failure (e.g. `forAllThrow(...)` / option flag), so `EXPECT_FOR_ALL`-style helpers can use `EXPECT_NO_THROW` / `ASSERT_NO_THROW` (or equivalent) and rely on GTest-native failure propagation/reporting.
- **Possible API**: (1) New helpers `forAllThrow(...)` and `matrixThrow(...)`; (2) Config flag in `ForAllConfig`/Property config such as `.throwOnFailure = true`; (3) `property(...).setThrowOnFailure(true).forAll(...)`.
- **Error model**: Throw a typed exception containing failure metadata (condition, file, line, serialized args, maybe shrink/reproduction stats), so callers can format or rethrow consistently.
- **EXPECT/ASSERT macros**: Add experimental `EXPECT_FOR_ALL_THROW` / `ASSERT_FOR_ALL_THROW` (temporary naming) to validate viability before replacing existing macros.
- **Validation**: Add tests confirming thrown exception includes accurate file/line and args, and that GTest output is clearer than bool-based wrappers for inline lambdas.
- **Location**: `PropertyBase` run/shrink path, `Property.hpp` API/macros, docs in `PropertyAPI.md`.

### [ ] Chainable property tests with Google Test integration
- **Problem**: EXPECT_FOR_ALL, ASSERT_FOR_ALL, EXPECT_MATRIX, ASSERT_MATRIX wrap `forAll`/`matrix` in EXPECT_TRUE/ASSERT_TRUE. The macros are inherently not chainable: you cannot chain `.example(...)`, `.matrix(...)`, or another `.forAll(...)` after them. The expansion `EXPECT_TRUE(forAll(...))` consumes the result; the outer expression is EXPECT_TRUE, so `.example()` etc. cannot be appended. Verbosity of `EXPECT_TRUE(property(callable).setSeed(0).forAll(gen))` is acceptable, but that form still doesn't allow `.forAll(gen1).example(list).matrix(...)` — i.e. chaining multiple execution modes.
- **Goal**: Integrate with Google Test (EXPECT/ASSERT, failure format) while allowing chaining of forAll/example/matrix (e.g. `prop.forAll(gen1).example(list).forAll(gen2)` or similar).
- **Possible directions**: (1) Lazy wrapper that collects a chain of forAll/example/matrix; when evaluated (or `.run()`), executes the chain and feeds final result to EXPECT_TRUE; (2) Macro `EXPECT_PROPERTY(expr)` that accepts a full chain: `EXPECT_PROPERTY(property(callable).forAll(gen1).example(list))`; (3) Builder that returns a type with `.expect()` / `.assert()` methods, each returning a chainable builder.
- **Location**: Property.hpp (macros, forAll/matrix), PropertyAPI.md.

### [ ] Merge concurrency shrink output styles (detailed + compact fallback)
- **Problem**: Current concurrency shrink logs use compact placeholders (`<initial>`, `ActionList(size=...)`), while stateful shrink logs can show full values/actions when printable; outputs are inconsistent.
- **Goal**: Use a merged strategy: prefer detailed stateful-style output (full initial object and actions when printers are available), but automatically fall back to compact summaries when types are not printable or output is too long.
- **Scope**: Add configurable/automatic thresholding for long action lists and safe fallback formatting for non-printable object/action types.
- **Benefit**: Improves debugging fidelity while keeping logs robust and readable in large/non-printable cases.
- **Location**: concurrency shrink arg formatting in `proptest/stateful/concurrency_function.hpp` (and shared formatting helpers if extracted).

### [ ] Concurrency testing shrinking
- **Structure**: Tuple of 3: `(object, front, rears)` where `rears` is `list<list<Action>>`. No additional shrinker needed — list shrinker composes (outer list = fewer threads, inner lists = fewer actions per thread).
- **Shrink order**: Reverse dependency order — rears → front → object, so use tuple order `(rears, front, object)`.
- **Phases**: (1) **Assessment phase** — measure reproduction rate; probe relaxed/release/seq_cst, pick mode that reproduces (prefer seq_cst if it does, for total ordering). (2) **Shrinking phase** — use chosen mode.
- **Failure-point truncation**: Reliable for front (sequential), failed rear thread; unreliable for other rear threads. For other threads: log can lag behind true completion (CPU/compiler reordering). With `memory_order_release`, lag bounded to at most 1 action; with relaxed, unbounded. Use partial truncation where reliable; rely on shrink candidates for other rears.
- **ConcurrentTestDump**: Log tracks `markActionStart`/`markActionEnd` per thread for total ordering of enter/exit. Configurable memory order: relaxed (best reproducibility, unbounded lag), release (bounded lag), seq_cst (total ordering, may reduce reproducibility). Use relaxed by default; avoid default seq_cst.
- **Next**: Implement in `handleShrink`; same pattern as PropertyBase::shrink with retry/timeout; reuse same config.

### [ ] Add concurrency cross-list shrinking (rear -> front migration)
- **Problem**: Current concurrency shrinking mostly reduces each list independently (front and each rear list), but does not model relationships between lists.
- **Goal**: Add a shrink strategy that can move selected actions from rear lists (concurrent workflow) into front (serial workflow) when it preserves failure, to find simpler and more interpretable minimal cases.
- **Why this matters**: Some failures depend more on action content than exact placement; migrating rear actions to front can reduce schedule complexity while keeping the core failing behavior.
- **Complexity note**: This is likely a significant rework (not a small tuple-order tweak) and may require a new composite shrinker/search strategy over `(front, rears)` rather than independent list shrinking.
- **Location**: Concurrency shrink candidate generation in `proptest/stateful/concurrency_function.hpp`, potentially with new shared shrink utilities.

### [ ] Add multi-pass shrinking for tuples
- **Problem**: Current shrinking primarily mutates one argument axis at a time, which can limit simplification quality for tuple-shaped inputs.
- **Goal**: Support multi-pass shrinking for tuples.
- **Approach A (incremental)**: Build multi-pass behavior by composing existing tuple shrink streams with `andThen` / `concat` (pass 1 result -> pass 2 shrink stream -> ...), with stop conditions (no progress / timeout / max passes).
- **Approach B (new shrinker)**: Implement a dedicated tuple multi-pass shrinker that manages pass index/progress explicitly and emits candidates round-by-round.
- **Scope**: Add tuple-shrinker support in the shrinker/generator stack and wire it into the main shrink candidate flow.
- **Location**: tuple/shrinker utilities and `PropertyBase` shrink candidate flow.

### [ ] Add stateful multi-pass shrinking (deferred until tuple multi-pass support)
- **Problem**: A single pass over arguments may miss further simplification opportunities after the first successful reductions.
- **Goal**: Add alternating multi-pass shrinking for stateful inputs (action list → object → ...) until no progress or timeout.
- **Dependency**: Implement after `Add multi-pass shrinking for tuples`.
- **Location**: stateful integration and `proptest/PropertyBase.cpp` shrink loop strategy.

### [ ] Unify stateful/concurrency test frameworks
- **Idea**: Treat stateful testing as a constrained case of concurrency testing (e.g., no rear lists / single serial schedule) and share one core execution + shrinking engine.
- **Goal**: Reduce duplicated logic across stateful/concurrency paths (callbacks, failure reporting, shrinking/retry/timeout behavior, output formatting) while preserving current user-facing APIs.
- **Scope**: Identify a common internal representation for schedules and action execution, then layer stateful/concurrency builders on top.
- **Prerequisites (parity before unification)**:
  - Add stateful-equivalent reproduction observability to concurrency (`setOnReproductionStats`, `setOnFailureReproduction`, `getLastReproductionStats`).
  - Align failure semantics, especially buffered `PROP_EXPECT` / `PropertyContext` handling, so stateful and concurrency report failures consistently.
  - Align shrink regeneration semantics (use saved random state consistently in concurrency shrink path).
  - Design concurrency-aware stats/tag aggregation: collect per-thread stats/failures (thread-local or equivalent) during concurrent execution, then merge deterministically after join (with clear rules for counts, ordering, and thread attribution).
  - Align stat/tag summary behavior and callback surface (including action lifecycle hooks) or explicitly define intentional differences.
- **Risks**: Non-trivial refactor with behavior-regression risk; requires staged migration with compatibility tests.
- **Location**: `proptest/stateful/stateful_function.hpp`, `proptest/stateful/concurrency_function.hpp`, and shared internals to be extracted.

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
