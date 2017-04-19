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
export DATAPATH="$PWD/data/"
export IMPATH="$DATAPATH/images/"
export EVTPATH="$DATAPATH/events/$EXPATH"
export GEOPATH="$DATAPATH/geometry/"
. /Applications/root_v6.08.06/bin/thisroot.sh
echo $PART
if [[ $PART = "Part1/" ]]; then 
	root start.C
elif [[ $PART = "Part2/" ]]; then 
	cd $EXPATH$PART$ROLE
	root start.C	
fi
rm *.pcm *.d *so
