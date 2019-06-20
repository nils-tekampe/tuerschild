import epaper7in5
from machine import Pin, SPI
import framebuf
import network
import urequests
from time import sleep_ms
from images import willkommen stop 

REFRESH_INTERVAL_MS=1000

def do_connect():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect('alpha', 'halima@2003')
        while not wlan.isconnected():
            pass
    print("network config:", wlan.ifconfig())


def print_image(self, state):
    SCK = Pin(18)
    MISO = Pin(19)
    MOSI = Pin(23)
    DC = Pin(17)
    CS = Pin(5)
    RST = Pin(16)
    BUSY = Pin(4)

    W = 640
    H = 384
    x = 0
    y = 0
    BLACK = 0
    WHITE = 1

    spi = SPI(2, baudrate=20000000, polarity=0,
              phase=0, sck=SCK, miso=MISO, mosi=MOSI)

    print("starting program")
    e = epaper7in5.EPD(spi, CS, DC, RST, BUSY)
    e.init()

    print("preparing frame buffer")
    buf = bytearray(W * H // 8)
    fb = framebuf.FrameBuffer(buf, W, H, framebuf.MONO_HLSB)

    if (state == "welcome"):
        print('Printing Willkommen')
        bufImage = willkommen
        e.display_frame(bufImage)
    elif(state == "stop"):
        print('Printing Stop')
        bufImage = stop
        e.display_frame(bufImage)

    print("Print abgeschlossen")


do_connect()
state = ""

while (True):
    response = urequests.get('https://doorsign.konfidas.de/status.php')
    if (response.text != state):
        print("Received response: " + response.text)
        state = response.text
        print_image(response.text)
    sleep(REFRESH_INTERVAL_MS)
