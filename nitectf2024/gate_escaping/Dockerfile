FROM debian:bookworm-slim

RUN useradd --no-create-home -u 1001 user

RUN apt update && \
    apt install -y --no-install-recommends socat && \
    rm -rf /var/lib/apt/lists/*

COPY chall /home/user/
COPY flag /

RUN chmod +x /home/user/chall && \
    chmod 444 /flag

WORKDIR /home/user
CMD socat -T60 TCP-LISTEN:1337,reuseaddr,fork EXEC:"/home/user/chall"
