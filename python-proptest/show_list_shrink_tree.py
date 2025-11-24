#!/usr/bin/env python3
"""
Script to visualize the shrink tree for an integer list of size 4.
"""

import sys
import os
sys.path.insert(0, os.path.dirname(__file__))

from python_proptest.core.shrinker.list import shrink_list
from python_proptest.core.shrinker.integral import shrink_integral
from python_proptest.core.shrinker import Shrinkable


def format_list(lst):
    """Format a list for display."""
    return "[" + ", ".join(str(x) for x in lst) + "]"


def collect_shrink_tree(shrinkable, max_depth=5, visited=None):
    """
    Collect all values in the shrink tree up to max_depth.
    Returns a dictionary mapping values to their children.
    """
    if visited is None:
        visited = set()
    
    value = shrinkable.value
    value_id = id(value) if isinstance(value, list) else value
    
    # Use a tuple representation for lists to make them hashable
    if isinstance(value, list):
        value_key = tuple(value)
    else:
        value_key = value
    
    if value_key in visited:
        return {}
    
    visited.add(value_key)
    
    result = {value_key: []}
    
    if max_depth <= 0:
        return result
    
    # Get all direct shrinks
    for child in shrinkable.shrinks().to_list():
        child_value = child.value
        if isinstance(child_value, list):
            child_key = tuple(child_value)
        else:
            child_key = child_value
        
        if child_key not in visited:
            result[value_key].append(child_key)
            # Recursively collect children
            child_tree = collect_shrink_tree(child, max_depth - 1, visited.copy())
            result.update(child_tree)
    
    return result


def print_tree_structure(tree, root, indent=0, max_depth=10, visited=None):
    """Print the tree structure in a readable format."""
    if visited is None:
        visited = set()
    if max_depth <= 0:
        return
    
    if root in visited:
        return
    visited.add(root)
    
    prefix = "  " * indent
    if isinstance(root, tuple):
        print(f"{prefix}{format_list(list(root))}")
    else:
        print(f"{prefix}{root}")
    
    if root in tree:
        for child in tree[root]:
            print_tree_structure(tree, child, indent + 1, max_depth - 1, visited)


def main():
    # Create a list of 4 integers: [10, 20, 30, 40]
    # Each element will have its own shrink tree
    print("Creating shrink tree for list [10, 20, 30, 40]")
    print("=" * 60)
    
    # Create shrinkables for each element
    # For integers, we'll use a simple range (0 to 100)
    element_shrinkables = [
        shrink_integral(10, 0, 100),
        shrink_integral(20, 0, 100),
        shrink_integral(30, 0, 100),
        shrink_integral(40, 0, 100),
    ]
    
    # Create the list shrinkable with membership-wise shrinking enabled
    list_shrinkable = shrink_list(
        element_shrinkables,
        min_size=0,
        membership_wise=True,
        element_wise=False  # Disable element-wise for cleaner output
    )
    
    print("\nRoot value:", format_list(list_shrinkable.value))
    print("\nShrink Tree Structure:")
    print("-" * 60)
    
    # Collect the shrink tree
    tree = collect_shrink_tree(list_shrinkable, max_depth=10)
    
    # Print the tree
    root_key = tuple(list_shrinkable.value)
    print_tree_structure(tree, root_key, max_depth=10)
    
    print("\n" + "=" * 60)
    print("\nNow with element-wise shrinking enabled:")
    print("-" * 60)
    
    # Create the list shrinkable with both membership-wise and element-wise shrinking
    list_shrinkable_full = shrink_list(
        element_shrinkables,
        min_size=0,
        membership_wise=True,
        element_wise=True
    )
    
    print("\nRoot value:", format_list(list_shrinkable_full.value))
    print("\nShrink Tree Structure (with element-wise shrinking):")
    print("-" * 60)
    
    # Collect the shrink tree
    tree_full = collect_shrink_tree(list_shrinkable_full, max_depth=5)
    
    # Print the tree (limited depth due to complexity)
    root_key = tuple(list_shrinkable_full.value)
    print_tree_structure(tree_full, root_key, max_depth=5)
    
    print("\n" + "=" * 60)
    print("\nSummary:")
    print(f"  Total unique values in membership-only tree: {len(tree)}")
    print(f"  Total unique values in full tree (depth 5): {len(tree_full)}")


if __name__ == "__main__":
    main()

