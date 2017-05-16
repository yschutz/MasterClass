powershell -command "& { iwr https://raw.githubusercontent.com/yschutz/MasterClass/master/installation/install.sh -OutFile install.bat }"

REM the configuration files

powershell -command "& { iwr  https://raw.githubusercontent.com/yschutz/MasterClass/master/installation/CONFIGURATION_FILE -OutFile CONFIGURATION_FILE }"

REM the user data file

powershell -command "& { iwr  https://raw.githubusercontent.com/yschutz/MasterClass/master/installation/USER_DATA_FILE -OutFile USER_DATA_FILE }"

REM  run the scrit

install.bat