#!/usr/bin/env bash
set -euo pipefail

# Output files
TODO_REPORT=ci_metrics_todos.txt
CPPLINT_REPORT=ci_metrics_cpplint.txt
LIZARD_REPORT=ci_metrics_lizard.txt
SUMMARY=ci_metrics_summary.md

PYTHON_CANDIDATES=(
  "/opt/esp/python_env/idf6.0_py3.12_env/bin/python"
  "python3"
  "python"
)

resolve_python_for_module() {
  local module_name="$1"
  local py

  for py in "${PYTHON_CANDIDATES[@]}"; do
    if command -v "$py" >/dev/null 2>&1 && "$py" -m "$module_name" --version >/dev/null 2>&1; then
      echo "$py"
      return 0
    fi
  done

  for py in "${PYTHON_CANDIDATES[@]}"; do
    if ! command -v "$py" >/dev/null 2>&1; then
      continue
    fi

    if "$py" -m pip --version >/dev/null 2>&1; then
      "$py" -m pip install --user "$module_name" >/dev/null 2>&1 || "$py" -m pip install "$module_name" >/dev/null 2>&1 || true
      if "$py" -m "$module_name" --version >/dev/null 2>&1; then
        echo "$py"
        return 0
      fi
    fi
  done

  return 1
}

# 1. Gather TODOs
{
  echo "# TODOs in Codebase"
  grep -r --include='*.cpp' --include='*.hpp' --exclude-dir=build --exclude-dir=build/ --exclude-dir=tests/build --exclude-dir=node_modules --exclude-dir=.git 'TODO' main tests || echo 'No TODOs found.'
} > "$TODO_REPORT"

# 2. Run cpplint (C++ linting)
if CPPLINT_PY="$(resolve_python_for_module cpplint)"; then
  find main tests -type f \( -name '*.cpp' -o -name '*.hpp' \) | xargs -r "$CPPLINT_PY" -m cpplint --quiet --filter=-legal/copyright,-whitespace/line_length 2> "$CPPLINT_REPORT" || true
  if [[ ! -s "$CPPLINT_REPORT" ]]; then
    echo "No cpplint issues found." > "$CPPLINT_REPORT"
  fi
else
  echo "cpplint could not be resolved or installed in CI environment." > "$CPPLINT_REPORT"
fi

# 3. Run lizard (code complexity)
if LIZARD_PY="$(resolve_python_for_module lizard)"; then
  "$LIZARD_PY" -m lizard main tests > "$LIZARD_REPORT" 2>&1 || true
  if [[ ! -s "$LIZARD_REPORT" ]]; then
    echo "No lizard output produced." > "$LIZARD_REPORT"
  fi
else
  echo "lizard could not be resolved or installed in CI environment." > "$LIZARD_REPORT"
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
