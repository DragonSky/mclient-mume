#!/bin/bash

if [ ! -d build ]; then mkdir build; fi
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=. && make && make install
ln -sf lib plugins
