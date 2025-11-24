# Python-proptest Analysis and Discussions

This directory contains analysis scripts, documentation, and discussions specifically related to the Python property-based testing implementation (`pyproptest`).

## Contents

### Analysis Scripts
- `show_list_shrink_tree.py` - Basic script to visualize shrink trees for hardcoded lists
- `show_list_shrink_tree_detailed.py` - Detailed visualization with better formatting
- `show_gen_list_shrink_tree.py` - Generate and display shrink trees from `Gen.list()` (full integer range)
- `show_gen_list_shrink_tree_bounded.py` - Generate and display shrink trees with bounded integers (0-100)
- `show_gen_list_shrink_tree_size3.py` - Generate shrink trees for lists of size 3

### Documentation
- `list_shrink_tree_structure.md` - Documentation of shrink tree structure for hardcoded lists
- `gen_list_shrink_tree_structure.md` - Documentation of shrink tree structure for generated lists (bounded)
- `gen_list_shrink_tree_size3.md` - Complete shrink tree documentation for lists of size 3
- `cppproptest_vs_python_list_shrinking.md` - Comparison between cppproptest and Python list shrinking implementations
- `collection_shrinking_status.md` - Status of set, string, and dictionary shrinking implementations (with priority analysis)
- `tuple_shrinking_status.md` - Status of tuple shrinking implementation

### Purpose

These files document:
- Shrink tree structures and behaviors
- Comparisons with cppproptest (reference implementation)
- Implementation status and gaps
- Priority analysis for fixing shrinking implementations
- Development/analysis discussions

**Note:** These files are for development/analysis purposes only and are not included in the published package.
