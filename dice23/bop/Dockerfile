FROM ubuntu@sha256:bffb6799d706144f263f4b91e1226745ffb5643ea0ea89c2f709208e8d70c999

RUN apt-get update && apt install libc-bin
COPY flag.txt /srv/app/
COPY bop /srv/app/run

CMD ["/bin/sh"]
