set -e
make clean
make
cd ../tests
make clean
make
