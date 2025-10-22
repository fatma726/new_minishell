FROM ubuntu:22.04

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    valgrind \
    gcc \
    make \
    libreadline-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /minishell

# Copy source code
COPY . .

# Clean and build minishell
RUN make clean && make bonus

# Set up for testing
RUN chmod +x minishell_bonus

# Default command
CMD ["bash"]