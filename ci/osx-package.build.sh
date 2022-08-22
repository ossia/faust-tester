#!/bin/bash -eux

mkdir build
cd build

cmake .. \
  -DOSSIA_SDK=/opt/ossia-sdk-x86_64 \
  -DBUILD_TESTING=1 \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake --build .

ctest --output-on-failure -V