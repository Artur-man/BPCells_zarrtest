cmake -DCMAKE_INSTALL_PREFIX=./package -B build . 
cmake --build build -j4
cmake --install build
mkdir -p package/libs
cp build/_deps/tensorstore-build/*.a package/libs/.
