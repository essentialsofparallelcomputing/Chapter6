#!/bin/sh
docker build --no-cache -t chapter6 .
#docker run -it --entrypoint /bin/bash chapter6
docker build --no-cache -f Dockerfile.Ubuntu20.04 -t chapter6 .
#docker run -it --entrypoint /bin/bash chapter6
docker build --no-cache -f Dockerfile.debian -t chapter6 .
#docker run -it --entrypoint /bin/bash chapter6
