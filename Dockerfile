FROM ubuntu:22.04

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    valgrind \
    gcc \
    make \
    vim \
    libreadline-dev \
    python3 python3-pip \
    net-tools \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /minishell

# Copy the minishell source code
COPY . .

# Install norminette for style checks
RUN pip3 install -q norminette

# Build the project (will be re-built when volume is mounted)
RUN make re

# Set file descriptor limit
RUN ulimit -n 1024

# Default command
CMD ["/bin/bash"]
