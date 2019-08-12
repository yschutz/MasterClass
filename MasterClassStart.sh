#! /bin/bash
if [ -z ${ROOTSYS+x} ]; then
echo "ROOTSYS is unset: Install ROOT";
    exit;
fi;
MCDIR=/Users/schutz/cernbox/MC
LIBDIR=$MCDIR/library
if [ -d "$LIBDIR" ]; then
	cd $LIBDIR
	make
    retVal=$?
 	if [ $retVal -ne 0 ]; then
    	echo "Compilation Error!"
		cd ..
		exit $retVal
	fi
	cd ..
else
	echo "ERROR: $LIBDIR not found!"
	exit 1
fi 
SAVE=$LD_LIBRARY_PATH
CPATH=`pwd`	
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CPATH/library
root -l GUI.cxx
export LD_LIBRARY_PATH=$SAVE
