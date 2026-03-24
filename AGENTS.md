# AGENTS.md

## Cursor Cloud specific instructions

This repository is a **documentation and analysis workspace** for property-based testing (PBT) frameworks. It contains primarily Markdown documentation and 5 Python analysis scripts. There is no build system, no test suite, no linter config, and no CI/CD.

### Running the analysis scripts

The only executable code is in `python-proptest/`. These scripts depend on the `python-proptest` PyPI package (installed via `pip install python-proptest`).

Run any script with:
```
python3 python-proptest/<script_name>.py
```

All 5 scripts:
- `show_list_shrink_tree.py` — visualizes shrink trees for hardcoded lists
- `show_list_shrink_tree_detailed.py` — detailed tree visualization with hierarchy
- `show_gen_list_shrink_tree.py` — shrink trees from `Gen.list(Gen.int(), 0, 4)` (full int range, large output)
- `show_gen_list_shrink_tree_bounded.py` — shrink trees with bounded integers (0-100)
- `show_gen_list_shrink_tree_size3.py` — complete shrink tree for lists of size 3

### Key notes

- No lint, test, or build commands exist for this repo. The repo is purely documentation + analysis scripts.
- The scripts use `sys.path.insert(0, os.path.dirname(__file__))` but the actual `python_proptest` package must be installed from PyPI — the sibling `../python-proptest` repo is not available in the Cloud VM.
- The VS Code workspace file `proptestports.code-workspace` references sibling repos (`../python-proptest`, `../cppproptest2`, `../dartproptest`, `../jsproptest`) that are not part of this repository.
