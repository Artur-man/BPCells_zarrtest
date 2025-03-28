# BPCells_zarrtest

```
cmake -DCMAKE_INSTALL_PREFIX=./package -B build . 
cmake --build build -j4
cmake --install build
# make -C build -j 4
# make install

export ZARR_VERSION=2
./build/my_target 1000 10 10

export ZARR_VERSION=3
./build/my_target 1000 10 10
```

In order to run the quick benchmark: 

```
bash grid_benchmark.sh
```
