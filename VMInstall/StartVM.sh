#!/bin/bash
if ! which virtualbox > /dev/null; then
 echo "you need to instal VirtualBox first from: https://www.virtualbox.org"
  if [ "x$(uname -s)" = "xLinux" ]; then
    echo "you need to download and install VMBox"
    if ! firefox https://www.virtualbox.org/wiki/Linux_Downloads; then
      echo "firefox not found on your computer"
      exit 1
    fi
  elif [ "x$(uname -s)" = "xDarwin" ]; then
    curl -o VirtualBox-5.1.20-114629-OSX.dmg http://download.virtualbox.org/virtualbox/5.1.20/VirtualBox-5.1.20-114629-OSX.dmg
    hdiutil attach VirtualBox-5.1.20-114629-OSX.dmg
    installer -package /Volumes/VirtualBox/VirtualBox.pkg -target /
    hdiutil detach /Volumes/VirtualBox/
  fi
fi
#dowload the installation files from github
# the install script (to launch the Virtual Machine for MacOS and Linux)
 curl -o install.sh https://raw.githubusercontent.com/yschutz/MasterClass/master/VMInstall/install.sh
# the configuration files
curl -o CONFIGURATION_FILE https://raw.githubusercontent.com/yschutz/MasterClass/master/VMInstall/CONFIGURATION_FILE
# the user data file
curl -o USER_DATA_FILE https://raw.githubusercontent.com/yschutz/MasterClass/master/VMInstall/USER_DATA_FILE
make the scrit executable and run it
chmod +x install.sh
./install.sh