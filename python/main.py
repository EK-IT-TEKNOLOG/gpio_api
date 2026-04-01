from arduino.app_utils import App
from arduino.app_bricks.web_ui import WebUI
from arduino.app_utils import *

led_is_on = False
LOG=True

port_state = {}

def log(msg):
    if LOG:
        print('[+]',msg)

def get_led_status():
    """Get current LED status for API."""
    return {
        "led_is_on": led_is_on,
        "status_text": "LED IS ON" if led_is_on else "LED IS OFF"
    }

def toggle_led_state(pin):
    """Toggle the LED state when receiving socket message."""
    global led_is_on
    led_is_on = not led_is_on

    # Call a function in the sketch, using the Bridge helper library, to control the state of the LED connected to the microcontroller.
    # This performs a RPC call and allows the Python code and the Sketch code to communicate.
    Bridge.call("set_led_state", led_is_on, int(pin))
    print('[+] Done calling to set led to '+str(led_is_on)+' on pin '+str(pin))

def pin_on(pin):
    log('Setting on')
    Bridge.call("set_led_state", True, int(pin))
    port_state[pin] = True 
    #log(port_state)

def pin_off(pin):
    log('Setting off')
    Bridge.call("set_led_state", False, int(pin))
    port_state[pin] = False

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
ui.expose_api("GET", "/hello", lambda: {"message": "Hello, world!"})
ui.expose_api("GET", "/status", get_led_status)
ui.expose_api("GET", "/configure_pin/{pin_no}/{direction}", configure_pin)
ui.expose_api("GET", "/toggle_led/{pin}", toggle_led_state)
ui.expose_api("GET", "/pin_on/{pin}", pin_on)
ui.expose_api("GET", "/pin_off/{pin}", pin_off)
ui.expose_api("GET", "/pin_status/{pin}", pin_status)

ui.expose_api("GET", "/test/{hest}", test)

# Start the application
App.run()

'''
import time

from arduino.app_utils import App

print("Hello world!")


def loop():
    """This function is called repeatedly by the App framework."""
    # You can replace this with any code you want your App to run repeatedly.
    time.sleep(10)


# See: https://docs.arduino.cc/software/app-lab/tutorials/getting-started/#app-run
App.run(user_loop=loop)
'''
