import sys
import serial
import time
import requests


from pynput.mouse import Button, Controller

mouse = Controller()

try:
  arduino = serial.Serial("COM3", timeout = 0.5, baudrate = 9600)
except:
  print("Please Check Port")

print("Press Ctrl-C to quit.")

def sendKey(k):
  r = requests.get("http://127.0.0.1:5000/presskey/" + k)
  return r.text

def releaseKey(k):
  r = requests.get("http://127.0.0.1:5000/releasekey/" + k)
  return r.text

W = 0x11
SPACE = 0X39

while True:
  inpt = str(arduino.readline())
  confInpt = inpt[2:len(inpt)-5]
  print(confInpt)
  if confInpt == "Walking: 0":
    sendKey("w")
  if confInpt == "Walking: 1":
    releaseKey("w")
  if confInpt == "Jump!":
    sendKey("SPACE")
    time.sleep(0.25)
    releaseKey("SPACE")
  if confInpt == "Destroy: 1":
    mouse.press(Button.left)
  if confInpt == "Destroy: 0":
    mouse.release(Button.left)
  if confInpt == "Place!":
    mouse.press(Button.right)
    mouse.release(Button.right)
  if confInpt != "":
    if confInpt[0] == "x":
      if confInpt[4] == "-":
        mouse.move(float(confInpt[5:len(confInpt)])*-10, 0)
      else:
        mouse.move(float(confInpt[4:len(confInpt)])*10,0)
    if confInpt[0] == "y":
      if confInpt[4] == "-":
        mouse.move(0, float(confInpt[5:len(confInpt)])*-10)
      else:
        mouse.move(0, float(confInpt[4:len(confInpt)])*10)