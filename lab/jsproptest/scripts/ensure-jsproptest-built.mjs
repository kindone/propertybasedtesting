/**
 * jsproptest's GitHub tree does not ship `dist/`; npm releases do. After a git
 * dependency install, run `npm install` + `npm run build` inside the package.
 */
import { spawnSync } from 'node:child_process'
import { existsSync } from 'node:fs'
import path from 'node:path'
import { fileURLToPath } from 'node:url'

const labRoot = path.join(path.dirname(fileURLToPath(import.meta.url)), '..')
const pkgRoot = path.join(labRoot, 'node_modules', 'jsproptest')
const distMain = path.join(pkgRoot, 'dist', 'index.js')

if (existsSync(distMain)) {
    process.exit(0)
}

const npm = process.platform === 'win32' ? 'npm.cmd' : 'npm'
const run = (args) => {
    const r = spawnSync(npm, args, { cwd: pkgRoot, stdio: 'inherit' })
    if (r.error) throw r.error
    if (r.status !== 0) process.exit(r.status ?? 1)
}

run(['install'])
run(['run', 'build'])

if (!existsSync(distMain)) {
    console.error('ensure-jsproptest-built: dist/index.js still missing after build')
    process.exit(1)
}
