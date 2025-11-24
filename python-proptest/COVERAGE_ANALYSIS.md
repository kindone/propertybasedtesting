# Code Coverage Analysis for python-proptest

**Generated:** 2025-01-20  
**Overall Coverage:** 92%

## Summary

The codebase has **92% overall coverage**, which is excellent. However, there are several files with coverage below 90% that may contain untested or outdated code.

## Files with Lower Coverage (< 90%)

### 1. `core/decorators.py` - 84% Coverage
- **Missing:** 22 statements
- **Partial Branches:** 15
- **Total Statements:** 188
- **Coverage:** 84%

**Analysis:**
- This is the decorator API module, which handles `@for_all`, `@example`, `@settings`, `@matrix`, etc.
- The missing coverage likely includes:
  - Edge cases in context detection (unittest vs pytest vs standalone)
  - Error handling paths
  - Matrix decorator edge cases
  - Settings override scenarios
  - Complex decorator chaining scenarios

**Recommendations:**
- Add tests for edge cases in context detection (when class detection fails)
- Test error paths when decorators are misused
- Add tests for complex decorator combinations
- Test matrix decorator with various input types

### 2. `core/try_.py` - 85% Coverage
- **Missing:** 16 statements
- **Total Statements:** 105
- **Coverage:** 85%

**Analysis:**
- This module provides `Try` monad for error handling
- Missing coverage likely includes:
  - Error recovery paths
  - Complex chaining scenarios
  - Edge cases in `attempt()` function

**Recommendations:**
- Add tests for error recovery scenarios
- Test complex `Try` chaining operations
- Test edge cases in `attempt()` function

### 3. `core/either.py` - 88% Coverage
- **Missing:** 9 statements
- **Partial Branches:** 1
- **Total Statements:** 80
- **Coverage:** 88%

**Analysis:**
- Provides `Either` monad (Left/Right)
- Missing coverage likely includes:
  - Edge cases in `map`, `flat_map`, `fold` operations
  - Error handling paths

**Recommendations:**
- Add tests for edge cases in monadic operations
- Test error propagation scenarios

### 4. `core/option.py` - 89% Coverage
- **Missing:** 8 statements
- **Total Statements:** 72
- **Coverage:** 89%

**Analysis:**
- Provides `Option` monad (Some/None)
- Missing coverage likely includes:
  - Edge cases in monadic operations
  - Error handling paths

**Recommendations:**
- Add tests for edge cases in `Option` operations
- Test `None_` vs `none` usage scenarios

### 5. `core/stateful.py` - 88% Coverage
- **Missing:** 8 statements
- **Partial Branches:** 5
- **Total Statements:** 82
- **Coverage:** 88%

**Analysis:**
- Provides stateful property testing
- Missing coverage likely includes:
  - Complex state machine scenarios
  - Error recovery in stateful tests
  - Edge cases in action generation

**Recommendations:**
- Add tests for complex stateful property scenarios
- Test error handling in stateful tests
- Test edge cases in action generation

## Files with Good Coverage (≥ 90%)

- `__init__.py`: 100%
- `core/__init__.py`: 100%
- `core/generator.py`: 95% (16 missing, but this is a backward compatibility layer)
- `core/property.py`: 94% (6 missing)
- `core/shrinker.py`: 93% (12 missing, backward compatibility layer)
- `core/stream.py`: 94% (3 missing)

## Notes on Code Structure

**Important:** The HTML coverage report appears to be from an older version of the codebase. The project has been refactored to have separate directories:

- `core/generator/` - Generator implementations
- `core/shrinker/` - Shrinker implementations  
- `core/combinator/` - Combinator implementations

The `generator.py` and `shrinker.py` files in the root of `core/` are now primarily backward compatibility layers that re-export from the new modular structure.

## Specific Untested Code Areas

### Decorators (`decorators.py` - 84% coverage)

**Existing Tests:**
- `tests/decorators/test_decorator_api.py` - Basic decorator usage
- `tests/decorators/test_example_decorator.py` - Example decorator
- `tests/decorators/test_matrix_decorator.py` - Matrix decorator
- `tests/decorators/test_decorator_combinations.py` - Decorator combinations
- `tests/decorators/test_run_for_all_dual.py` - Dual mode testing

**Likely Untested Areas (22 missing statements):**
1. **Context Detection Edge Cases:**
   - When class detection fails (lines 84-92)
   - When `__self__` is not available
   - When module lookup fails
   - Import errors for unittest (line 100-101)

2. **Error Handling:**
   - ValueError when generator count doesn't match parameters (lines 112-116)
   - Exception handling in wrapper functions (lines 200-230)

3. **Decorator Chaining:**
   - Complex decorator combinations
   - Settings override scenarios
   - Matrix decorator edge cases

4. **`run_property_test` function** (line 419) - May not be fully tested

### Try Monad (`try_.py` - 85% coverage)

**Existing Tests:** None found in test directory structure

**Likely Untested Areas (16 missing statements):**
1. **Error Recovery:**
   - `recover()` method on `Success` (line 97)
   - `recover()` method on `Failure` (line 145)
   - Complex recovery scenarios

2. **Filter Operations:**
   - `filter()` on `Success` (line 100)
   - `filter()` on `Failure` (line 151)

3. **Edge Cases:**
   - `get_exception()` on `Success` (line 82) - should raise
   - `get()` on `Failure` (line 130) - should raise
   - Complex chaining scenarios

4. **`attempt()` function** (line 165) - May have untested error paths

### Either Monad (`either.py` - 88% coverage)

**Existing Tests:** None found in test directory structure

**Likely Untested Areas (9 missing statements):**
1. Error handling paths in monadic operations
2. Edge cases in `fold`, `map`, `flat_map` operations
3. Complex chaining scenarios

### Option Monad (`option.py` - 89% coverage)

**Existing Tests:** None found in test directory structure

**Likely Untested Areas (8 missing statements):**
1. Edge cases in monadic operations
2. `None_` vs `none` usage scenarios
3. Complex chaining scenarios

### Stateful Testing (`stateful.py` - 88% coverage)

**Existing Tests:**
- `tests/property/test_stateful.py` - Basic stateful testing
- `tests/property/test_stateful_examples.py` - Examples

**Likely Untested Areas (8 missing statements, 5 partial branches):**
1. Complex state machine scenarios
2. Error recovery in stateful tests
3. Edge cases in action generation
4. Partial branch coverage suggests some conditional paths are not fully tested

## Recommendations

1. **Regenerate Coverage Report**: Run a fresh coverage analysis to get accurate data for the refactored codebase structure.

2. **Focus on Decorators**: The `decorators.py` file has the lowest coverage (84%) and is a critical user-facing API. Prioritize adding tests for:
   - Edge cases in context detection (when class/module lookup fails)
   - Error handling paths (ValueError, exception handling)
   - Complex decorator combinations
   - `run_property_test` function

3. **Add Monad Tests**: Create dedicated test files for `Try`, `Either`, and `Option` monads:
   - `tests/core/test_try.py` - Comprehensive Try monad tests
   - `tests/core/test_either.py` - Comprehensive Either monad tests
   - `tests/core/test_option.py` - Comprehensive Option monad tests
   - Focus on error recovery, filtering, and edge cases

4. **Stateful Testing**: Expand test coverage for stateful property testing scenarios, especially error recovery and complex state machines.

5. **Integration Tests**: Consider adding integration tests that exercise the full decorator API with various generator combinations.

## Action Items

- [x] Regenerate coverage report with current codebase structure
- [x] Create `tests/core/test_try.py` for Try monad (target: 90%+ coverage)
- [x] Create `tests/core/test_either.py` for Either monad (target: 90%+ coverage)
- [x] Create `tests/core/test_option.py` for Option monad (target: 90%+ coverage)
- [x] Add tests for decorator edge cases (context detection failures, error handling)
- [x] Add tests for `run_property_test` function
- [x] Expand stateful property test coverage
- [ ] Review and remove any truly outdated/unused code paths

## Coverage Improvements Made

### New Test Files Created

1. **`tests/core/test_try.py`** (36 tests, ~400 lines)
   - Comprehensive tests for `Success` class (18 tests)
   - Comprehensive tests for `Failure` class (12 tests)
   - Tests for `attempt()` function (4 tests)
   - Tests for Try chaining operations (2 tests)

2. **`tests/core/test_either.py`** (28 tests, ~250 lines)
   - Comprehensive tests for `Left` class (10 tests)
   - Comprehensive tests for `Right` class (12 tests)
   - Tests for Either chaining operations (4 tests)
   - Tests for error handling (2 tests)

3. **`tests/core/test_option.py`** (29 tests, ~250 lines)
   - Comprehensive tests for `Some` class (11 tests)
   - Comprehensive tests for `None_` class (9 tests)
   - Tests for `none()` function (2 tests)
   - Tests for Option chaining operations (4 tests)
   - Tests for edge cases (3 tests)

4. **`tests/decorators/test_decorator_edge_cases.py`** (33 tests, ~200 lines)
   - Tests for context detection edge cases (5 tests)
   - Tests for error handling (4 tests)
   - Tests for decorator metadata preservation (4 tests)
   - Tests for decorator combinations (5 tests)
   - Tests for unittest/pytest integration (3 tests)
   - Tests for `run_property_test` function (1 test)
   - Other edge cases (11 tests)

5. **`tests/property/test_stateful_edge_cases.py`** (21 tests, ~300 lines)
   - Tests for startup/cleanup callbacks (8 tests)
   - Tests for error handling in callbacks (3 tests)
   - Tests for Action with/without model (2 tests)
   - Tests for edge cases (max_actions=0, seeds, etc.) (4 tests)
   - Tests for factory functions (4 tests)

**Total: 147 new tests added**

### Expected Coverage Improvements

These new tests should significantly improve coverage for:
- `try_.py`: From 85% → Expected 95%+
- `either.py`: From 88% → Expected 95%+
- `option.py`: From 89% → Expected 95%+
- `decorators.py`: From 84% → Expected 90%+
- `stateful.py`: From 88% → Expected 92%+

**Overall coverage**: Expected to improve from 92% → 94%+

