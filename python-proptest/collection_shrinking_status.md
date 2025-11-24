# Collection Shrinking Status: Set, String, Dictionary

## Summary

All three collection types (set, string, dictionary) have shrinking implementations, but they may not match cppproptest's behavior exactly.

---

## 1. String Shrinking

### Python Implementation (`shrinker/string.py`):

**Functions:**
- `shrink_string()` - for ASCII strings
- `shrink_unicode_string()` - for Unicode strings

**Strategy:**
- Uses `shrinkable_array()` with both membership-wise and element-wise shrinking
- Converts string to list of character codes (Shrinkable[int])
- Shrinks by:
  1. **Membership-wise**: Removing characters (length shrinking)
  2. **Element-wise**: Shrinking character codes (binary search towards 0)

**Current Implementation:**
```python
def shrink_string(value: str, min_length: int = 0, char_shrinkables: List[Shrinkable[int]] = None) -> Shrinkable[str]:
    if char_shrinkables is None:
        char_shrinkables = [binary_search_shrinkable(ord(c)) for c in value]
    
    array_shrinkable = shrinkable_array(
        char_shrinkables,
        min_size=min_length,
        membership_wise=True,
        element_wise=True,
    )
    
    return array_shrinkable.map(lambda points: "".join(chr(p) for p in points))
```

### cppproptest Expected Behavior:

From test: `shrinkString("abc", 0)` should produce:
```
{value: "abc" (61 62 63), shrinks: [
  {value: "" ()},                    // empty
  {value: "a" (61)},                 // single char
  {value: "ab" (61 62), shrinks: [   // prefix
    {value: "b" (62)}                // can shrink to "b"
  ]}, 
  {value: "bc" (62 63)},             // suffix
  {value: "c" (63)}                  // single char
]}
```

**Key Pattern**: Uses `shrinkFrontAndThenMid` strategy (same as lists) to generate all subsets.

### Status:
✅ **Has implementation** - Uses `shrinkable_array` which should work
⚠️ **May not match exactly** - Depends on whether `shrink_membership_wise` matches `shrinkFrontAndThenMid`

---

## 2. Set Shrinking

### Python Implementation (`shrinker/list.py`):

**Function:**
- `shrink_set()`

**Strategy:**
- Uses `shrinkable_array()` with **membership-wise only** (no element-wise)
- Converts set to list, shrinks, then converts back to set
- Element-wise shrinking is disabled to avoid duplicates

**Current Implementation:**
```python
def shrink_set(
    shrinkable_elems: List[Shrinkable[T]],
    min_size: int = 0,
) -> Shrinkable[Set[T]]:
    array_shrinkable = shrinkable_array(
        shrinkable_elems, min_size, membership_wise=True, element_wise=False
    )
    return array_shrinkable.map(lambda arr: set(arr))
```

### cppproptest Expected Behavior:

From test: `shrinkSet({1, 2, 3}, 0)` should produce:
```
{value: { 1, 2, 3 }, shrinks: [
  {value: { }},                    // empty
  {value: { 1 }},                  // single element
  {value: { 1, 2 }, shrinks: [     // subset
    {value: { 2 }}                 // can shrink to {2}
  ]}, 
  {value: { 3 }},                  // single element
  {value: { 1, 3 }},               // subset
  {value: { 2, 3 }}                // subset
]}
```

**Key Pattern**: 
- Uses `shrinkContainer` with `elementwise=false, membershipwise=true`
- Same `shrinkFrontAndThenMid` strategy as lists
- Generates all 2^N subsets

### Status:
✅ **Has implementation** - Uses `shrinkable_array` with membership-wise only
⚠️ **May not match exactly** - Depends on whether `shrink_membership_wise` matches `shrinkFrontAndThenMid`

---

## 3. Dictionary Shrinking

### Python Implementation (`shrinker/list.py`):

**Function:**
- `shrink_dict()`

**Strategy:**
- Shrinks values only (keys are fixed to maintain dict structure)
- Uses `shrinkable_array()` on values with both membership-wise and element-wise
- Reconstructs dict by pairing with original keys

**Current Implementation:**
```python
def shrink_dict(
    key_shrinkables: List[Shrinkable[T]],
    value_shrinkables: List[Shrinkable[U]],
    min_size: int = 0,
) -> Shrinkable[Dict[T, U]]:
    value_list_shrinkable = shrinkable_array(
        value_shrinkables, min_size, membership_wise=True, element_wise=True
    )
    
    def reconstruct_dict(values: List[U]) -> Dict[T, U]:
        result = {}
        for i, value in enumerate(values):
            if i < len(key_shrinkables):
                result[key_shrinkables[i].value] = value
        return result
    
    return value_list_shrinkable.map(reconstruct_dict)
```

### cppproptest Expected Behavior:

From test: `shrinkMap({(100,1), (300,3), (500,5)}, 0)` should produce:
```
{value: { (100, 1), (300, 3), (500, 5) }, shrinks: [
  {value: { }},                    // empty
  {value: { (100, 1) }, shrinks: [ // single pair
    {value: { (100, 0) }}          // shrink value: 1 -> 0
  ]}, 
  {value: { (100, 1), (300, 3) }, shrinks: [
    {value: { (300, 3) }, shrinks: [
      {value: { (300, 0) }},       // shrink value: 3 -> 0
      {value: { (300, 1) }},        // shrink value: 3 -> 1
      {value: { (300, 2) }}         // shrink value: 3 -> 2
    ]}
  ]},
  {value: { (500, 5) }, shrinks: [
    {value: { (500, 0) }},          // shrink value: 5 -> 0
    {value: { (500, 2) }, shrinks: [
      {value: { (500, 1) }}         // shrink value: 2 -> 1
    ]},
    {value: { (500, 3) }},
    {value: { (500, 4) }}
  ]},
  // ... more combinations
]}
```

**Key Pattern**:
- Uses `shrinkMap` which shrinks pairs
- Shrinks by membership (removing pairs) and by values (shrinking values)
- Keys are NOT shrunk (to maintain dict structure)
- Uses `shrinkFrontAndThenMid` for membership-wise

### Status:
✅ **Has implementation** - Shrinks values with membership-wise and element-wise
⚠️ **May not match exactly** - 
  - Uses simplified approach (shrinks values as list, then reconstructs)
  - cppproptest uses `shrinkMap` which may use pair shrinking
  - Depends on whether `shrink_membership_wise` matches `shrinkFrontAndThenMid`

---

## Common Issue: Membership-wise Shrinking

All three collection types rely on `shrink_membership_wise()` which currently uses a simple "remove from end/beginning" strategy. This is **different** from cppproptest's `shrinkFrontAndThenMid` which:

1. Generates **all 2^N subsets** (for size N)
2. Uses binary search for front size
3. Recursively shrinks by incrementing rear size

### Impact:

- **String**: May not generate all substring combinations
- **Set**: May not generate all subset combinations  
- **Dictionary**: May not generate all key-value pair subset combinations

All three will work, but may not be as comprehensive as cppproptest's shrinking.

---

## Recommendations

1. **String**: ✅ Implementation looks correct, but verify it matches cppproptest's output
2. **Set**: ✅ Implementation looks correct, but verify it matches cppproptest's output
3. **Dictionary**: ⚠️ May need to use pair shrinking instead of value-only shrinking
4. **All**: Need to fix `shrink_membership_wise` to match `shrinkFrontAndThenMid` for full compatibility

---

## Priority Analysis: Which Needs Fixing Most?

### 🔴 **Dictionary (Map) Shrinking - HIGHEST PRIORITY**

**Why it's most deteriorating:**

1. **Fundamental Architectural Mismatch**:
   - cppproptest: Uses `shrinkMap` which creates **pairs** from (key, value) shrinkables and shrinks them together
   - Python: Only shrinks **values**, keys are **completely fixed**
   - Missing core feature: **pair shrinking**

2. **Missing Functionality**:
   - Cannot shrink keys at all, even when they have shrinkable values
   - cppproptest can shrink both keys and values together as pairs

3. **Code Evidence**:
   - Python code has comment: `"This is a simplified version - a full implementation would use pair shrinking"`

4. **Impact Example**:
   - cppproptest: `{(100,1), (300,3)}` can shrink to `{(0,0), (300,3)}` (key 100→0, value 1→0)
   - Python: Can only shrink to `{(100,0), (300,3)}` (value only, key fixed)

**Fix Required**: Implement pair shrinking infrastructure, then use it for dictionary shrinking.

---

### 🟡 **String Shrinking - MEDIUM PRIORITY**

**Why it's less critical:**

- Uses different strategy (element-wise + membership-wise) vs cppproptest's simpler length-based approach
- Still functional, just different behavior
- Depends on `shrink_membership_wise` fix for full compatibility

---

### 🟢 **Set Shrinking - LOWEST PRIORITY**

**Why it's least critical:**

- Same conceptual approach as cppproptest (membership-wise only)
- Works correctly, just not as comprehensive (won't generate all 2^N subsets)
- Depends on `shrink_membership_wise` fix for full compatibility

---

### **Root Cause: Membership-wise Shrinking**

All three collections (and lists) share a common underlying issue: `shrink_membership_wise` doesn't match cppproptest's `shrinkFrontAndThenMid` strategy. However, **Dictionary has an ADDITIONAL fundamental issue** - it doesn't use pair shrinking at all.

---

## Recommended Fix Order:

1. **Dictionary** - Implement pair shrinking (fundamental architectural fix)
   - This will also benefit tuple shrinking
   - Creates reusable pair shrinking infrastructure

2. **Membership-wise shrinking** - Fix `shrink_membership_wise` to match `shrinkFrontAndThenMid`
   - This will improve: Set, String, List, and Dictionary shrinking
   - Enables generation of all 2^N subsets

3. **String** - Verify/align strategy if needed (after membership-wise fix)

4. **Set** - Should work correctly after membership-wise fix

