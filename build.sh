#!/bin/bash

THIS_NAME=$(basename "$0")
BINARY_NAME="open-forest"


function do_build()
{
    # FIXME: cd to THIS_PATH
    mkdir -p build/cmake
    cd build/cmake
    cmake -DCMAKE_BUILD_TYPE=None "$@" ../.. && make
}

function do_make()
{
    # FIXME: cd to THIS_PATH
    cd build/cmake
    make 
}

function do_make_openforest()
{
    # FIXME: cd to THIS_PATH
    cd build/cmake
    make open-forest/fast 
}

function do_clean()
{
    # FIXME: cd to THIS_PATH
    cd build
    rm -rf cmake

    # (ignoring build/open-forest)
}

function do_init-submodules()
{
    # FIXME: cd to THIS_PATH
    # create git submodules (libs/)
    
    git submodule init
    git submodule update

}

function do_usage()
{
    echo "usage: $THIS_NAME                  : same as '$THIS_NAME build'"
    echo "       $THIS_NAME build            : configure and build into './build' folder, using CMake"
    echo "       $THIS_NAME make             : make preconfigured CMake by 'build'"
    echo "       $THIS_NAME make-openforest  : make openforest part of preconfigured CMake by 'build'"
    echo "       $THIS_NAME clean            : clean build"
    echo "       $THIS_NAME init-submodules  : retrieve git-submodules (only needed for first build)"
}



################################################################################
#   main


if [ "$#" -eq 0 ] ; then
    # build program, default
    echo ""
    echo "running '$THIS_NAME build'. use '$THIS_NAME help' for other options."
    do_build
else 
    # handle arguments
    case "$1" in
      "build")  
          do_build
          ;;
      "make")  
          do_make
          ;;
      "make-openforest")  
          do_make_openforest
          ;;
      "clean")  
          do_clean
          ;;
      "init-submodules")  
          do_init-submodules
          ;;
      "help")
          do_usage
          ;;
      *)  
          do_usage
          ;;
    esac

fi


