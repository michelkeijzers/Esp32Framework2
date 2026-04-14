import fs from "node:fs";
import path from "node:path";

function assertExists(filePath, label = "file") {
  if (!fs.existsSync(filePath)) {
    throw new Error(`Missing ${label}: ${filePath}`);
  }
}

function assertNonEmptyDir(dirPath, label = "directory") {
  assertExists(dirPath, label);
  const entries = fs.readdirSync(dirPath);
  if (entries.length === 0) {
    throw new Error(`Empty ${label}: ${dirPath}`);
  }
}

const root = process.cwd();

// Smoke-check source files required for the website/app simulator workflow.
const requiredSourceFiles = [
  "index.html",
  "control.html",
  "configuration.html",
  "security.html",
  "server.py",
  path.join("scripts", "sync-public.mjs"),
  path.join("scripts", "ensure-rollup-native.mjs"),
];

for (const relPath of requiredSourceFiles) {
  assertExists(path.join(root, relPath), "source file");
}

// If build output exists, validate key output artifacts as well.
const distDir = path.join(root, "dist");
if (fs.existsSync(distDir)) {
  assertExists(path.join(distDir, "index.html"), "build output file");
  assertNonEmptyDir(path.join(distDir, "assets"), "build assets directory");
}

console.log("Website smoke tests passed.");
