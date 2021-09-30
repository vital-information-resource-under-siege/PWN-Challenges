FROM ubuntu
ENV LC_CTYPE C.UTF-8
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
apt-get install -y socat 
RUN useradd -d /home/ctf3/ -m -p ctf -s /bin/bash ctf3
RUN echo "ctf3:ctf3" | chpasswd
WORKDIR /home/ctf3
COPY chall3 . 
COPY flag.txt .
USER ctf3
EXPOSE 1028
CMD socat TCP-LISTEN:1028,fork,reuseaddr exec:"stdbuf -i0 -o0 -e0 ./chall3"
