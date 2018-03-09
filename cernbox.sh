#
case $1 in
linux)
firefox https://cernbox.cern.ch/index.php/s/zw0cULC2TIkcBMZ &
;;
windows1)
firefox https://cernbox.cern.ch/index.php/s/One9UfxlZYkzD9T &
;;
windows2)
firefox https://cernbox.cern.ch/index.php/s/FA1cugP86eMRR4G &
;;
*)
echo "Usage: cernbox.sh [linux | windows1 | windows2]"
esac
