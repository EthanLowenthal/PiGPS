sudo killall gpsd
lsof -n | grep /dev/ttyAMA0
sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock