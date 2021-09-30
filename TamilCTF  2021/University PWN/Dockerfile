FROM ubuntu
ENV LC_CTYPE C.UTF-8
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
apt-get install -y socat 
RUN useradd -d /home/ctf5/ -m -p ctf -s /bin/bash ctf5
RUN echo "ctf5:ctf5" | chpasswd
WORKDIR /home/ctf5
COPY akka_university . 
COPY libc.so.6 .
COPY ld-2.31.so .
COPY libc6_2.31-0ubuntu9.2_amd64.so .
COPY flag.txt .
USER ctf5
EXPOSE 1029
CMD socat TCP-LISTEN:1029,fork,reuseaddr exec:"stdbuf -i0 -o0 -e0 ./akka_university"
