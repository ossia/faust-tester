#!/bin/bash
export BUILD_FOLDER=/tmp/build
export SOURCE_FOLDER="$PWD"
wget -nv https://github.com/ossia/sdk/releases/download/sdk26/sdk-linux.tar.xz
tar xaf sdk-linux.tar.xz
rm -rf  sdk-linux.tar.xz

mkdir -p $BUILD_FOLDER
ln -s $BUILD_FOLDER build

echo "#!/bin/bash -eux

cd /build
cmake -GNinja /src \
  -DOSSIA_SDK=/opt/ossia-sdk \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake --build .

ctest --output-on-failure -V
" > build-script.sh
chmod +x build-script.sh

docker pull ossia/score-package-linux
docker run \
           -v "build-script.sh:/Recipe" \
           --mount type=bind,source="$PWD/opt/ossia-sdk",target=/opt/ossia-sdk \
           --mount type=bind,source="$SOURCE_FOLDER",target=/src \
           --mount type=bind,source="$BUILD_FOLDER",target=/build \
           ossia/score-package-linux \
           /bin/bash /Recipe

