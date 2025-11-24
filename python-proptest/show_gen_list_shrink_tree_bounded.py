#!/usr/bin/env python3
"""
Script to extract and display the shrink tree from Gen.list(Gen.int(0, 100), 0, 4)
where the generated list has size 4, using bounded integers for readability.
"""

import sys
import os
sys.path.insert(0, os.path.dirname(__file__))

import random
from python_proptest import Gen


def format_list(lst):
    """Format a list for display."""
    if not lst:
        return "[]"
    return "[" + ", ".join(f"{x:3d}" if isinstance(x, int) else str(x) for x in lst) + "]"


def collect_all_shrinks(shrinkable, max_depth=10, visited=None):
    """
    Collect all unique values in the shrink tree.
    Returns a set of all values (as tuples for lists to make them hashable).
    """
    if visited is None:
        visited = set()
    
    value = shrinkable.value
    if isinstance(value, list):
        value_key = tuple(value)
    else:
        value_key = value
    
    if value_key in visited or max_depth <= 0:
        return visited
    
    visited.add(value_key)
    
    # Get all direct shrinks
    for child in shrinkable.shrinks().to_list():
        collect_all_shrinks(child, max_depth - 1, visited)
    
    return visited


def build_tree_structure(shrinkable, max_depth=10, visited_nodes=None):
    """
    Build a tree structure showing parent-child relationships.
    Returns a dictionary mapping values to their children.
    """
    if visited_nodes is None:
        visited_nodes = {}
    
    value = shrinkable.value
    value_key = tuple(value) if isinstance(value, list) else value
    
    if value_key in visited_nodes:
        return visited_nodes[value_key]
    
    if max_depth <= 0:
        node = {"value": value, "children": []}
        visited_nodes[value_key] = node
        return node
    
    children = []
    for child_shrinkable in shrinkable.shrinks().to_list():
        child_value = child_shrinkable.value
        child_key = tuple(child_value) if isinstance(child_value, list) else child_value
        
        if child_key not in visited_nodes:
            child_node = build_tree_structure(child_shrinkable, max_depth - 1, visited_nodes)
            children.append(child_node)
        else:
            children.append(visited_nodes[child_key])
    
    node = {"value": value, "children": children}
    visited_nodes[value_key] = node
    return node


def print_tree(node, prefix="", is_last=True, max_depth=10, current_depth=0, visited_print=None, max_children=20):
    """Print the tree in a tree-like format."""
    if current_depth >= max_depth:
        return
    
    if visited_print is None:
        visited_print = set()
    
    value = node["value"]
    if isinstance(value, list):
        display = format_list(value)
        value_key = tuple(value)
    else:
        display = str(value)
        value_key = value
    
    # Avoid printing duplicates
    if value_key in visited_print:
        return
    
    visited_print.add(value_key)
    
    # Print current node
    marker = "└── " if is_last else "├── "
    print(prefix + marker + display)
    
    # Print children (limit to max_children for readability)
    children = node["children"]
    if children:
        # Show first few and last few if there are many
        if len(children) > max_children:
            to_show = children[:max_children//2] + children[-max_children//2:]
            for i, child in enumerate(to_show):
                is_last_child = (i == len(to_show) - 1)
                extension = "    " if is_last else "│   "
                if i == max_children//2 - 1:
                    print(prefix + extension + "    ... ({} more children) ...".format(len(children) - max_children))
                print_tree(child, prefix + extension, is_last_child, max_depth, current_depth + 1, visited_print, max_children)
        else:
            for i, child in enumerate(children):
                is_last_child = (i == len(children) - 1)
                extension = "    " if is_last else "│   "
                print_tree(child, prefix + extension, is_last_child, max_depth, current_depth + 1, visited_print, max_children)


def print_flat_sorted(shrinkable, max_depth=10, max_items=50):
    """Print all values in sorted order (by length, then by values)."""
    all_values = collect_all_shrinks(shrinkable, max_depth)
    
    # Convert to lists and sort
    list_values = []
    for val in all_values:
        if isinstance(val, tuple):
            list_values.append(list(val))
        else:
            list_values.append([val])
    
    # Sort by length first, then by values
    list_values.sort(key=lambda x: (len(x), x))
    
    print(f"\nAll unique values in shrink tree (showing first {max_items}, sorted by length, then by values):")
    print("-" * 80)
    for i, lst in enumerate(list_values[:max_items]):
        print(f"  {format_list(lst)}")
    if len(list_values) > max_items:
        print(f"  ... ({len(list_values) - max_items} more values)")


def main():
    print("=" * 80)
    print("SHRINK TREE FROM Gen.list(Gen.int(0, 100), 0, 4)")
    print("=" * 80)
    print("\nUsing bounded integers (0-100) for readability")
    
    # Create the generator with bounded integers
    gen = Gen.list(Gen.int(0, 100), min_length=0, max_length=4)
    
    # Generate multiple times until we get a list of size 4
    rng = random.Random(42)
    shrinkable = None
    attempts = 0
    
    print("\nGenerating lists until we get one of size 4...")
    while attempts < 100:
        shrinkable = gen.generate(rng)
        if len(shrinkable.value) == 4:
            break
        attempts += 1
    
    if shrinkable is None or len(shrinkable.value) != 4:
        print("Failed to generate a list of size 4 after 100 attempts")
        return
    
    root_value = shrinkable.value
    print(f"\nGenerated list: {format_list(root_value)}")
    print(f"List size: {len(root_value)}")
    
    print("\n" + "=" * 80)
    print("SHRINK TREE STRUCTURE (showing first 3 levels, max 20 children per node)")
    print("=" * 80)
    
    # Build and print the tree
    tree = build_tree_structure(shrinkable, max_depth=5)
    
    if isinstance(root_value, list):
        root_key = tuple(root_value)
    else:
        root_key = root_value
    
    print("\nTree structure:")
    print("-" * 80)
    print_tree(tree, max_depth=3, max_children=20)
    
    # Print flat sorted list
    print("\n" + "=" * 80)
    print_flat_sorted(shrinkable, max_depth=5, max_items=50)
    
    # Statistics
    all_values = collect_all_shrinks(shrinkable, max_depth=5)
    print(f"\nStatistics:")
    print(f"  Total unique values in shrink tree (depth 5): {len(all_values)}")
    
    # Count by length
    length_counts = {}
    for val in all_values:
        if isinstance(val, tuple):
            length = len(val)
        else:
            length = 1
        length_counts[length] = length_counts.get(length, 0) + 1
    
    print(f"\n  Values by length:")
    for length in sorted(length_counts.keys()):
        print(f"    Length {length}: {length_counts[length]} values")
    
    print("\n" + "=" * 80)
    print("\nNote: The tree is very large due to element-wise shrinking.")
    print("Each integer element has its own shrink tree (binary search towards 0).")
    print("The full tree combines membership-wise (removing elements) and")
    print("element-wise (shrinking integers) strategies.")


if __name__ == "__main__":
    main()

