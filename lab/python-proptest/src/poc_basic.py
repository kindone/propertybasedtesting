"""Minimal lab script: depends on python-proptest (see ../requirements.txt)."""

from __future__ import annotations

from python_proptest import Gen, run_for_all


def main() -> None:
    ok = run_for_all(
        lambda x, y: x + y == y + x,
        Gen.int(),
        Gen.int(),
        num_runs=64,
        seed=42,
    )
    if not ok:
        raise SystemExit("poc_basic: property failed")
    print("poc_basic: property passed")


if __name__ == "__main__":
    main()
