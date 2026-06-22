FROM debian:bookworm

RUN apt-get update && apt-get install -y \
    build-essential \
    libfltk1.3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make

CMD ["./build/bin/schelling"]