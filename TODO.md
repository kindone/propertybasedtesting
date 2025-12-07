# TODO: Property-Based Testing Framework Updates

This file tracks cross-language implementation tasks and feature parity requirements.

## Floating Point Generator Enhancement

### Status: ✅ Python Completed, ✅ C++ Completed, ⏳ Dart/JS Pending

### Description
Implement consistent floating point generator behavior across all language implementations:
- **Default behavior**: Generate only finite values (practical for most tests)
- **Optional probabilities**: Allow explicit control over NaN, +inf, and -inf generation
- **API**: `Gen.float(nanProb, posInfProb, negInfProb)` with all probabilities defaulting to 0.0

### Implementation Status

#### ✅ Python (`python-proptest`)
- **Status**: Completed
- **Implementation**:
  - `Gen.float(nan_prob=0.0, posinf_prob=0.0, neginf_prob=0.0)`
  - Finite-only generation using bit interpretation with rejection loop
  - Uses `one_of` internally when probabilities are specified
  - Covers full finite float space including denormals
- **Files Modified**:
  - `python_proptest/core/generator/floating.py`
  - `python_proptest/core/generator/gen.py`
  - `docs/generators.md`
- **Date Completed**: 2025-12-06

#### ✅ C++ (`cppproptest2`)
- **Status**: Completed
- **Implementation**:
  - `Arbi<float>(double nanProb = 0.0, double posInfProb = 0.0, double negInfProb = 0.0)`
  - `Arbi<double>(double nanProb = 0.0, double posInfProb = 0.0, double negInfProb = 0.0)`
  - `generateFiniteFloat()` helper function with bit interpretation and rejection loop using `isfinite()`
  - Uses `gen::oneOf` with `gen::weightedGen` for probability-based selection when probabilities > 0
  - Covers full finite float space including denormals
  - Comprehensive tests including domain preservation during shrinking
- **Files Modified**:
  - `proptest/generator/floating.hpp`
  - `proptest/generator/floating.cpp`
  - `proptest/test/test_floating.cpp`
  - `docs/Generators.md`
- **Date Completed**: 2025-01-20

#### ⏳ Dart (`dartproptest`)
- **Status**: Pending
- **Required Changes**:
  - Add probability parameters to float generator
  - Implement finite-only generation with rejection loop
  - Use `oneOf` combinator for probability-based selection
  - Update documentation

#### ⏳ TypeScript/JavaScript (`jsproptest`)
- **Status**: Pending
- **Required Changes**:
  - Add probability parameters to float generator
  - Implement finite-only generation with rejection loop
  - Use `oneOf` combinator for probability-based selection
  - Update documentation

### Design Principles

1. **Consistency**: All implementations should have the same API and behavior
2. **Practical Defaults**: Generate finite values by default (most tests don't need inf/NaN)
3. **Explicit Control**: Allow users to specify probabilities when they need special values
4. **Efficiency**: Use rejection loop for finite generation (very rare retries, ~99.95% success rate)
5. **Coverage**: Finite generation should cover full finite float space including denormals

### API Specification

```python
# Python
Gen.float(nan_prob=0.0, posinf_prob=0.0, neginf_prob=0.0)
```

```cpp
// C++
Arbi<float>(double nanProb = 0.0, double posInfProb = 0.0, double negInfProb = 0.0)
```

```dart
// Dart (to be implemented)
Gen.float({double nanProb = 0.0, double posInfProb = 0.0, double negInfProb = 0.0})
```

```typescript
// TypeScript/JavaScript (to be implemented)
Gen.float({nanProb = 0.0, posInfProb = 0.0, negInfProb = 0.0})
```

### Validation Requirements

For each implementation:
- [x] Default generates only finite values (✅ Python, ✅ C++)
- [x] NaN probability works correctly (✅ Python, ✅ C++)
- [x] +inf probability works correctly (✅ Python, ✅ C++)
- [x] -inf probability works correctly (✅ Python, ✅ C++)
- [x] All probabilities can be combined (✅ Python, ✅ C++)
- [x] Validation: each prob between 0.0 and 1.0 (✅ Python, ✅ C++)
- [x] Validation: sum of probs <= 1.0 (✅ Python, ✅ C++)
- [x] Finite generation covers full float space (✅ Python, ✅ C++)
- [x] Documentation updated with examples (✅ Python, ✅ C++)

### Notes

- Python implementation uses bit interpretation (64-bit double) with rejection loop
- C++ will use same approach (32-bit for float, 64-bit for double)
- Dart and JS should follow same pattern for their respective float types
- All implementations should use `oneOf`/`one_of` combinator internally when probabilities are specified

---

**Last Updated**: 2025-01-20
**Next Steps**: Implement Dart and JS versions

