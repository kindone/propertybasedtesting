#!/usr/bin/env python3
"""
Script to visualize the detailed shrink tree for an integer list of size 4.
Shows the complete tree structure with proper hierarchy.
"""

import sys
import os
sys.path.insert(0, os.path.dirname(__file__))

from python_proptest.core.shrinker.list import shrink_list
from python_proptest.core.shrinker.integral import shrink_integral
from python_proptest.core.shrinker import Shrinkable


def format_list(lst):
    """Format a list for display."""
    return "[" + ", ".join(f"{x:3d}" for x in lst) + "]"


def build_tree_structure(shrinkable, max_depth=10, visited=None):
    """
    Build a tree structure showing parent-child relationships.
    Returns a list of (value, children) tuples.
    """
    if visited is None:
        visited = {}
    
    value = shrinkable.value
    value_key = tuple(value) if isinstance(value, list) else value
    
    if value_key in visited:
        return visited[value_key]
    
    if max_depth <= 0:
        node = {"value": value, "children": []}
        visited[value_key] = node
        return node
    
    children = []
    for child_shrinkable in shrinkable.shrinks().to_list():
        child_value = child_shrinkable.value
        child_key = tuple(child_value) if isinstance(child_value, list) else child_value
        
        if child_key not in visited:
            child_node = build_tree_structure(child_shrinkable, max_depth - 1, visited)
            children.append(child_node)
        else:
            children.append(visited[child_key])
    
    node = {"value": value, "children": children}
    visited[value_key] = node
    return node


def print_tree(node, prefix="", is_last=True, max_depth=10, current_depth=0):
    """Print the tree in a tree-like format."""
    if current_depth >= max_depth:
        return
    
    value = node["value"]
    if isinstance(value, list):
        display = format_list(value)
    else:
        display = str(value)
    
    # Print current node
    marker = "└── " if is_last else "├── "
    print(prefix + marker + display)
    
    # Print children
    children = node["children"]
    if children:
        for i, child in enumerate(children):
            is_last_child = (i == len(children) - 1)
            extension = "    " if is_last else "│   "
            print_tree(child, prefix + extension, is_last_child, max_depth, current_depth + 1)


def print_flat_list(shrinkable, max_depth=10, visited=None):
    """Print all values in the shrink tree in a flat list with indentation."""
    if visited is None:
        visited = set()
    
    value = shrinkable.value
    value_key = tuple(value) if isinstance(value, list) else value
    
    if value_key in visited or max_depth <= 0:
        return
    
    visited.add(value_key)
    
    if isinstance(value, list):
        print(f"  {format_list(value)}")
    else:
        print(f"  {value}")
    
    for child in shrinkable.shrinks().to_list():
        print_flat_list(child, max_depth - 1, visited)


def main():
    print("=" * 80)
    print("SHRINK TREE FOR INTEGER LIST OF SIZE 4")
    print("=" * 80)
    print("\nExample list: [10, 20, 30, 40]")
    print("\n" + "=" * 80)
    
    # Create shrinkables for each element
    element_shrinkables = [
        shrink_integral(10, 0, 100),
        shrink_integral(20, 0, 100),
        shrink_integral(30, 0, 100),
        shrink_integral(40, 0, 100),
    ]
    
    print("\n1. MEMBERSHIP-WISE SHRINKING ONLY")
    print("-" * 80)
    print("\nThis shows how the list shrinks by removing elements:")
    print("(removing from end, then from beginning)")
    
    list_shrinkable_membership = shrink_list(
        element_shrinkables,
        min_size=0,
        membership_wise=True,
        element_wise=False
    )
    
    tree_membership = build_tree_structure(list_shrinkable_membership, max_depth=10)
    print_tree(tree_membership, max_depth=10)
    
    print("\n" + "=" * 80)
    print("\n2. MEMBERSHIP-WISE + ELEMENT-WISE SHRINKING")
    print("-" * 80)
    print("\nThis shows how the list shrinks by both removing elements")
    print("and shrinking individual elements (showing first 3 levels):")
    
    list_shrinkable_full = shrink_list(
        element_shrinkables,
        min_size=0,
        membership_wise=True,
        element_wise=True
    )
    
    tree_full = build_tree_structure(list_shrinkable_full, max_depth=3)
    print_tree(tree_full, max_depth=3)
    
    print("\n" + "=" * 80)
    print("\n3. FLAT LIST OF ALL SHRINK CANDIDATES (Membership + Element-wise)")
    print("-" * 80)
    print("\nAll unique values in the shrink tree (limited to depth 4):")
    print_flat_list(list_shrinkable_full, max_depth=4)
    
    print("\n" + "=" * 80)
    print("\nEXPLANATION:")
    print("-" * 80)
    print("""
The shrink tree for a list of size 4 has two main strategies:

1. MEMBERSHIP-WISE SHRINKING:
   - Removes elements from the list
   - Tries: empty list, remove from end, remove from beginning
   - Order: [] (simplest), then progressively longer lists

2. ELEMENT-WISE SHRINKING:
   - Shrinks individual elements in the list
   - For each element, uses integer shrinking (binary search towards 0)
   - Tries simpler values first (0, then midpoints, etc.)

The tree structure ensures:
- No duplicates (each value appears once)
- Simplest first (try simplest values first)
- Logarithmic growth (binary search pattern for integers)
    """)


if __name__ == "__main__":
    main()

