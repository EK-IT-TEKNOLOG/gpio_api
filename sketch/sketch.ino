#include <Arduino_RouterBridge.h>
#include <Wire.h>

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
  Bridge.provide("analog_write", analog_write_pin);
  Bridge.provide("analog_atten", set_atten);
  Bridge.provide("analog_write_res", set_write_res);
  Bridge.provide("init_i2c", init_i2c);
  Bridge.provide("start_write_i2c", start_write_i2c);
  Bridge.provide("end_write_i2c", end_write_i2c);
  Bridge.provide("byte_write_i2c", byte_write_i2c);
  Bridge.provide("start_read_i2c", start_read_i2c);
  Bridge.provide("byte_read_i2c", byte_read_i2c);

  Bridge.provide("init_uart", init_uart);
  Bridge.provide("avaiable_data_uart", avaiable_data_uart);
  Bridge.provide("read_uart", read_uart);
  Bridge.provide("write_uart", write_uart);
  Bridge.provide("deinit_uart", deinit_uart);
}

void loop() {}

void init_uart(int speed) {
  Serial1.begin(speed);
}

bool avaiable_data_uart() {
  return Serial1.available();
}

char read_uart() {
  return Serial1.read();
}

void write_uart(byte b) {
  Serial1.write(b);
  Serial1.flush();
}

void deinit_uart() {
  Serial1.end();
}

void init_i2c() {
  Wire.begin(); // I2C in UNO-style headers (D20, D21)
}

void start_write_i2c(int addr) {
  Wire.beginTransmission(addr);
}

void end_write_i2c(int addr) {
  Wire.endTransmission();
}

void byte_write_i2c(int b) {
  Wire.write(b);
}

void start_read_i2c(int addr, int buflen) {
  Wire.requestFrom(addr, buflen);
}

int byte_read_i2c() {
  if (Wire.available()) {
    return Wire.read();
  } else {
    return -1;
  }
}

int analog_read_pin(int pin) {
  return analogRead(pin);
}

void analog_write_pin(int pin, int value) {
  analogWrite(pin, value);
}
/*
int analog_read_pin(uint8_t pin) {
  return analogReadMilliVolts(pin);
}
*/

void set_atten(int bits) {
  analogReadResolution(bits);
}

void set_write_res(int bits) {
  analogWriteResolution(bits);
}

uint8_t configure_pin(int PIN_NO, int DIRECTION) {
  pinMode(PIN_NO, DIRECTION);
  // digitalWrite(PIN_NO, HIGH);
  // delay(500);
  // digitalWrite(PIN_NO, LOW);
  return 0;
}

uint8_t set_led_state(bool state, int pin) {
    // LOW state means LED is ON
    digitalWrite(pin, state ? LOW : HIGH);
    return 0;
}

uint8_t get_pin_state(int pin) {
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
