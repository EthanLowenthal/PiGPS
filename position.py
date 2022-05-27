import threading
import time
from gps import *

# sudo killall gpsd
# lsof -n | grep /dev/ttyAMA0
# sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock


class GpsPoller(threading.Thread):

   def __init__(self):
       threading.Thread.__init__(self)
       self.session = gps(mode=WATCH_ENABLE)
       self.current_value = None

   def get_current_value(self):
       return self.current_value

   def run(self):
       try:
            while True:
                self.current_value = self.session.next()
                time.sleep(0.2) # tune this, you might not get values that quickly
       except StopIteration:
            pass

def getPositionData(data):
    # For a list of all supported classes and fields refer to:
    # https://gpsd.gitlab.io/gpsd/gpsd_json.html
    if data['class'] == 'TPV':
        print(data)
        latitude = getattr(data,'lat', "Unknown")
        longitude = getattr(data,'lon', "Unknown")
        print("Your position: lat, lon = " + str(latitude) + ", " + str(longitude))


if __name__ == '__main__':

   gpsp = GpsPoller()
   gpsp.start()
   # gpsp now polls every .2 seconds for new data, storing it in self.current_value
   while 1:
       # In the main thread, every 5 seconds print the current value
        time.sleep(0.2)
        data = gpsp.get_current_value()
        getPositionData(data)
