# BPCells_zarrtest

How to build

```
cmake -DCMAKE_INSTALL_PREFIX=./package -B build . 
cmake --build build -j4
cmake --install build
mkdir -p package/libs
cp build/_deps/tensorstore-build/*.a package/libs/.
```



```
export ZARR_VERSION=2
./build/my_target 1000 10 10

export ZARR_VERSION=3
./build/my_target 1000 10 10
```

In order to run the quick benchmark: 

```
bash grid_benchmark.sh
```
