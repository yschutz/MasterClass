#!/bin/bash
GetOSName()
{
OS=`uname`
if [ "$OS" = "Linux" ]; then
    local dis=`lsb_release -i`
    OS=`echo ${dis#*:}`
    local ver=`lsb_release -r`
    ver=${ver#*:}
    ver=${ver%.*}
    OS=`echo $OS$ver gcc`
    local gccv=`gcc -dumpversion`
    OS=`echo $OS$gccv`
elif [ "$OS" == "Darwin" ]; then 
    OS=OsX
    OS=$OS" "`sw_vers -productVersion`
    OS=${OS%.*}
else 
    echo "ERROR: no root distribution for OS = $OS"      
fi
}
GetOSName
case $OS in 
    "OsX 10.14")
    echo https://root.cern/download/root_v6.18.00.macosx64-10.14-clang100.tar.gz
    ;;
    "OsX 10.13")
    echo https://root.cern/download/root_v6.18.00.macosx64-10.13-clang100.tar.gz
    ;;
    "Ubuntu 18 gcc7")
    echo https://root.cern/download/root_v6.18.00.Linux-ubuntu18-x86_64-gcc7.4.tar.gz 
    ;;
    "Ubuntu 16 gcc5")
    echo https://root.cern/download/root_v6.18.00.Linux-ubuntu16-x86_64-gcc5.4.tar.gz
    ;;
    "Ubuntu 14 gcc4")
    echo https://root.cern/download/root_v6.18.00.Linux-ubuntu14-x86_64-gcc4.8.tar.gz
    ;;
    *) 
    echo not binary root distribution for $OS; check here https://root.cern.ch/content/release-61800
esac
