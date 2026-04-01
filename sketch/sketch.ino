#include <Arduino_RouterBridge.h>

void setup() {
  // put your setup code here, to run once:
    //pinMode(LED_BUILTIN, OUTPUT);
    // Start with the LED OFF (HIGH state of the PIN)
    //digitalWrite(LED_BUILTIN, HIGH);

    Bridge.begin();
    Bridge.provide("set_led_state", set_led_state);
    Bridge.provide("get_pin_state", get_pin_state);
    Bridge.provide("configure_pin", configure_pin);
    Bridge.provide("get_pin_number_from_name", get_pin_number);
    Bridge.provide("analog_read", analog_read_pin);
    Bridge.provide("analog_atten", set_atten);
}

void loop() {}

int analog_read_pin(uint8_t pin) {
  return analogRead(pin);
}

/*
int analog_read_pin(uint8_t pin) {
  return analogReadMilliVolts(pin);
}
*/

void set_atten(int bits) {
  analogReadResolution(bits);
}

uint8_t configure_pin(uint8_t PIN_NO, uint8_t DIRECTION) {
  pinMode(PIN_NO, DIRECTION);
  // digitalWrite(PIN_NO, HIGH);
  // delay(500);
  // digitalWrite(PIN_NO, LOW);
  return 0;
}

uint8_t set_led_state(bool state, uint8_t pin) {
    // LOW state means LED is ON
    digitalWrite(pin, state ? LOW : HIGH);
    return 0;
}

uint8_t get_pin_state(uint8_t pin) {
  return digitalRead(pin);
  //bool value = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
  //return value;
  /*
  if (PIND & (1 << 7)) {
    // HIGH
  } else {
    // LOW
  }
  */
  //TODO: Read this and figure out a way to do this: https://forum.arduino.cc/t/reading-a-port-on-an-stm32-bluepill-like-pinx-on-an-avr/617960
}

uint8_t get_pin_number() {
  return A0;
}
