FROM ubuntu
ENV LC_CTYPE C.UTF-8
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
apt-get install -y socat
RUN useradd -d /home/ctf2/ -m -p ctf -s /bin/bash ctf2
RUN echo "ctf2:ctf2" | chpasswd
WORKDIR /home/ctf2
COPY chall2 .
COPY flag.txt .
USER ctf2
EXPOSE 1337
CMD socat TCP-LISTEN:1337,fork,reuseaddr exec:"stdbuf -i0 -o0 -e0 ./chall2"
