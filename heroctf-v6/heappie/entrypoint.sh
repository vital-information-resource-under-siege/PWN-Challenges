#!/bin/bash

while :
do
    su challenge -c "exec socat TCP-LISTEN:${LISTEN_PORT},reuseaddr,fork EXEC:'/app/heappie,stderr'";
done