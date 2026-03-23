# cppproptest lab

Fetches [cppproptest2](https://github.com/kindone/cppproptest2) via CMake FetchContent and links `proptest`. Needs **CMake ≥ 3.21**, **C++23**, **Git**, and network (first configure also pulls GoogleTest inside cppproptest2).

```bash
cmake -S . -B build
cmake --build build --target poc_basic
./build/poc_basic
```

Windows (VS-style build): `build\Release\poc_basic.exe` (or `Debug`).

**Other repo / revision:** `-DCPPPROPTEST_GIT_REPOSITORY=...` and `-DCPPPROPTEST_GIT_TAG=...` (branch, tag, or commit; defaults in `CMakeLists.txt`). Re-run CMake in a clean `build/` if those change.

**More targets:** add `src/*.cpp` and an `add_executable` that links `proptest`, uses `cxx_std_23`, and `target_include_directories(..., "${cppproptest2_SOURCE_DIR}")`; copy the `PROPTEST_DLL` block from `poc_basic` if you link a shared `proptest`.
