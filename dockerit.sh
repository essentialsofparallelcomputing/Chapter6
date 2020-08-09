#!/bin/sh
docker build -t chapter6 .
docker run -it --entrypoint /bin/bash chapter6
