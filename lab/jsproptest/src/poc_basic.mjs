import { Gen, Property } from 'jsproptest'

try {
    const ok = new Property((x, y) => x + y === y + x)
        .setSeed('42')
        .setNumRuns(64)
        .forAll(Gen.interval(-1_000_000, 1_000_000), Gen.interval(-1_000_000, 1_000_000))
    if (!ok) {
        console.error('poc_basic: property failed')
        process.exit(1)
    }
    console.log('poc_basic: property passed')
} catch (e) {
    console.error(e)
    process.exit(1)
}
