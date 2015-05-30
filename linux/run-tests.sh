set -e
make clean
make -j4
cd ../tests
make clean
make -j4
