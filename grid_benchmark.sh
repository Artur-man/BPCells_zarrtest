export ZARR_VERSION=2
rm -f test.txt
rm -f test_all.txt
for shape in 5000000 10000000 20000000 40000000 160000000; do
  for chunk in 500000 1000000 2500000 5000000; do
    for append in 1000000 2000000 8000000 16000000 3200000; do
        for iter in 1 2 3 4 5; do
            rm -rf /tmp/testo/zarrV2/
            ./build/my_target $shape $chunk $append
            cat test.txt >> test_all.txt
        done
    done 
  done 
done 