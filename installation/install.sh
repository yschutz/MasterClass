#!/bin/bash
# Download
if [ "x$(uname -s)" = "xLinux" ]; then
  curl -o cernvm-launch https://ecsft.cern.ch/dist/cernvm/launch/bin/Linux/cernvm-launch
else
  curl -o cernvm-launch https://ecsft.cern.ch/dist/cernvm/launch/bin/Mac/cernvm-launch
fi
chmod +x cernvm-launch  # make it executable
# As root user, place it in a directory from the $PATH environment
#sudo mv        cernvm-launch /usr/local/bin

# Run
export name=`echo MCat$HOSTNAME | cut -d. -f1`
./cernvm-launch create --name $name USER_DATA_FILE CONFIGURATION_FILE
rm cernvm-launch
if [ "x$(uname -s)" = "xDarwin" ]; then
  /Applications/Firefox.app/Contents/MacOS/firefox https://cernvm-online.cern.ch/dashboard/ &
else
  firefox  https://cernvm-online.cern.ch/dashboard/ &
fi
echo ========================================================================
echo pair the context definition MasterClass with the virtual machine MasterClass
echo login to your virtual machine as user name #<PIN Code>
echo ========================================================================
