FROM ubuntu
ENV LC_CTYPE C.UTF-8
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
apt-get install -y socat 
RUN useradd -d /home/ctf4/ -m -p ctf -s /bin/bash ctf4
RUN echo "ctf4:ctf4" | chpasswd
WORKDIR /home/ctf4
COPY cute_little_vulnerable_storage . 
COPY libc-2.25.so .
COPY ld-2.25.so .
COPY libc.so.6 .
COPY flag.txt .
USER ctf4
EXPOSE 1027
CMD socat TCP-LISTEN:1027,fork,reuseaddr exec:"stdbuf -i0 -o0 -e0 ./cute_little_vulnerable_storage"
