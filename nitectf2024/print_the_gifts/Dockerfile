FROM debian:bookworm-slim AS chroot

RUN useradd --no-create-home -u 1000 user

RUN apt update && \
    apt install -y socat libc6 && \
    rm -rf /var/lib/apt/lists/*

COPY chall /home/user/
COPY flag.txt /home/user/

RUN chmod +x /home/user/chall && \
    chmod 444 /home/user/flag.txt
WORKDIR /home/user/

USER user


CMD socat -T60 TCP-LISTEN:1337,reuseaddr,fork EXEC:"/home/user/chall"