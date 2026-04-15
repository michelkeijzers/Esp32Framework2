#!/usr/bin/env bash
set -euo pipefail

MODE="full"
while [[ $# -gt 0 ]]; do
  case "$1" in
    --lizard-only)
      MODE="lizard-only"
      shift
      ;;
    *)
      echo "Unknown argument: $1" >&2
      exit 1
      ;;
  esac
done

OUTPUT_DIR="${CI_METRICS_OUTPUT_DIR:-$PWD}"
mkdir -p "$OUTPUT_DIR"

TODO_REPORT="$OUTPUT_DIR/ci_metrics_todos.txt"
CPPLINT_REPORT="$OUTPUT_DIR/ci_metrics_cpplint.txt"
LIZARD_REPORT="$OUTPUT_DIR/ci_metrics_lizard.txt"
LIZARD_CSV_REPORT="$OUTPUT_DIR/ci_metrics_lizard.csv"
METRICS_JSON="$OUTPUT_DIR/ci_metrics_metrics.json"
SUMMARY="$OUTPUT_DIR/ci_metrics_summary.md"

LIZARD_CCN_THRESHOLD="${LIZARD_CCN_THRESHOLD:-18}"
LIZARD_NLOC_THRESHOLD="${LIZARD_NLOC_THRESHOLD:-89}"
LIZARD_LENGTH_THRESHOLD="${LIZARD_LENGTH_THRESHOLD:-107}"
LIZARD_ARGUMENT_THRESHOLD="${LIZARD_ARGUMENT_THRESHOLD:-9}"

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

write_placeholder() {
  local path="$1"
  local message="$2"
  printf '%s\n' "$message" > "$path"
}

create_cpp_source_list() {
  local source_list
  source_list="$(mktemp)"

  find main tests -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) \
    ! -path '*/build/*' \
    ! -path '*/node_modules/*' \
    ! -path '*/dist/*' \
    ! -path '*/.venv/*' \
    ! -path '*/.api-simulator-venv/*' \
    ! -path 'main/libraries/esp_littlefs/*' \
    | sort > "$source_list"

  echo "$source_list"
}

run_todos=1
run_cpplint=1
if [[ "$MODE" == "lizard-only" ]]; then
  run_todos=0
  run_cpplint=0
fi

if (( run_todos )); then
  todo_matches="$(mktemp)"
  if ! grep -r --include='*.c' --include='*.cpp' --include='*.h' --include='*.hpp' --exclude-dir=build --exclude-dir=tests/build --exclude-dir=node_modules --exclude-dir=.git 'TODO' main tests > "$todo_matches"; then
    :
  fi

  {
    echo "# TODOs in Codebase"
    if [[ -s "$todo_matches" ]]; then
      cat "$todo_matches"
    else
      echo "No TODOs found."
    fi
  } > "$TODO_REPORT"
  rm -f "$todo_matches"
else
  write_placeholder "$TODO_REPORT" "Skipped in lizard-only mode."
fi

if (( run_cpplint )); then
  if CPPLINT_PY="$(resolve_python_for_module cpplint)"; then
    find main tests -type f \( -name '*.cpp' -o -name '*.hpp' \) \
      ! -path '*/build/*' \
      ! -path '*/node_modules/*' \
      ! -path '*/dist/*' \
      ! -path '*/.venv/*' \
      ! -path '*/.api-simulator-venv/*' \
      ! -path 'main/libraries/esp_littlefs/*' \
      | xargs -r "$CPPLINT_PY" -m cpplint --quiet --filter=-legal/copyright,-whitespace/line_length 2> "$CPPLINT_REPORT" || true
    if [[ ! -s "$CPPLINT_REPORT" ]]; then
      write_placeholder "$CPPLINT_REPORT" "No cpplint issues found."
    fi
  else
    write_placeholder "$CPPLINT_REPORT" "cpplint could not be resolved or installed in the current environment."
  fi
else
  write_placeholder "$CPPLINT_REPORT" "Skipped in lizard-only mode."
fi

source_list="$(create_cpp_source_list)"
trap 'rm -f "$source_list"' EXIT

lizard_exit=0
if LIZARD_PY="$(resolve_python_for_module lizard)"; then
  if [[ -s "$source_list" ]]; then
    set +e
    "$LIZARD_PY" -m lizard \
      -l cpp \
      -f "$source_list" \
      --csv \
      -C "$LIZARD_CCN_THRESHOLD" \
      -L "$LIZARD_LENGTH_THRESHOLD" \
      -a "$LIZARD_ARGUMENT_THRESHOLD" \
      -T "nloc=$LIZARD_NLOC_THRESHOLD" \
      -i 0 \
      > "$LIZARD_CSV_REPORT"
    lizard_exit=$?
    set -e
  else
    write_placeholder "$LIZARD_CSV_REPORT" ""
  fi
else
  lizard_exit=1
  write_placeholder "$LIZARD_CSV_REPORT" ""
fi

export MODE
export LIZARD_EXIT_CODE="$lizard_exit"
export LIZARD_CCN_THRESHOLD
export LIZARD_NLOC_THRESHOLD
export LIZARD_LENGTH_THRESHOLD
export LIZARD_ARGUMENT_THRESHOLD

python_for_reports="${LIZARD_PY:-python3}"
"$python_for_reports" - "$LIZARD_CSV_REPORT" "$source_list" "$LIZARD_REPORT" "$METRICS_JSON" "$SUMMARY" "$TODO_REPORT" "$CPPLINT_REPORT" <<'PY'
import csv
import json
import os
import sys
from datetime import datetime, timezone
from pathlib import Path

csv_path = Path(sys.argv[1])
source_list_path = Path(sys.argv[2])
lizard_report_path = Path(sys.argv[3])
metrics_json_path = Path(sys.argv[4])
summary_path = Path(sys.argv[5])
todo_report_path = Path(sys.argv[6])
cpplint_report_path = Path(sys.argv[7])

mode = os.environ.get("MODE", "full")
lizard_exit_code = int(os.environ.get("LIZARD_EXIT_CODE", "0"))
thresholds = {
    "cyclomatic_complexity": int(os.environ.get("LIZARD_CCN_THRESHOLD", "18")),
    "nloc": int(os.environ.get("LIZARD_NLOC_THRESHOLD", "89")),
    "length": int(os.environ.get("LIZARD_LENGTH_THRESHOLD", "107")),
    "parameter_count": int(os.environ.get("LIZARD_ARGUMENT_THRESHOLD", "9")),
}


def read_lines(path: Path, limit: int | None = None) -> list[str]:
    if not path.exists():
        return []
    with path.open("r", encoding="utf-8", errors="replace") as handle:
        lines = [line.rstrip("\n") for line in handle]
    return lines[:limit] if limit is not None else lines


def render_excerpt(lines: list[str], fallback: str) -> list[str]:
    return lines if lines else [fallback]


source_files = [line.strip() for line in read_lines(source_list_path) if line.strip()]
file_line_metrics: list[dict[str, object]] = []
for source_file in source_files:
    source_path = Path(source_file)
    try:
        with source_path.open("r", encoding="utf-8", errors="replace") as handle:
            line_count = sum(1 for _ in handle)
    except OSError:
        line_count = 0
    file_line_metrics.append({"path": source_file, "lines": line_count})

total_cpp_lines = sum(item["lines"] for item in file_line_metrics)
largest_files = sorted(file_line_metrics, key=lambda item: int(item["lines"]), reverse=True)[:5]
largest_file = largest_files[0] if largest_files else {"path": "", "lines": 0}

rows: list[dict[str, object]] = []
if csv_path.exists():
    with csv_path.open("r", encoding="utf-8", errors="replace") as handle:
        for row in csv.reader(handle):
            if not row or row[0] == "NLOC":
                continue
            try:
                rows.append(
                    {
                        "nloc": int(row[0]),
                        "cyclomatic_complexity": int(row[1]),
                        "token_count": int(row[2]),
                        "parameter_count": int(row[3]),
                        "length": int(row[4]),
                        "location": row[5],
                        "file": row[6],
                        "function": row[7],
                        "signature": row[8],
                        "start_line": int(row[9]),
                        "end_line": int(row[10]),
                    }
                )
            except (IndexError, ValueError):
                continue

warning_counts = {
    "cyclomatic_complexity": sum(1 for row in rows if row["cyclomatic_complexity"] > thresholds["cyclomatic_complexity"]),
    "nloc": sum(1 for row in rows if row["nloc"] > thresholds["nloc"]),
    "length": sum(1 for row in rows if row["length"] > thresholds["length"]),
    "parameter_count": sum(1 for row in rows if row["parameter_count"] > thresholds["parameter_count"]),
}
threshold_failed = any(count > 0 for count in warning_counts.values()) or lizard_exit_code != 0
status = "pass"
if lizard_exit_code != 0 and not rows:
    status = "error"
elif threshold_failed:
    status = "fail"

maxima = {}
for metric in ("cyclomatic_complexity", "nloc", "length", "parameter_count"):
    if rows:
        best = max(rows, key=lambda item: int(item[metric]))
        maxima[metric] = {
            "value": int(best[metric]),
            "function": best["function"],
            "file": best["file"],
            "start_line": int(best["start_line"]),
            "end_line": int(best["end_line"]),
        }
    else:
        maxima[metric] = {
            "value": 0,
            "function": "",
            "file": "",
            "start_line": 0,
            "end_line": 0,
        }

top_offenders = {}
for metric in ("cyclomatic_complexity", "nloc", "length", "parameter_count"):
    top_rows = sorted(rows, key=lambda item: int(item[metric]), reverse=True)[:5]
    top_offenders[metric] = [
        {
            "value": int(item[metric]),
            "function": str(item["function"]),
            "file": str(item["file"]),
            "start_line": int(item["start_line"]),
            "end_line": int(item["end_line"]),
        }
        for item in top_rows
    ]

todo_lines = read_lines(todo_report_path, 20)
cpplint_lines = read_lines(cpplint_report_path, 20)

metrics_payload = {
    "generated_at_utc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
    "mode": mode,
    "cpp_sources": {
        "file_count": len(source_files),
        "total_lines": total_cpp_lines,
        "largest_file": largest_file,
        "largest_files": largest_files,
    },
    "lizard": {
        "status": status,
        "exit_code": lizard_exit_code,
        "threshold_failed": threshold_failed,
        "thresholds": thresholds,
        "function_count": len(rows),
        "warning_counts": warning_counts,
        "maxima": maxima,
        "top_offenders": top_offenders,
    },
}

with metrics_json_path.open("w", encoding="utf-8") as handle:
    json.dump(metrics_payload, handle, indent=2)
    handle.write("\n")

lizard_lines = [
    "Lizard Quality Gate",
    "",
    f"Status: {status.upper()}",
    f"Files scanned: {len(source_files)}",
    f"Functions scanned: {len(rows)}",
    f"Total C/C++ lines: {total_cpp_lines}",
    f"Largest file: {largest_file['path']} ({largest_file['lines']} lines)",
    "",
    "Thresholds:",
    f"  Cyclomatic complexity <= {thresholds['cyclomatic_complexity']}",
    f"  Function NLOC <= {thresholds['nloc']}",
    f"  Function length <= {thresholds['length']}",
    f"  Parameters <= {thresholds['parameter_count']}",
    "",
    "Warnings:",
    f"  Cyclomatic complexity: {warning_counts['cyclomatic_complexity']}",
    f"  Function NLOC: {warning_counts['nloc']}",
    f"  Function length: {warning_counts['length']}",
    f"  Parameters: {warning_counts['parameter_count']}",
    "",
    "Maxima:",
    f"  Cyclomatic complexity: {maxima['cyclomatic_complexity']['value']} ({maxima['cyclomatic_complexity']['function']})",
    f"  Function NLOC: {maxima['nloc']['value']} ({maxima['nloc']['function']})",
    f"  Function length: {maxima['length']['value']} ({maxima['length']['function']})",
    f"  Parameters: {maxima['parameter_count']['value']} ({maxima['parameter_count']['function']})",
    "",
    "Top offenders:",
]

for metric in ("cyclomatic_complexity", "nloc", "length", "parameter_count"):
    lizard_lines.append(f"  {metric}:")
    offenders = top_offenders[metric]
    if not offenders:
        lizard_lines.append("    None")
        continue
    for offender in offenders:
        lizard_lines.append(
            f"    {offender['value']}: {offender['function']} ({offender['file']}:{offender['start_line']})"
        )

with lizard_report_path.open("w", encoding="utf-8") as handle:
    handle.write("\n".join(lizard_lines))
    handle.write("\n")

summary_lines = ["# CI Metrics Summary", ""]

if mode != "lizard-only":
    summary_lines.extend(["## TODOs", *render_excerpt(todo_lines, "No TODOs found."), ""])
    summary_lines.extend(["## cpplint (C++ Lint) Issues", *render_excerpt(cpplint_lines, "No cpplint issues found."), ""])

summary_lines.extend(
    [
        "## Code Quality (lizard)",
        f"- Status: {status.upper()}",
        f"- Source files scanned: {len(source_files)}",
        f"- Functions scanned: {len(rows)}",
        f"- Total C/C++ lines: {total_cpp_lines}",
        f"- Largest file: {largest_file['path']} ({largest_file['lines']} lines)",
        f"- Thresholds: CCN <= {thresholds['cyclomatic_complexity']}, NLOC <= {thresholds['nloc']}, length <= {thresholds['length']}, args <= {thresholds['parameter_count']}",
        f"- Warning counts: CCN {warning_counts['cyclomatic_complexity']}, NLOC {warning_counts['nloc']}, length {warning_counts['length']}, args {warning_counts['parameter_count']}",
        f"- Max complexity: {maxima['cyclomatic_complexity']['value']} ({maxima['cyclomatic_complexity']['function']})",
        f"- Max function NLOC: {maxima['nloc']['value']} ({maxima['nloc']['function']})",
        f"- Max function length: {maxima['length']['value']} ({maxima['length']['function']})",
        f"- Max parameter count: {maxima['parameter_count']['value']} ({maxima['parameter_count']['function']})",
        "",
        "### Largest C/C++ Files",
    ]
)

if largest_files:
    for file_metric in largest_files:
        summary_lines.append(f"- {file_metric['path']}: {file_metric['lines']} lines")
else:
    summary_lines.append("- No C/C++ source files found.")

summary_lines.extend(["", "### Top Lizard Offenders"])
for metric in ("cyclomatic_complexity", "nloc", "length", "parameter_count"):
    offenders = top_offenders[metric][:3]
    if offenders:
        for offender in offenders:
            summary_lines.append(
                f"- {metric}: {offender['value']} in {offender['function']} ({offender['file']}:{offender['start_line']})"
            )
    else:
        summary_lines.append(f"- {metric}: no entries")

summary_lines.extend(["", "---", "Full reports are available as CI artifacts."])

with summary_path.open("w", encoding="utf-8") as handle:
    handle.write("\n".join(summary_lines))
    handle.write("\n")
PY

echo "CI metrics generated: $SUMMARY"
exit "$lizard_exit"
