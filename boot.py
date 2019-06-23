import machine
import network


def do_connect():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect('alpha', 'halima@2003')
        while not wlan.isconnected():
            pass
    print("network config :", wlan.ifconfig())

do_connect()
wdt = machine.WDT(timeout=WATCHDOG_INTERVAL) 
wdt.feed()

if (machine.reset_cause() == machine.DEEPSLEEP_RESET):
    print("just woke up from deep sleep ")