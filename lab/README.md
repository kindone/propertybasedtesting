# Lab

Sandboxes that pull each port from GitHub so you can experiment without local path wiring.

| Lab | README |
|-----|--------|
| [cppproptest](cppproptest/) (CMake / C++23 / FetchContent → [cppproptest2](https://github.com/kindone/cppproptest2)) | [cppproptest/README.md](cppproptest/README.md) |
| [python-proptest](python-proptest/) (venv / pip Git URL → [python-proptest](https://github.com/kindone/python-proptest)) | [python-proptest/README.md](python-proptest/README.md) |
| [jsproptest](jsproptest/) (npm / `github:` + postinstall build → [jsproptest](https://github.com/kindone/jsproptest)) | [jsproptest/README.md](jsproptest/README.md) |

`build/`, `.venv/`, `node_modules/`, and common Python caches are listed in [`.gitignore`](../.gitignore).
