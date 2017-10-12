#!/bin/sh
if [ ! -d './third-lib/gflags' ]; then
    cd third-lib/
    git clone https://github.com/gflags/gflags 
    cd gflags/
    mkdir build
    cd build
    cmake ..
    make
    cd ../../..
fi
if [ ! -d './third-lib/glog' ]; then
    cd third-lib/
    mkdir -p glog
    cd glog
    git clone https://github.com/google/glog.git ./
    cmake ./
    cd ../..
fi
#./tool/protoc -I=./include/net/policy/ --cpp_out=./include/net/policy  include/net/policy/*.proto
protoc -I=./proto/ --cpp_out=./proto/  proto/*.proto
protoc -I=./test/ --cpp_out=./test/  test/*.proto
set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-./build}
BUILD_TYPE=${BUILD_TYPE:-release}
INSTALL_DIR=${INSTALL_DIR:-install}
PREFIX_PATH='./;./third-lib/gflags/build'
#PREFIX_PATH='./'
mkdir -p $BUILD_DIR/$BUILD_TYPE \
  && cd $BUILD_DIR/$BUILD_TYPE \
  && CXX="g++" cmake \
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
           "-DCMAKE_PREFIX_PATH=$PREFIX_PATH" \
           $SOURCE_DIR \
  && make $*

# Use the following command to run all the unit tests
# at the dir $BUILD_DIR/$BUILD_TYPE :
# CTEST_OUTPUT_ON_FAILURE=TRUE make test

# cd $SOURCE_DIR && doxygen

