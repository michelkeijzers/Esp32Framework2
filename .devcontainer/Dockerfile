FROM espressif/idf:v5.4.1

# Install additional tools
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl \
    gcovr \
    lcov \
    && rm -rf /var/lib/apt/lists/*

# Node.js 20 LTS via NodeSource
RUN curl -fsSL https://deb.nodesource.com/setup_20.x | bash - \
    && apt-get install -y nodejs \
    && rm -rf /var/lib/apt/lists/*

# Source IDF export in non-interactive shells used by VS Code tasks
RUN echo ". \$IDF_PATH/export.sh > /dev/null 2>&1 || true" >> /root/.bashrc
