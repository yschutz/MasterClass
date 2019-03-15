#! /bin/bash
help() 
{
	echo "usage: ./Bash_StartExercise.sh [-h | --help] [-e | --exercise RAA | Strangeness] [-p | --part 1 | 2] [-l | --langue | --language FR | EN]"
	exit 1
}

if [[ $# -eq 0 ]]; then 
	help
fi
export BLA="FR"
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
		default)
		;;
		*)
		echo "OPTION NON RECONNUE / UNKNOWN OPTION"
		exit 1
		shift
		;;
	esac
shift
done
export EXPATH="$EXPATH/"
if [ -d Data-Masterclass ]
then 
     export DATAPATH="Data-Masterclass"
else 
     export DATAPATH="root://eospublic.cern.ch//eos/experiment/alice/MasterClass.data/data/"
fi
export IMPATH="images/"
export EVTPATH="$DATAPATH/events/$EXPATH"
export GEOPATH="$DATAPATH/geometry/"
if [ "x$(uname -s)" = "xDarwin" ]; then
 . /Applications/root_v6.09.02/bin/thisroot.sh
elif [ "x$(uname -s)" = "xLinux" ]; then
 # export PATH=/cvmfs/sft.cern.ch/lcg/releases/gcc/4.8.4/x86_64-slc6/bin/:$PATH
 # export LD_LIBRARY_PATH=/cvmfs/sft.cern.ch/lcg/releases/gcc/4.8.4/x86_64-slc6/lib64:$LD_LIBRARY_PATH
  if [ -d /cvmfs/sft.cern.ch ]
  then 
        echo "CVMFS found ... load root envt from CVMFS..."
        source /cvmfs/sft.cern.ch/lcg/contrib/gcc/4.8/x86_64-centos7-gcc48-opt/setup.sh
        source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.14.06/x86_64-centos7-gcc48-opt/bin/thisroot.sh
        echo ""
  else 
        # eval `/usr/local/etc/use/chenv  cern2017`
        echo "no ROOT found automatically under CVMFS, try to issue it manually..."
        source /libcern/root/6.14.06/CentOS7.3-x86_64/setup.sh
        echo ""
  fi
fi
if [[ $PART = "Part1/" ]]; then
        echo "Launch part 1..."
# 	root -l start.C
elif [[ $PART = "Part2/" ]]; then 
        echo "Launch part 2..."
# 	cd $EXPATH$PART
fi

	root -l start.C	


cd $EXPATH/$PART
rm *.pcm *.d
cd -
