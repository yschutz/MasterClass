REM Download
powershell -command "& { iwr  https://ecsft.cern.ch/dist/cernvm/launch/bin/Win/cernvm-launch.exe -OutFile cernvm-launch.exe  }"

REM Run
hostname > .tmp
set /p HOSTNAME=<.tmp
set name=MCat%HOSTNAME%
del .tmp
REM if ! ./
setlocal
cernvm-launch create --name %name% USER_DATA_FILE CONFIGURATION_FILE
if errorlevel 1 cernvm-launch start %name%
del cernvm-launch.exe