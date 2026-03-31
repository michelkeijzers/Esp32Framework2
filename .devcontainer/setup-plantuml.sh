#!/usr/bin/env bash

set -euo pipefail

PLANTUML_VERSION="1.2025.10"
PLANTUML_DIR="/workspace/.tools/plantuml"
PLANTUML_JAR="${PLANTUML_DIR}/plantuml-${PLANTUML_VERSION}.jar"
PLANTUML_URL="https://github.com/plantuml/plantuml/releases/download/v${PLANTUML_VERSION}/plantuml-${PLANTUML_VERSION}.jar"

if ! command -v java >/dev/null 2>&1; then
    echo "Java is not installed. Rebuild the dev container to apply the Dockerfile changes."
    exit 1
fi

mkdir -p "${PLANTUML_DIR}"

if [ ! -f "${PLANTUML_JAR}" ]; then
    tmp_file="${PLANTUML_JAR}.tmp"
    rm -f "${tmp_file}"
    curl -fL "${PLANTUML_URL}" -o "${tmp_file}"
    mv "${tmp_file}" "${PLANTUML_JAR}"
fi

echo "PlantUML ready: ${PLANTUML_JAR}"