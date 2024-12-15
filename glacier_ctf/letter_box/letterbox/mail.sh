#!/bin/sh
socat TCP-LISTEN:8081,fork,nodelay,reuseaddr,pktinfo EXEC:"/usr/bin/stdbuf -i0 -o0 -e0 /app/main"
