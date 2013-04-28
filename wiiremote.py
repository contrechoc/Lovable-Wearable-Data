import cwiid
import time
import threading
import urllib2
import serial
import re
import urllib
from lxml import html
urlContent = ""
lineNumber = 100
sunActivity = 0
listLength = 0

def getExoPlanet():
  #url = "http://www.swpc.noaa.gov/ftpdir/lists/ace/ace_swepam_1m.txt"
  global urlContent
  global listLength
  print "start"
  #try:
    #url = "https://raw.github.com/hannorein/oec_tables/master/comma_separated/open_exoplanet_catalogue.txt"
   # url = "exoplanet.txt"
    #urlContent = urllib2.urlopen(url).read(10000)
    #urlContent=urlContent.split("\n")
  #except IOError:
   # print "exoplanet problem"
  #for line in urlContent:
  #  print line
  try:
    f = open("exoplanet.txt", "r")
    try:
    # Read the entire contents of a file at once.
      urlContent = f.read()
      urlContent=urlContent.split("\n")
      listLength = len( urlContent )
      print listLength
      print "------"     
# print urlContent
    finally:
      f.close()
  except IOError:
    pass

def sendUART(whichData):
  ser = serial.Serial("/dev/ttyAMA0", 9600, timeout=1.0);
  if whichData == "exoplanet":
    thisLine = urlContent[lineNumber].split(',')
    writeString="exo*" + str(thisLine[0])+"*"+str(thisLine[1])+"*"+ str(thisLine[2])+"*"+ str(thisLine[3])+"*"+ str(thisLine[4])
    lengthStr = len(writeString)
    while len(writeString)<40:
      writeString = writeString + "x"
    ser.write(writeString)
    #ser.write("exo")
    #time.sleep(.1)
    #ser.write((thisLine[0]))
    #time.sleep(.1)
    #ser.write((thisLine[1]))
    #time.sleep(.1)
    #ser.write((thisLine[2]))
    #time.sleep(.1)
    #ser.write((thisLine[3]))
    print writeString 
  if whichData == "sunactivity":
    ser.write("sun " + str(lastLine[len(lastLine)-1]))
  ser.close()

def getSunActivity():
  global sunActivity
  url = "http://www.swpc.noaa.gov/ftpdir/lists/ace/ace_swepam_1m.txt"
  urlContent = urllib2.urlopen(url).read(200000)
  urlContent=urlContent.split("\n")
  lastLineNumber = len(urlContent) - 2
  checkNumber = -9999
  while (checkNumber < -9000) and (lastLineNumber > -1):
    recentLine = urlContent[lastLineNumber]
    recentLine = ' '.join(recentLine.split())
    print recentLine
    lastLine = recentLine.split(" ")
        #print lastLine[len(lastLine)-1]
        #print lastLine[len(lastLine)-2]
    checkNumber = float(lastLine[len(lastLine)-2])
    lastLineNumber = lastLineNumber -1

  print "sunactivity " + lastLine[len(lastLine)-1]
  #ser = serial.Serial("/dev/ttyAMA0", 9600, timeout=1.0);
  #ser.write("f " + str(lastLine[len(lastLine)-1]))
  #ser.close()



 
button_delay = 0.1
 
print 'Press 1 + 2 on your Wii Remote now ...'
time.sleep(1)
 
# Connect to the Wii Remote. If it times out
# then quit.
try:
  wii=cwiid.Wiimote()
except RuntimeError:
  print "Error opening wiimote connection"
  quit()
 
print 'Wii Remote connected...\n'
print 'Press some buttons!\n'
print 'Press PLUS and MINUS together to disconnect and quit.\n'
 
wii.rpt_mode = cwiid.RPT_BTN | cwiid.RPT_ACC
print 'get exoplanet file'
getExoPlanet()

while True:
 
  buttons = wii.state['buttons']
 
  # If Plus and Minus buttons pressed
  # together then rumble and quit.
  if (buttons - cwiid.BTN_PLUS - cwiid.BTN_MINUS == 0):
    print '\nClosing connection ...'
    wii.rumble = 1
    time.sleep(1)
    wii.rumble = 0
    exit(wii)
 
  # Check if other buttons are pressed by
  # doing a bitwise AND of the buttons number
  # and the predefined constant for that button.
  if (buttons & cwiid.BTN_LEFT):
    print 'Left pressed'
    time.sleep(button_delay)
 
  if(buttons & cwiid.BTN_RIGHT):
    print 'Right pressed'
    time.sleep(button_delay)
 
  if (buttons & cwiid.BTN_UP):
    print 'Up pressed'
    time.sleep(button_delay)
 
  if (buttons & cwiid.BTN_DOWN):
    print 'Down pressed'
    time.sleep(button_delay)
 
  if (buttons & cwiid.BTN_1):
    print 'Button 1 pressed'
    time.sleep(button_delay)
 
  if (buttons & cwiid.BTN_2):
    print 'Button 2 pressed'
    time.sleep(button_delay)
 
#  if (buttons & cwiid.BTN_A):
#    print 'Button A pressed'
#    time.sleep(button_delay)
 
  if (buttons & cwiid.BTN_B):
    print 'Button B pressed'
    time.sleep(button_delay)
 
  if (buttons & cwiid.BTN_HOME):
    print 'Home Button pressed'
    time.sleep(button_delay)
 
  if (buttons & cwiid.BTN_MINUS):
    print 'Minus Button pressed'
    time.sleep(button_delay)
 
  if (buttons & cwiid.BTN_PLUS):
    print 'Plus Button pressed'
    time.sleep(button_delay)

  if wii.state['acc'][2] > 150:
    print "acc get next exoplanet"
    lineNumber = 32 + (wii.state['acc'][2] - 150) * 3
    print "get line number " + str( lineNumber)
    if listLength < lineNumber:
      lineNumber = listLength - 10
    if lineNumber < 30:
      lineNumber = 35
    sendUART("exoplanet");

 # wii.rumble = (wii.state['acc'][0] < 126)
  if (buttons & cwiid.BTN_A):
    print "acc " + str(wii.state['acc'][0]) + " " + str(wii.state['acc'][1]) + " " + str(wii.state['acc'][2])  
    if wii.state['acc'][2] > 150:
      print "next exoplanet"
      lineNumber = lineNumber + 1
      print urlContent[lineNumber]
      sendUART("exoplanet");	
    if wii.state['acc'][1] > 150:
      print "former exoplanet"
      lineNumber = lineNumber - 1
      sendUART("exoplanet");
      #print urlContent[lineNumber]
    if wii.state['acc'][0] > 200:
      print "solar value"
      getSunActivity()
      sendUART("sunactivity");
    time.sleep(.5)

  if (buttons & cwiid.BTN_A):
    wii.led = (wii.state['led'] + 1) % 16
    time.sleep(button_delay)
