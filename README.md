# Property-Based Testing Workspace

This workspace contains multiple property-based testing (PBT) framework implementations across different programming languages, along with analysis, comparisons, and discussions.

## Workspace Structure

### Core Implementations

The workspace contains property-based testing libraries for multiple languages:

1. **`cppproptest2/`** - C++ property-based testing framework (reference implementation)
   - Primary reference implementation
   - Most validated and comprehensive
   - Used as the benchmark for comparing other implementations

2. **`pyproptest/`** - Python property-based testing framework
   - Published to PyPI as `python-proptest`
   - Actively developed and maintained
   - Currently being refactored to match cppproptest architecture

3. **`jsproptest/`** - TypeScript/JavaScript property-based testing framework
   - Published to npm
   - Ported to Dart as dartproptest

4. **`dartproptest/`** - Dart property-based testing framework
   - Port of jsproptest to Dart
   - Published to pub.dev

### Analysis and Discussions

This directory (`propertybasedtesting/`) contains analysis, comparisons, and discussions:

```
propertybasedtesting/
├── README.md                    # This file - workspace overview
├── validation/                  # Validation results and analysis
│   ├── README.md               # Validation overview
│   ├── VALIDATION_RESULTS.md   # Python flat_map validation
│   ├── CPP_VALIDATION_RESULTS.md  # C++ flat_map validation
│   └── CRITICAL_ANALYSIS_FLATMAP.md  # Critical analysis of flat_map
└── python-proptest/            # Python-specific analysis
    ├── README.md               # Python-proptest analysis overview
    ├── collection_shrinking_status.md    # Set/String/Dict shrinking status
    ├── tuple_shrinking_status.md         # Tuple shrinking status
    ├── cppproptest_vs_python_list_shrinking.md  # List shrinking comparison
    ├── *_shrink_tree_structure.md       # Shrink tree documentation
    └── show_*.py               # Analysis scripts
```

## Current Status and Priorities

### Python-proptest Refactoring Status

The Python implementation is currently being refactored to align with cppproptest's architecture:

#### ✅ Completed
- **Generator/Shrinker Separation**: Separated generator logic from shrinker logic into dedicated modules
- **Modular Shrinker Organization**: Created dedicated shrinker modules:
  - `shrinker/integral.py` - Integer shrinking
  - `shrinker/bool.py` - Boolean shrinking
  - `shrinker/floating.py` - Float shrinking
  - `shrinker/string.py` - String shrinking
  - `shrinker/list.py` - List/Set/Dict shrinking
  - `shrinker/pair.py` - Pair (2-tuple) shrinking
- **Combinator Organization**: Moved combinators to `combinator/` directory
- **RNG State Management**: Fixed RNG state cloning for reproducible shrinking
- **Transform Combinators**: Fixed `map` and `filter` to apply recursively to shrink trees
- **Test Organization**: Reorganized tests to match new directory structure

#### ⚠️ Known Issues and Priorities

**Priority 1: Dictionary Shrinking** 🔴 **HIGHEST PRIORITY**
- **Issue**: Missing pair shrinking - only shrinks values, keys are fixed
- **Impact**: Cannot shrink dictionary keys, even when they have shrinkable values
- **Status**: Architectural mismatch with cppproptest
- **Fix Required**: Implement pair shrinking infrastructure

**Priority 2: Membership-wise Shrinking** 🟡 **HIGH PRIORITY**
- **Issue**: `shrink_membership_wise` doesn't match cppproptest's `shrinkFrontAndThenMid`
- **Impact**: Doesn't generate all 2^N subsets for collections of size N
- **Affects**: Lists, Sets, Strings, Dictionaries
- **Fix Required**: Implement `shrinkFrontAndThenMid` strategy

**Priority 3: Tuple Shrinking** 🟡 **MEDIUM PRIORITY**
- **Issue**: Only handles 2-element tuples (pairs), not general N-tuples
- **Status**: Basic shrinking exists but not recursive
- **Fix Required**: Implement general `shrink_tuple` function

**Priority 4: String Shrinking** 🟢 **LOWER PRIORITY**
- **Issue**: Different strategy than cppproptest (uses element-wise + membership-wise)
- **Status**: Functional but may not match cppproptest exactly
- **Note**: Depends on membership-wise fix

## Key Architectural Patterns

### Generator/Shrinker Separation

Following cppproptest's architecture:
- **Generators** (`generator/`): Responsible for generating random values
- **Shrinkers** (`shrinker/`): Responsible for shrinking failing values to minimal counterexamples
- Clear separation of concerns

### Shrinkable Pattern

All generators return `Shrinkable[T]` objects that contain:
- The generated value
- A lazy stream of simpler values (shrinks)
- Recursive shrinking capability

### Shrinking Strategies

1. **Binary Search Shrinking**: For integers, efficiently finds simpler values
2. **Membership-wise Shrinking**: For collections, removes elements
3. **Element-wise Shrinking**: For collections, shrinks individual elements
4. **Pair Shrinking**: For tuples/dictionaries, shrinks pairs together

## Analysis Documents

### Validation Results

See `validation/` directory for validation results:

- **`VALIDATION_RESULTS.md`**: Python `flat_map` implementation validation (8 tests, all passed ✅)
- **`CPP_VALIDATION_RESULTS.md`**: C++ `flat_map` implementation validation (5 tests, all passed ✅)
- **`CRITICAL_ANALYSIS_FLATMAP.md`**: Critical analysis of potential issues in both implementations
- **`CPPPROPTEST_VS_PYTHON_FLATMAP.md`**: In-depth architectural comparison of cppproptest vs Python flat_map
- **`README.md`**: Overview of validation results and findings

### Python-proptest Analysis

See `python-proptest/` directory for detailed analysis:

- **`collection_shrinking_status.md`**: Comprehensive status of set, string, and dictionary shrinking with priority analysis
- **`tuple_shrinking_status.md`**: Status of tuple shrinking implementation
- **`cppproptest_vs_python_list_shrinking.md`**: Detailed comparison of list shrinking strategies
- **`*_shrink_tree_structure.md`**: Documentation of shrink tree structures and behaviors

### Analysis Scripts

Python scripts for visualizing and analyzing shrink trees:
- `show_list_shrink_tree*.py` - Visualize shrink trees for lists
- `show_gen_list_shrink_tree*.py` - Generate and display shrink trees from generators

## Reference Implementation

**cppproptest2** serves as the reference implementation because:
- Most validated and comprehensive
- Well-documented shrinking strategies
- Used as benchmark for correctness
- Other implementations aim to match its behavior

## Development Workflow

1. **Identify Gap**: Compare Python implementation with cppproptest
2. **Document**: Create analysis documents in `python-proptest/`
3. **Implement**: Fix issues following cppproptest patterns
4. **Verify**: Test against cppproptest's expected behavior
5. **Document**: Update status documents

## Notes for AI Assistants

When working in this workspace:

1. **Reference Implementation**: Always check `cppproptest2/` for expected behavior
2. **Analysis First**: Check `propertybasedtesting/python-proptest/` for existing analysis
3. **Architecture**: Follow the generator/shrinker separation pattern
4. **Shrinking**: Ensure shrinking matches cppproptest's strategies
5. **Testing**: Verify against cppproptest test cases when possible

## Related Workspaces

- `proptestports.code-workspace` - VS Code workspace configuration
- `proptestdoc/` - Documentation projects

---

**Last Updated**: Analysis documents reflect status as of recent refactoring work to align Python implementation with cppproptest architecture.
