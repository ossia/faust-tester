#!/bin/bash
export BUILD_FOLDER=/tmp/build
export SOURCE_FOLDER="$PWD"
wget -nv https://github.com/ossia/sdk/releases/download/sdk26/sdk-linux.tar.xz
tar xaf sdk-linux.tar.xz
rm -rf  sdk-linux.tar.xz

mkdir -p $BUILD_FOLDER

echo '#!/bin/bash -eux

export OSSIA_SDK=/opt/ossia-sdk
export PATH=$OSSIA_SDK/llvm/bin:$PATH
export LD_LIBRARY_PATH=$OSSIA_SDK/llvm/lib

export CC=$OSSIA_SDK/llvm/bin/clang
export CXX=$OSSIA_SDK/llvm/bin/clang++

cd /build
cmake /src \
  -DOSSIA_SDK=/opt/ossia-sdk \
  -DBUILD_TESTING=1 \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake --build .

ctest --output-on-failure -V
' > build-script.sh
chmod +x build-script.sh

docker pull ossia/score-package-linux
docker run \
           -v "build-script.sh:/build-script.sh" \
           --mount type=bind,source="$PWD/opt/ossia-sdk",target=/opt/ossia-sdk \
           --mount type=bind,source="$SOURCE_FOLDER",target=/src \
           --mount type=bind,source="$BUILD_FOLDER",target=/build \
           ossia/score-package-linux \
           /bin/bash /src/build-script.sh

