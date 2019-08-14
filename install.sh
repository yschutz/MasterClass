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
        gccv=${gccv%.*}
        OS=`echo $OS$gccv`
    elif [ "$OS" == "Darwin" ]; then 
        OS=OsX
        OS=$OS" "`sw_vers -productVersion`
        OS=${OS%.*}
    else 
        echo "!!!! ERROR: no root distribution for OS = $OS"      
        exit
    fi
}
InstallRoot()
{
    GetOSName
    cd $INSTALDIR
    case $OS in 
        "OsX 10.14")
        root=root_v6.18.00.macosx64-10.14-clang100.tar.gz
        ;;
        "OsX 10.13")
        root=root_v6.18.00.macosx64-10.13-clang100.tar.gz
        ;;
        "Ubuntu 18 gcc7")
        root=root_v6.18.00.Linux-ubuntu18-x86_64-gcc7.4.tar.gz 
        ;;
        "Ubuntu 16 gcc5")
        root=root_v6.18.00.Linux-ubuntu16-x86_64-gcc5.4.tar.gz
        ;;
        "Ubuntu 14 gcc4")
        root=root_v6.18.00.Linux-ubuntu14-x86_64-gcc4.8.tar.gz
        ;;
        "Scientific 7 gcc4.8")
        root=root_v6.18.00.Linux-centos7-x86_64-gcc4.8.tar.gz
        ;;
        *) 
        echo "not binary root distribution for $OS; check here https://root.cern.ch/content/release-61800"
        exit
    esac
        wget https://root.cern/download/$root
        tar -zxvf $root
        rm $root
        source $INSTALDIR/root/bin/thisroot.sh
        echo "*****************************"
        echo root is installed at $ROOTSYS
        echo "*****************************"
}
INSTALDIR=$HOME/MC
if [ ! -d $INSTALDIR ]; then 
    mkdir $INSTALDIR
fi
cd $INSTALDIR
MCDIR=$INSTALDIR/MasterClass
#Check if INSTALDIR/MasterClass exists and remove it 
if [ -d $IMCDIR ]; then 
    rm -fr $MCDIR
fi    
#download source code from github into $HOME/MasterClass 
git clone https://github.com/yschutz/MasterClass.git
if [ $? -eq 127 ]; then 
    echo "!!!! ERROR: you need to install git on your computer"
    exit
fi    
#check if root is installed and install it if requested
if [ -z "${ROOTSYS}" ]; then
	echo "ROOTSYS is unset: You need to install ROOT";
	echo "Download the appropriate binary distributions from https://root.cern.ch/content/release-61800"
	echo "Make sure gcc and C++ STL are installed on your computer"
	gcc --version
	if [ $? -eq 127 ]; then 
		echo "You need to install gcc (version 4.8, 5.4 or 7.4) and C++ STL"
		exit
	fi
	echo "Do you wish to install root?"
	select yn in "Yes" "No"; do
    	case $yn in
        	Yes ) InstallRoot; break;;
        	No ) exit;;
    	esac
	done
fi;
cd MasterClass 
#compile and link
LIBDIR=$MCDIR/library
if [ -d "$LIBDIR" ]; then
	cd $LIBDIR
	make
    retVal=$?
 	if [ $retVal -ne 0 ]; then
    	echo "!!!! Compilation Error !!!"
		cd $MCDIR
		exit $retVal
	fi
    echo $MCDIR
	cd $MCDIR
else
	echo "!!!! ERROR: $LIBDIR not found"
	exit 1
fi 
echo "Installation completed .... "
echo "To start the exercises: $MCDIR/MasterClassStart.sh"
