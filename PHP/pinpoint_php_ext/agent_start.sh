#!/usr/bin/evn bash﻿
# set -x
set -e
PHP_EXT_DIR=$(dirname $(readlink -f "$0"))
PINPOINT_COMMON_DIR=$PHP_EXT_DIR/../../common
echo $PHP_EXT_DIR
func_build_common()
{
    cd $PINPOINT_COMMON_DIR
    if ! test -d "build"; then
        mkdir build 
    fi
    cd build
    # cmake -DCMAKE_BUILD_TYPE=Release .. 
    cmake -DCMAKE_BUILD_TYPE=Debug  .. 
    make
    cd $PHP_EXT_DIR
}


func_cp_common()
{
    cd $PHP_EXT_DIR
    cp $PINPOINT_COMMON_DIR/lib depends/  -rf
    cp $PINPOINT_COMMON_DIR/include depends/  -rf
    cd $PHP_EXT_DIR
}

func_build_php_agent()
{
    phpize
    ./configure 
    make
}

func_clean_last_build()
{
    if test -f "$PHP_EXT_DIR/configure"; then
        make clean
    fi
}

main()
{
    func_clean_last_build
    func_build_common
    func_cp_common
    func_build_php_agent
}

main
