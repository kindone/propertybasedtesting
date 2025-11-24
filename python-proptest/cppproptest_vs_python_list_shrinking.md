# Comparison: cppproptest vs Python-proptest List Shrinking

## Test Case: `[1, 2, 3]` with membership-wise only

### cppproptest Expected Output (from test_shrinker2.cpp)

For `shrinkListLike<vector, int>(baseShr, 0, false, true)` (elementwise=false, membershipwise=true):

```
{value: [ 1, 2, 3 ], shrinks: [
  {value: [  ]},                    // empty
  {value: [ 1 ]},                   // single element (first)
  {value: [ 1, 2 ], shrinks: [      // prefix of length 2
    {value: [ 2 ]}                   // can shrink to [2] (remove first)
  ]}, 
  {value: [ 3 ]},                   // single element (last)
  {value: [ 1, 3 ]},                // first and last
  {value: [ 2, 3 ]}                 // last two
]}
```

### Python-proptest Current Output

For `shrink_list(shrinkable_elems, min_size=0, membership_wise=True, element_wise=False)`:

```
[4, 3, 11]  (example from our test)
├── []                                    (empty)
├── [4, 3]                                (remove last)
├── [3, 11]                               (remove first)
└── (element-wise shrinks if enabled)
```

## Key Differences

### 1. Membership-wise Strategy

**cppproptest (`shrinkFrontAndThenMid`):**
- Uses a "front and rear" strategy
- Shrinks the front size using binary search (0 to maxFrontSize)
- Keeps rear part intact
- Recursively shrinks by fixing more elements to the rear
- Generates: `[]`, `[1]`, `[1,2]`, `[3]`, `[1,3]`, `[2,3]`

**Python-proptest (`shrink_membership_wise`):**
- Uses a simpler "remove from end/beginning" strategy
- Generates: `[]`, `[4, 3]` (remove last), `[3, 11]` (remove first)
- Does not generate all possible subsets

### 2. Complete Subset Generation

**cppproptest:**
- For a list of size N, generates 2^N unique subsets (all possible combinations)
- Test confirms: `EXPECT_EQ(checkShrinkable(shr), pow(2, N))`

**Python-proptest:**
- Currently generates only a subset of all possible combinations
- Missing many intermediate combinations

## cppproptest Implementation Details

From `listlike.cpp`, the `shrinkFrontAndThenMid` function:

1. **Front size shrinking**: Uses `shrinkIntegral` to shrink front size from `minFrontSize` to `maxFrontSize`
2. **Rear preservation**: Keeps `rearSize` elements at the end intact
3. **Recursive shrinking**: Recursively shrinks by incrementing `rearSize`

```cpp
VectorShrinker::shrinkable_t VectorShrinker::shrinkFrontAndThenMid(
    const Shrinkable<vector<ShrinkableBase>>& shr, 
    size_t minSize, 
    size_t rearSize) 
{
    // Remove front as much as possible while keeping rear intact
    const auto& shrinkableCont = shr.getRef();
    size_t minFrontSize = minSize >= rearSize ? minSize - rearSize : 0;
    size_t maxFrontSize = shrinkableCont.size() - rearSize;
    
    // Front size within [minFrontSize, maxFrontSize]
    ShrinkableBase rangeShrinkable = shrinkIntegral<size_t>(maxFrontSize - minFrontSize)
        .template map<size_t>([minFrontSize](const size_t& s) { return s + minFrontSize; });
    
    return rangeShrinkable.flatMap([shr, maxFrontSize](const Any& frontSizeAny) -> ShrinkableBase {
        const size_t& frontSize = frontSizeAny.getRef<size_t>();
        const auto& shrinkableCont = shr.getRef();
        // Concat front and rear parts
        auto contPtr = util::make_unique<vector<ShrinkableBase>>(
            shrinkableCont.begin(), 
            shrinkableCont.begin() + frontSize);
        contPtr->insert(contPtr->end(), 
                       shrinkableCont.begin() + maxFrontSize, 
                       shrinkableCont.end());
        return ShrinkableBase(util::make_any<vector<ShrinkableBase>>(util::move(contPtr)));
    }).concat([minSize, rearSize](const shrinkable_t& parent) -> stream_t {
        // Recursively shrink front further by fixing one more element to rear
        return shrinkFrontAndThenMid(parent, minSize, rearSize + 1).getShrinks();
    });
}
```

## What Needs to Change

Our Python implementation should:

1. **Implement `shrinkFrontAndThenMid` strategy** instead of simple remove-from-end/beginning
2. **Generate all 2^N subsets** for a list of size N
3. **Use binary search for front size** (similar to how we shrink integers)
4. **Recursively shrink by incrementing rear size**

The current `shrink_membership_wise` function is too simplistic and doesn't match cppproptest's behavior.

