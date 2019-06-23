import epaper7in5
import machine
# from machine import Pin, SPI, deepsleep, lightsleep, WDT
import framebuf
import network
import urequests
from time import sleep_ms
from welcome import welcome
from stop import stop
import time
from boot import wdt, do_connect

WATCHDOG_INTERVAL = 600000
REFRESH_INTERVAL_MS = 1000
SLEEP_TIME = 600000
SCK = machine.Pin(18)
MISO = machine.Pin(19)
MOSI = machine.Pin(23)
DC = machine.Pin(17)
CS = machine.Pin(5)
RST = machine.Pin(16)
BUSY = machine.Pin(4)
W = 640
H = 384
x = 0
y = 0
BLACK = 0
WHITE = 1


def print_image(state):
    print("starting to print image for state: "+state)
    spi = machine.SPI(2, baudrate=20000000, polarity=0,
              phase=0, sck=SCK, miso=MISO, mosi=MOSI)

    e = epaper7in5.EPD(spi, CS, DC, RST, BUSY)
    e.init()

    if (state == "welcome"):
        print('Printing Willkommen')
        e.display_frame(welcome)
    elif(state == "stop"):
        print('Printing Stop')
        e.display_frame(stop)

    print("Print abgeschlossen")


while (True):
    start = time.time()
    response = urequests.get('https://doorsign.konfidas.de/status.php')
    print("response from webservice: "+response.text)
    print_image(response.text)
    response = urequests.get('https://doorsign.konfidas.de/confirmUpdate.php')
    end = time.time()
    print("time consumption of one cycle: "+str(end - start))
    wdt.feed()
    print("going to sleep now ")
    machine.deepsleep(SLEEP_TIME)