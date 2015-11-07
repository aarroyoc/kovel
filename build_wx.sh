#!/bin/bash
mkdir build
cd build
cmake .. -DWX_UI=ON
make
cd ..
cd test
./test_dae.sh
./test_json.sh
