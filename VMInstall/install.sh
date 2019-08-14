#!/bin/bash
# Download
if [ "x$(uname -s)" = "xLinux" ]; then
  curl -o cernvm-launch https://ecsft.cern.ch/dist/cernvm/launch/bin/Linux/cernvm-launch
elif [ "x$(uname -s)" = "xDarwin" ]; then
  curl -o cernvm-launch https://ecsft.cern.ch/dist/cernvm/launch/bin/Mac/cernvm-launch
fi 
chmod +x cernvm-launch  # make it executable
# As root user, place it in a directory from the $PATH environment
#sudo mv        cernvm-launch /usr/local/bin

# Run
export name=`echo MCat$HOSTNAME | cut -d. -f1`
if ! ./cernvm-launch create --name $name USER_DATA_FILE CONFIGURATION_FILE; then 
	./cernvm-launch start $name
fi	
# rm cernvm-launch