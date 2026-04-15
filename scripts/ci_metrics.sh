#!/usr/bin/env bash
set -euo pipefail

# Output files
TODO_REPORT=ci_metrics_todos.txt
CPPLINT_REPORT=ci_metrics_cpplint.txt
LIZARD_REPORT=ci_metrics_lizard.txt
SUMMARY=ci_metrics_summary.md

# 1. Gather TODOs
{
  echo "# TODOs in Codebase"
  grep -r --include='*.cpp' --include='*.hpp' --exclude-dir=build --exclude-dir=build/ --exclude-dir=tests/build --exclude-dir=node_modules --exclude-dir=.git 'TODO' main tests || echo 'No TODOs found.'
} > "$TODO_REPORT"

# 2. Run cpplint (C++ linting)
cpplint_found=0
if command -v cpplint >/dev/null 2>&1; then
  find main tests -type f \( -name '*.cpp' -o -name '*.hpp' \) | xargs -r cpplint --quiet --filter=-legal/copyright,-whitespace/line_length 2> "$CPPLINT_REPORT" || true
  cpplint_found=1
elif command -v python >/dev/null 2>&1 && python -m cpplint --version >/dev/null 2>&1; then
  find main tests -type f \( -name '*.cpp' -o -name '*.hpp' \) | xargs -r python -m cpplint --quiet --filter=-legal/copyright,-whitespace/line_length 2> "$CPPLINT_REPORT" || true
  cpplint_found=1
elif command -v python3 >/dev/null 2>&1 && python3 -m cpplint --version >/dev/null 2>&1; then
  find main tests -type f \( -name '*.cpp' -o -name '*.hpp' \) | xargs -r python3 -m cpplint --quiet --filter=-legal/copyright,-whitespace/line_length 2> "$CPPLINT_REPORT" || true
  cpplint_found=1
fi
if [[ $cpplint_found -eq 0 ]]; then
  echo "cpplint not found or not installed." > "$CPPLINT_REPORT"
fi

# 3. Run lizard (code complexity)
lizard_found=0
if command -v lizard >/dev/null 2>&1; then
  find main tests -type f \( -name '*.cpp' -o -name '*.hpp' \) | xargs -r lizard > "$LIZARD_REPORT" || true
  lizard_found=1
elif command -v python >/dev/null 2>&1 && python -m lizard --version >/dev/null 2>&1; then
  find main tests -type f \( -name '*.cpp' -o -name '*.hpp' \) | xargs -r python -m lizard > "$LIZARD_REPORT" || true
  lizard_found=1
elif command -v python3 >/dev/null 2>&1 && python3 -m lizard --version >/dev/null 2>&1; then
  find main tests -type f \( -name '*.cpp' -o -name '*.hpp' \) | xargs -r python3 -m lizard > "$LIZARD_REPORT" || true
  lizard_found=1
fi
if [[ $lizard_found -eq 0 ]]; then
  echo "lizard not found or not installed." > "$LIZARD_REPORT"
fi

# 4. Generate Markdown summary
{
  echo "# CI Metrics Summary"
  echo
  echo "## TODOs"
  head -20 "$TODO_REPORT"
  echo
  echo "## cpplint (C++ Lint) Issues"
  head -20 "$CPPLINT_REPORT"
  echo
  echo "## Code Complexity (lizard)"
  head -20 "$LIZARD_REPORT"
  echo
  echo "---"
  echo "Full reports are available as CI artifacts."
} > "$SUMMARY"

echo "CI metrics generated: $SUMMARY"
