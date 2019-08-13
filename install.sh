#!/bin/bash
INSTALDIR=$HOME
cd $INSTALDIR
MCDIR=$INSTALDIR/MasterClass
#Check if INSTALDIR/MasterClass exists and remove it 
if [ -d $IMCDIR ]; then 
    rm -fr $MCDIR
fi    
#download source code from github into $HOME/MasterClass 
git clone https://github.com/yschutz/MasterClass.git
cd MasterClass 
#compile and link
if [ -z ${ROOTSYS+x} ]; then
echo "ROOTSYS is unset: Install ROOT";
echo "Download the appropriate binary distributions from https://root.cern.ch/content/release-61800"
echo "Make sure gcc and C++ STL are installed"
    exit;
fi;
LIBDIR=$MCDIR/library
if [ -d "$LIBDIR" ]; then
	cd $LIBDIR
	make
    retVal=$?
 	if [ $retVal -ne 0 ]; then
    	echo "Compilation Error!"
		cd $MCDIR
		exit $retVal
	fi
    echo $MCDIR
	cd $MCDIR
else
	echo "ERROR: $LIBDIR not found!"
	exit 1
fi 