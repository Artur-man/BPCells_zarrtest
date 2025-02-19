# BPCells_zarrtest

```
cmake -B build .
make -C build -j 4

export ZARR_VERSION=2
./build/my_target

export ZARR_VERSION=3
./build/my_target
```

In order to run the quick benchmark: 

```
bash grid_benchmark.sh
```
