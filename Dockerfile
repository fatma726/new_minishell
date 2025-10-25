FROM ubuntu:22.04

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    valgrind \
    gcc \
    make \
    vim \
    libreadline-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /minishell

# Copy the minishell source code
COPY . .

# Build the project
RUN make re

# Set file descriptor limit
RUN ulimit -n 1024

# Default command
CMD ["/bin/bash"]
