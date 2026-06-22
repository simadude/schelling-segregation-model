FROM debian:bookworm

RUN apt-get update && apt-get install -y \
    build-essential \
    libfltk-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make build

CMD ["./build/bin/schelling"]
