from arduino.app_utils import App
from arduino.app_bricks.web_ui import WebUI
from arduino.app_utils import *

LOG=True

port_state = {}

def log(msg):
    if LOG:
        print('[+]',msg)

def pin_on(pin):
    log('Setting on')
    Bridge.call("set_led_state", True, int(pin))
    port_state[pin] = True 
    #log(port_state)

def pin_off(pin):
    log('Setting off')
    Bridge.call("set_led_state", False, int(pin))
    port_state[pin] = False

def analog_read(pin):
    return Bridge.call("analog_read", int(pin))

def analog_write(pin, value):
    return Bridge.call("analog_write", int(pin), int(value))

def analog_atten(bits):
    return Bridge.call("analog_atten", int(bits))

def analog_write_res(bits):
    return Bridge.call("analog_write_res", int(bits))

def init_i2c():
    return Bridge.call("init_i2c")

def write_i2c(addr, buf):
    #TODO Test if buf is needed to convert to bytes
    #TODO Test if buf is list, if not then raise error
    try:
        buf=eval(buf)
    except:
        return -1
    Bridge.call("start_write_i2c", int(addr))
    for b in buf:
        Bridge.call("byte_write_i2c", int(b))
    Bridge.call("end_write_i2c")

def read_i2c(addr, buflen):
    Bridge.call("start_read_i2c", int(addr), int(buflen))
    buf = []
    for i in range(buflen):
        buf.append(Bridge.call("byte_read_i2c"))
    return buf

def pin_status(pin):
    #res = Bridge.call("get_pin_state", int(pin))
    #log(f'[+] GOT {res} from pin state on {pin}')
    if pin in port_state:
        return port_state[pin]
    else:
        return False

#input = 0
#output = 1
#input_pullup = 2
def configure_pin(pin_no, direction):
    #TODO: Error handling
    a = Bridge.call("configure_pin", int(pin_no), int(direction))
    port_state[pin_no] = False
    #print('[+] RES: '+str(a))
    return a

def test(hest):
    res = Bridge.call("get_pin_number_from_name")
    #print('PRINTET HEST '+str(hest)+' '+str(type(hest)))
    return 'HEST '+str(res)+' '+str(type(res))

# Initialize WebUI
ui = WebUI()
ui.expose_api("GET", "/configure_pin/{pin_no}/{direction}", configure_pin)
ui.expose_api("GET", "/pin_on/{pin}", pin_on)
ui.expose_api("GET", "/pin_off/{pin}", pin_off)
ui.expose_api("GET", "/pin_status/{pin}", pin_status)
ui.expose_api("GET", "/analog_read/{pin}", analog_read)
ui.expose_api("GET", "/analog_write/{pin}/{value}", analog_write)
ui.expose_api("GET", "/analog_atten/{bits}", analog_atten)
ui.expose_api("GET", "/analog_write_res/{bits}", analog_write_res)
ui.expose_api("GET", "/init_i2c", init_i2c)
ui.expose_api("GET", "/write_i2c/{addr}/{buf}", write_i2c)
ui.expose_api("GET", "/read_i2c/{addr}/{buflen}", read_i2c)

ui.expose_api("GET", "/test/{hest}", test)

# Start the application
App.run()
