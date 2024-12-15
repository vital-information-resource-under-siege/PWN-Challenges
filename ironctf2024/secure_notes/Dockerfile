FROM pwn.red/jail
COPY --from=ubuntu / /srv
COPY flag.txt /srv/app/
COPY libc.so.6 /srv/app/
COPY ld-linux-x86-64.so.2 /srv/app/
ENV JAIL_TIME=120
COPY SecureNotes /srv/app/run
