#! /bin/bash
launch_root() 
{
	export OPT=$1
	root -l GUI.cxx
	RETVAL=$?
	return $RETVAL; 
}
if [ -z ${ROOTSYS+x} ]; then
echo "ROOTSYS is unset: Install ROOT";
    exit;
fi;
MCDIR=`pwd`
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
RETVAL=1;
while [ $RETVAL -ne 0 ]; do  
	launch_root $RETVAL
	RETVAL=$?
done
export LD_LIBRARY_PATH=$SAVE
