#!/bin/bash

THIS_NAME=$(basename "$0")


function do_build()
{
    # FIXME: cd to THIS_PATH
    cd ./build
    cmake "$@" .. && make
}

function do_clean()
{
    # FIXME: cd to THIS_PATH
    echo "TODO: clean"
}

function do_get-submodules()
{
    # FIXME: cd to THIS_PATH
    # create git submodules (libs/)

    # glfw
    git submodule init libs/glfw          #glfw
    git submodule update libs/glfw

    # nanovg
    #git submodule init libs/nanovg        #nanovg
    #git submodule update libs/nanovg 

    # tinyxml2
    git submodule init libs/tinyxml2      #tinyxml
    git submodule update libs/tinyxml2

    # turbobadger
    git submodule init libs/turbobadger   #turbobadger
    git submodule update libs/turbobadger

    # libvpx
    #git submodule init libs/libvpx        #libvpx
    #git submodule update libs/libvpx


    # boost
    #git submodule init libs/boost         #boost
    #git submodule update libs/boost       
    #cd libs/boost
    #git submodule init     xxx
    #git submodule update   xxx
}

function do_usage()
{
    echo "usage:"
    echo "       $THIS_NAME                  : same as '$THIS_NAME build'"
    echo "       $THIS_NAME build            : build into './build' folder, using CMake"
    echo "       $THIS_NAME clean            : clean build"
    echo "       $THIS_NAME pull-submodules  : retrieve git-submodules (only needed for first build)"
}



################################################################################
#   main

if [ "$#" -eq 0 ] ; then
    # build program, default
    do_build
else 
    # handle arguments
    case "$1" in
      "build")  
          do_build
          ;;

      "clean")  
          do_clean
          ;;
      "pull-submodules")  
          do_pull-submodules
          ;;
      *)  
          do_usage
          ;;
    esac

fi


