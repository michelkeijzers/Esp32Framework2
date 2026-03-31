import { createRequire } from 'node:module';
import os from 'node:os';

const require = createRequire(import.meta.url);

function expectedRollupPackage() {
  if (process.platform !== 'linux') {
    return null;
  }

  if (process.arch === 'x64') {
    return '@rollup/rollup-linux-x64-gnu';
  }

  if (process.arch === 'arm64') {
    return '@rollup/rollup-linux-arm64-gnu';
  }

  return null;
}

const expectedPkg = expectedRollupPackage();

if (!expectedPkg) {
  console.log(`Skipping native Rollup preflight for ${process.platform}/${process.arch}.`);
  process.exit(0);
}

try {
  require.resolve(`${expectedPkg}/package.json`);
  console.log(`Native Rollup package found: ${expectedPkg}`);
} catch {
  const platformSummary = `${process.platform}/${process.arch}`;
  console.error(`Missing native Rollup package for ${platformSummary}: ${expectedPkg}`);
  console.error('Run: npm run repair-deps');
  console.error(`Node.js: ${process.version} | Host: ${os.hostname()}`);
  process.exit(1);
}
