#!/bin/bash -eux
export OSSIA_SDK=/opt/ossia-sdk-x86_64

sudo xcode-select -s /Applications/Xcode.app/Contents/Developer

mkdir build
cd build

xcrun cmake -GNinja .. \
  -DOSSIA_SDK=$OSSIA_SDK \
  -DBUILD_TESTING=1 \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake --build .

ctest --output-on-failure -V