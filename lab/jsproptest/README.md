# jsproptest lab

Depends on [jsproptest](https://github.com/kindone/jsproptest) via **`github:kindone/jsproptest`** (default branch). The upstream repo has no prebuilt `dist/`; **`npm install` runs a postinstall script** that runs `npm install` + `npm run build` inside `node_modules/jsproptest` when `dist/` is missing. Published installs from the npm registry already include `dist/` and skip that step.

Needs **Node** `^18.18 || ^20.9 || >=21.1` (same engines as jsproptest), **npm**, **Git**, and network.

```bash
npm install
npm start
# or: node src/poc_basic.mjs
```

**Registry instead of Git:** in `package.json`, set `"jsproptest": "^0.5.4"` (or another version), then `npm install` again.

**Fork / branch:** use e.g. `"jsproptest": "github:you/jsproptest#your-branch"`.

Add scripts under `src/` and wire them in `package.json` as needed.
