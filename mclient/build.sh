#!/bin/bash

if [ -d build ]; then rm -rf build; fi
mkdir build && cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=. && make -j3 && make install

ln -sf lib plugins
ln -sf ../config config
