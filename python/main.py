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

def write_i2c(addr, buf, end_comm = True):
    #TODO Test if buf is needed to convert to bytes
    #TODO Test if buf is list, if not then raise error
    try:
        buf=eval(buf)
    except:
        return -1
    log(f'Writing {buf} to device addr: {addr}')
    Bridge.call("start_write_i2c", int(addr))
    for b in buf:
        Bridge.call("byte_write_i2c", int(b))
    if end_comm:
        Bridge.call("end_write_i2c")

def read_i2c(addr, buflen):
    Bridge.call("start_read_i2c", int(addr), int(buflen))
    buf = []
    buflen = int(buflen)
    for i in range(buflen):
        buf.append(Bridge.call("byte_read_i2c"))
    return buf

def scan_i2c():
    res = []
    for i in range(1,127):
        #res.append(Bridge.call("scan_i2c", int(i)))
        if Bridge.call("scan_i2c", int(i)) == 0:
            res.append(i)
    return res

def pin_status(pin, in_pin = False):
    if in_pin:
        res = Bridge.call("get_pin_state", int(pin))
        log(f'[+] GOT {res} from pin state on {pin}')
        return eval(res.text)
    else:
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
    res = Bridge.call("test")
    #print('PRINTET HEST '+str(hest)+' '+str(type(hest)))
    return 'HEST '+str(res)+' '+str(type(res))

def init_uart(speed):
    Bridge.call("init_uart", int(speed))
    return 'OK'

def deinit_uart(speed):
    Bridge.call("deinit_uart")

def avaiable_data_uart():
    return Bridge.call('avaiable_data_uart')

def read_uart():
    return Bridge.call('read_uart')

def write_uart(bytes):
    try:
        bytes = eval(bytes)
    except:
        raise 'Error converting'
    for b in bytes:
        Bridge.call('write_uart', int(b))

def init_spi(cs_pin):
    log(f'Initiaing SPI with CS on {cs_pin}')
    res = Bridge.call('init_spi', int(cs_pin))
    log(f'Initiating status: {res}')
    return res

def tx_rx_spi(cs_pin, data):
    data = eval(data)
    res = []
    log(f'Sending {data} on SPI with CS {cs_pin}')
    for i in range(len(data)-1):
        r = Bridge.call('tx_rx_spi', int(cs_pin), int(data[i]), True)
        res.append(r)
    r = Bridge.call('tx_rx_spi', int(cs_pin), int(data[-1]), False)
    res.append(r)
    log(f'Got this back: {res}')
    return res
        

def init_dht(pin_no, dht_type):
    Bridge.call('init_dht', int(pin_no), int(dht_type))

def dht_read_temp(pin_no):
    Bridge.call('dht_read_temp', int(pin_no))

def dht_read_hum(pin_no):
    Bridge.call('dht_read_hum', int(pin_no))

# Initialize WebUI
ui = WebUI()
ui.expose_api("GET", "/configure_pin/{pin_no}/{direction}", configure_pin)
ui.expose_api("GET", "/pin_on/{pin}", pin_on)
ui.expose_api("GET", "/pin_off/{pin}", pin_off)
ui.expose_api("GET", "/pin_status/{pin}/{in_pin}", pin_status)

ui.expose_api("GET", "/analog_read/{pin}", analog_read)
ui.expose_api("GET", "/analog_write/{pin}/{value}", analog_write)
ui.expose_api("GET", "/analog_atten/{bits}", analog_atten)
ui.expose_api("GET", "/analog_write_res/{bits}", analog_write_res)

ui.expose_api("GET", "/init_i2c", init_i2c)
ui.expose_api("GET", "/write_i2c/{addr}/{buf}/{end_comm}", write_i2c)
ui.expose_api("GET", "/read_i2c/{addr}/{buflen}", read_i2c)
ui.expose_api("GET", "/scan_i2c", scan_i2c)

ui.expose_api("GET", "/init_uart/{speed}", init_uart)
ui.expose_api("GET", '/avaiable_data_uart', avaiable_data_uart)
ui.expose_api("GET", "/read_uart", read_uart)
ui.expose_api("GET", "/write_uart/{bytes}", write_uart)
ui.expose_api("GET", "/deinit_uart", deinit_uart)

ui.expose_api("GET", "/init_spi/{cs_pin}", init_spi)
ui.expose_api("GET", "/tx_rx_spi/{cs_pin}/{data}", tx_rx_spi)

ui.expose_api("GET", "/init_dht/{pin_no}/{dht_type}", init_dht)
ui.expose_api('GET', '/dht_read_temp/{pin_no}', dht_read_temp)
ui.expose_api('GET', '/dht_read_hum/{pin_no}', dht_read_hum)

ui.expose_api("GET", "/test/{hest}", test)

# Start the application
App.run()
