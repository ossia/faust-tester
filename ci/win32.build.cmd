mkdir build
cd build

set PATH=%PATH%;c:\ossia-sdk\llvm\bin
cmake -GNinja %BUILD_SOURCESDIRECTORY% ^
  -DCMAKE_C_COMPILER=c:/ossia-sdk/llvm/bin/clang.exe ^
  -DCMAKE_CXX_COMPILER=c:/ossia-sdk/llvm/bin/clang++.exe ^
  -DOSSIA_SDK=c:\ossia-sdk ^
  -DBUILD_TESTING=1 ^
  -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake --build .

ctest --output-on-failure -V