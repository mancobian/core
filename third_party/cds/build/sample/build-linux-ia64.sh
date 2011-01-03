#!/bin/sh
clear

export BOOST_ROOT=~/boost

GCC_ROOT=/usr/local/gcc-4.3.3
export PATH=${GCC_ROOT}/bin:${PATH}
export LD_LIBRARY_PATH=${GCC_ROOT}/lib:${LD_LIBRARY_PATH}

./build.sh -b 64 \
--clean \
-x 'g++' \
-z '-std=c++0x' \
-l '-L${BOOST_ROOT}/bin/linux-ia64' \
--with-boost ${BOOST_ROOT} \
2>&1 | tee build-linux-ia64.log

