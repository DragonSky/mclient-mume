#!/bin/bash

autoreconf -if
if [ ! -d build ]; then mkdir build; fi
cd build
../configure && make && make install
