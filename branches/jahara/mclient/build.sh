#!/bin/bash

if [ -d build ]; then
    cd build
    make -j3
    make install
else
    mkdir build
    cd build
    cmake ../ -DCMAKE_INSTALL_PREFIX=. && make -j3 && make install
    ln -sf lib plugins
    ln -sf ../config config
fi