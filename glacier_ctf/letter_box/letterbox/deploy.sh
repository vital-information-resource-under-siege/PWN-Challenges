#!/bin/sh

check() {
  echo -e "\e[1;34m[+] Verifying Challenge Integrity\e[0m"
  sha256sum -c sha256sum
}

build_container() {
  echo -e "\e[1;34m[+] Building Challenge Docker Container\e[0m"
  docker build -t localhost/chall-letterbox --platform linux/amd64 . 
}

# Common error on default Ubuntu 24.04:
# 
# initCloneNs():391 mount('/', '/', NULL, MS_REC|MS_PRIVATE, NULL): Permission denied
# Change --user 1337:1337 to --user 0:0 in run_container()
# or
# $ sudo sysctl -w kernel.apparmor_restrict_unprivileged_unconfined=0
# $ sudo sysctl -w kernel.apparmor_restrict_unprivileged_userns=0
# and then restore them back when finished

run_container() {
  echo -e "\e[1;34m[+] Running Challenge Docker Container on 127.0.0.1:1337\e[0m"
  docker run --name chall-letterbox --rm -p 1337:1337 -p 8081:8081 -it localhost/chall-letterbox
}

kill_container() {
	docker ps --filter "name=chall-letterbox" --format "{{.ID}}" \
		| tr '\n' ' ' \
		| xargs docker stop -t 0 \
		|| true
}

case "${1}" in
  "check")
    check
    ;;
  "build")
    build_container
    ;;
  "run")
    run_container
    ;;
  "kill")
    kill_container
    ;;
  *)
    check
    build_container
    run_container
    ;;
esac
