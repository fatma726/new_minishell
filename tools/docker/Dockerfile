FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    build-essential \
    valgrind \
    libreadline-dev \
    pkg-config \
    bash \
    git \
    ca-certificates \
    python3 \
    python3-pip \
  && rm -rf /var/lib/apt/lists/*

# Norminette (42)
RUN pip3 install --no-cache-dir norminette

WORKDIR /work
ENTRYPOINT ["/bin/bash", "-lc"]

