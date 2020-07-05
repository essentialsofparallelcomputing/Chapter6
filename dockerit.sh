#!/bin/sh
docker build --no-cache -t chapter6 .
docker run -it --entrypoint /bin/bash chapter6
