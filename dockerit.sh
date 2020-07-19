#!/bin/sh
docker build -f Dockerfile.Ubuntu20.04 -t chapter6 .
docker run -it --entrypoint /bin/bash chapter6
