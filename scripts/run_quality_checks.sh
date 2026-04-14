#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_ROOT"

HOST_TEST_BUILD_DIR="/tmp/esp32framework2-tests-build"
WEBSITE_DIR="main/project_dmx_controller/website"
SUBMODULE_HEADER="main/libraries/esp_littlefs/include/esp_littlefs.h"

echo "========== QUALITY: SUBMODULE CHECK =========="
if [[ ! -f "$SUBMODULE_HEADER" ]]; then
  echo "Missing $SUBMODULE_HEADER"
  echo "Run: git submodule update --init --recursive"
  exit 1
fi

echo ""
echo "========== QUALITY: CLANG-FORMAT =========="
find main tests -type f \( -name "*.cpp" -o -name "*.hpp" \) | xargs -r clang-format -n --Werror

echo ""
echo "========== QUALITY: PRETTIER (REPO) =========="
if [[ -x "$WEBSITE_DIR/node_modules/.bin/prettier" ]]; then
  PRETTIER_BIN="$WEBSITE_DIR/node_modules/.bin/prettier"
elif command -v prettier >/dev/null 2>&1; then
  PRETTIER_BIN="$(command -v prettier)"
else
  echo "Prettier not found. Installing website dependencies to obtain local Prettier..."
  npm --prefix "$WEBSITE_DIR" ci --include=optional --no-audit --no-fund
  PRETTIER_BIN="$WEBSITE_DIR/node_modules/.bin/prettier"
fi

"$PRETTIER_BIN" --check "**/*.{js,ts,css,scss,md,json,html}"

echo ""
echo "========== QUALITY: CLANG-TIDY (PRODUCTION) =========="
cmake -S tests -B "$HOST_TEST_BUILD_DIR" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null
COMPILE_DB="$HOST_TEST_BUILD_DIR/compile_commands.json"
if [[ ! -f "$COMPILE_DB" ]]; then
  echo "compile_commands.json not found at $COMPILE_DB"
  exit 1
fi

grep -o '"file": "[^"]*"' "$COMPILE_DB" \
  | sed 's/"file": "//; s/"$//' \
  | grep '^/workspace/main/.*\\.cpp$' \
  | sort -u > /tmp/clang_tidy_files.txt

if [[ ! -s /tmp/clang_tidy_files.txt ]]; then
  echo "No production source files found for clang-tidy"
  exit 1
fi

xargs -a /tmp/clang_tidy_files.txt clang-tidy -p "$HOST_TEST_BUILD_DIR" --warnings-as-errors=* --quiet

echo ""
echo "========== QUALITY: WEBSITE =========="
if [[ ! -f "$WEBSITE_DIR/package.json" ]]; then
  echo "Missing $WEBSITE_DIR/package.json"
  exit 1
fi

# Match CI behavior: always perform a clean install before website checks.
npm --prefix "$WEBSITE_DIR" ci --include=optional --no-audit --no-fund
npm --prefix "$WEBSITE_DIR" run format:check
npm --prefix "$WEBSITE_DIR" run build
npm --prefix "$WEBSITE_DIR" run test

echo ""
echo "Quality checks passed."
