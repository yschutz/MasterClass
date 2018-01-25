#! /bin/bash
help() 
{
	echo "usage: ./start.sh [-h | --help] [-e | --exercise RAA | Strangeness] [-p | --part 1 | 2] [-l | --langue | --language FR | EN] [-r | --role | --user student | teacher | example]"
	exit 1
}

if [[ $# -eq 0 ]]; then 
	help
fi
export BLA="FR"
export ROLE=""
while [[ $# -gt 0 ]] 
do 
	key="$1"
	case $key in 
		-h | --help)
		help
		;;
		-e | --exercise)
		EXPATH="$2"
		shift
		;;
		-p | --part)
		export PART="Part$2/"
		shift
		;;
		-l | --langue | --language)
		export BLA="$2"
		shift
		;;
		-r | --role)
		export ROLE="$2/"
		shift
		;;
		default)
		;;
		*)
		echo OPTION NON RECONNUE / UNKNOW OPTION
		exit 1
		shift
		;;
	esac
shift
done
export EXPATH="$EXPATH/"
export DATAPATH="root://eospublic.cern.ch//eos/experiment/alice/MasterClass.data/data/"
export IMPATH="images/"
export EVTPATH="$DATAPATH/events/$EXPATH"
export GEOPATH="$DATAPATH/geometry/"
if [ "x$(uname -s)" = "xDarwin" ]; then
 . /Applications/root_v6.09.02/bin/thisroot.sh
elif [ "x$(uname -s)" = "xLinux" ]; then
 # export PATH=/cvmfs/sft.cern.ch/lcg/releases/gcc/4.8.4/x86_64-slc6/bin/:$PATH
 # export LD_LIBRARY_PATH=/cvmfs/sft.cern.ch/lcg/releases/gcc/4.8.4/x86_64-slc6/lib64:$LD_LIBRARY_PATH
 source /cvmfs/sft.cern.ch/lcg/contrib/gcc/4.8/x86_64-slc6-gcc48-opt/setup.sh
 source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.04.02/x86_64-slc6-gcc48-opt/root/bin/thisroot.sh	
fi
if [[ $PART = "Part1/" ]]; then 
	root start.C
elif [[ $PART = "Part2/" ]]; then 
	cd $EXPATH$PART$ROLE
	root start.C	
fi
cd $EXPATH/$PART
rm *.pcm *.d
cd -
