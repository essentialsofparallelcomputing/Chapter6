FROM intel/oneapi-basekit:devel-ubuntu18.04
WORKDIR /project
RUN apt-get update && \
    apt-get install -y bash cmake git vim gcc apt-utils && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
RUN apt-get update && \
   apt-get install -y intel-hpckit-getting-started intel-oneapi-common-vars intel-oneapi-common-licensing \
      intel-oneapi-itac intel-oneapi-mpi-devel intel-oneapi-dev-utilities intel-oneapi-tbb-devel \
      intel-oneapi-icc intel-oneapi-ifort intel-oneapi-clck intel-oneapi-inspector

SHELL ["/bin/bash", "-c"]

RUN groupadd -r chapter6 && useradd -r -s /bin/false -g chapter6 chapter6

WORKDIR /chapter6
RUN chown -R chapter6:chapter6 /chapter6
USER chapter6

RUN source /opt/intel/inteloneapi/setvars.sh
RUN git clone --recursive https://github.com/essentialsofparallelcomputing/Chapter6.git

ENTRYPOINT ["bash"]
