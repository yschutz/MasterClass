#! /bin/bash
launch_root() 
{
	export OPT=$1
	root -l GUI.cxx
	RETVAL=$?
	return $RETVAL; 
}
# Move to MasterClass directory
START_DIR=`dirname ${BASH_SOURCE[0]}`
cd $START_DIR
if [ -f .config ]; then
	source .config
	echo "MC directory : "$MCDIR
	echo "ROOT directory : "$ROOTDIR
else
	echo "!!!! ERROR lcoal configuration NOT FOUND, please re-run install.sh"
	exit
fi
# Check ROOT
if [ -z "${ROOTSYS}" ]; then
	if [ -d $ROOTDIR ]; then 
		source $ROOTDIR/bin/thisroot.sh
	else
		echo "!!!! ERROR ROOTSYS is unset, install ROOT";
    	exit;
	fi
fi
DIRSAVE=`pwd`
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
	echo "!!!! ERROR: $LIBDIR not found!"
	cd $DIRSAVE
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
cd $DIRSAVE
