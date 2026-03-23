# python-proptest lab

Installs [python-proptest](https://github.com/kindone/python-proptest) from Git via `requirements.txt`. Needs **Python ≥ 3.8**, **pip**, **Git**, and network.

```bash
python3 -m venv .venv && source .venv/bin/activate   # Windows: .venv\Scripts\activate
pip install -U pip && pip install -r requirements.txt
python src/poc_basic.py
```

**Pin revision:** edit the `@…` suffix on the URL in `requirements.txt` (branch, tag, or commit). **Use PyPI instead:** replace that line with e.g. `python-proptest==0.1.5`, then reinstall (a fresh venv avoids mixed Git/PyPI installs).

Add scripts under `src/` and extra deps in `requirements.txt` as needed.
