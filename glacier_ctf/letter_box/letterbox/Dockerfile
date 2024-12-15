# Alpine 3.19.1 (base system)

###############
# Base system #
###############
FROM docker.io/library/ubuntu@sha256:3f85b7caad41a95462cf5b787d8a04604c8262cdcdf9a472b8c52ef83375fe15 AS builder
# Install apt dependencies if needed

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y python3 python3-venv socat coreutils && apt-get clean

# Enable the virtual enviroment
RUN python3 -m venv /.venv
ENV PATH="/.venv/bin:$PATH"

# Install pip dependencies
COPY ./requirements.txt /app/requirements.txt
RUN pip install --no-cache-dir --upgrade -r /app/requirements.txt 

# Copy challenge required files
RUN mkdir -p /app
RUN mkdir -p /app/templates
COPY ./templates/login.html /app/templates/
COPY ./challenge /app/challenge
COPY ./web.sh /app/
COPY ./mail.sh /app/
COPY ./main /app/
RUN chmod +x /app/challenge
COPY ./flag.txt /flag.txt
COPY ./entrypoint.sh /app/entrypoint.sh
RUN chmod +x /app/entrypoint.sh

CMD /app/entrypoint.sh
