FROM espressif/idf:v5.4.1

# Install additional tools


# Install Java (OpenJDK 17) for PlantUML and other tools, and Graphviz for PlantUML diagram rendering
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl \
    gcovr \
    lcov \
    openjdk-17-jre-headless \
    graphviz \
    && rm -rf /var/lib/apt/lists/*

# Node.js 20 LTS via NodeSource
RUN curl -fsSL https://deb.nodesource.com/setup_20.x | bash - \
    && apt-get install -y nodejs \
    && rm -rf /var/lib/apt/lists/*

# Source IDF export in non-interactive shells used by VS Code tasks
RUN echo ". \$IDF_PATH/export.sh > /dev/null 2>&1 || true" >> /root/.bashrc
