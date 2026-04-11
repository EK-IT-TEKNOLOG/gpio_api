#include <Arduino_RouterBridge.h>
#include <Wire.h>
#include <SPI.h>
#include "DHT.h"
#include <Adafruit_NeoPixel.h>

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
  Bridge.provide("scan_i2c", scan_i2c);
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

  Bridge.provide("init_spi", init_spi);
  Bridge.provide("tx_rx_spi", tx_rx_spi);

  Bridge.provide("init_neopixel", init_neopixel);
  Bridge.provide("set_pixel_color", set_pixel_color);
  Bridge.provide("show_colors", show_colors);
  Bridge.provide("clear_colors", clear_colors);
  
  Bridge.provide("test", test);
}

void loop() {}

//HashType<int,DHT> hashRawArray[HASH_SIZE]; 
//HashMap<int,DHT> dhtMap = HashMap<int,DHT>( hashRawArray , HASH_SIZE ); 
//Hashtable<int, DHT> dhtMap;

void* dht;
Adafruit_NeoPixel* WS2812B;

//DHT dht = dht11(1,11);

void init_neopixel(int num_pixels, int led_pin) {
  WS2812B = new Adafruit_NeoPixel(num_pixels, led_pin, NEO_GRB+NEO_KHZ800);
  WS2812B->begin();
  //WS2812B.clear();
}

void set_pixel_color(int pixel, int r, int g, int b) {
  WS2812B->setPixelColor(pixel, WS2812B->Color(r, g, b));
}

void show_colors() {
  WS2812B->show();
}

void clear_colors() {
  WS2812B->clear();
  WS2812B->show();
}
/*
* Define types of sensors. *
static const uint8_t DHT11{11};  **< DHT TYPE 11
static const uint8_t DHT12{12};  **< DHY TYPE 12
static const uint8_t DHT21{21};  **< DHT TYPE 21 
static const uint8_t DHT22{22};  **< DHT TYPE 22 
static const uint8_t AM2301{21}; **< AM2301 *
*/

void init_dht(int pin_no, int dht_type) {
  DHT d(pin_no, dht_type);
  d.begin();
  dht = &d;
  //dhtMap[0](pin_no, dht);
  //DHT d = dhtMap.getElement(pin_no);
  //dhtMap.getValueOf(pin_no).begin();
}

float dht_read_temp(int pin_no) {
  DHT d = *(DHT*) dht;
  return d.readTemperature();
}

float dht_read_hum(int pin_no) {
  return (*(DHT*) dht).readHumidity();
}

bool init_spi(int cs_pin) {
  pinMode(cs_pin, OUTPUT);
  digitalWrite(cs_pin, HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  return true;
}

int tx_rx_spi(int cs_pin, int b, bool cont) {
  digitalWrite(cs_pin, LOW);
  byte res = SPI.transfer((byte)b);
  if (!cont)
    digitalWrite(cs_pin, HIGH);
  return (int)res;
}

void init_uart(int speed) {
  Serial.begin(speed);
  Serial.flush();
}

bool avaiable_data_uart() {
  return Serial.available();
}

char read_uart() {
  return Serial.read();
}

void write_uart(byte b) {
  Serial.write(b);
  Serial.flush();
}

void deinit_uart() {
  Serial.end();
}

void init_i2c() {
  Wire.begin(); // I2C in UNO-style headers (D20, D21)
}

int scan_i2c(int a) {
  byte error;

  Wire.beginTransmission(a);
  error = Wire.endTransmission();
  return error;
  /*
  if (error == 0) {
    return 1;
  }
  else {
    return 0;
  }
  */
}

void start_write_i2c(int addr) {
  Wire.beginTransmission(addr);
}

byte end_write_i2c() {
  return Wire.endTransmission();
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

int test() {
  /*
  WS2812B = new Adafruit_NeoPixel(2,4, NEO_GRB + NEO_KHZ800);
  WS2812B->begin();
  delay(1000);
  WS2812B->setPixelColor(0, WS2812B->Color(0,0,255));
  delay(1000);
  WS2812B->show();
  delay(3000);
  WS2812B->clear();
  delay(1000);
*/
  Adafruit_NeoPixel WS2812B(2,4, NEO_GRB + NEO_KHZ800);
  WS2812B.begin();
  delay(1000);
  WS2812B.setPixelColor(0, WS2812B.Color(0,0,255));
  delay(1000);
  WS2812B.show();
  delay(3000);
  WS2812B.clear();
  delay(1000);
  return 0;
}
