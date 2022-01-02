/*!
 * @file JM_MAX6675.cpp
 *
 * @mainpage MAX6675 Thermocouple Breakout Driver (based on Adafruit MAX31855 library)
 *
 * @section intro_sec Introduction : original follows
 *
 * This is the documentation for Adafruit's MAX31855 thermocouple breakout
 * driver for the Arduino platform.  It is designed specifically to work with
 * the Adafruit MAX31855 breakout: https://www.adafruit.com/products/269
 *
 * This breakout uses SPI to communicate, 3 pins are required
 * to interface with the breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section dependencies Dependencies
 *
 * This library depends on <a
 * href="https://github.com/adafruit/Adafruit_BusIO"> Adafruit_BusIO</a> being
 * present on your system. Please make sure you have installed the latest
 * version before using this library.
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text above must be included in any redistribution.
 *
 */

#include "JM_MAX6675.h"
#ifdef __AVR
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif

#include <stdlib.h>

/**************************************************************************/
/*!
    @brief  Instantiates a new JM_MAX6675 class using software SPI.

    @param _sclk The pin to use for SPI Serial Clock.
    @param _cs The pin to use for SPI Chip Select.
    @param _miso The pin to use for SPI Master In Slave Out.
*/
/**************************************************************************/
JM_MAX6675::JM_MAX6675(int8_t _sclk, int8_t _cs, int8_t _miso)
    : spi_dev(_cs, _sclk, _miso, -1, 1000000) {}

/**************************************************************************/
/*!
    @brief  Instantiates a new JM_MAX6675 class using hardware SPI.

    @param _cs The pin to use for SPI Chip Select.
    @param _spi which spi buss to use.
*/
/**************************************************************************/
JM_MAX6675::JM_MAX6675(int8_t _cs, SPIClass *_spi)
    : spi_dev(_cs, 1000000, SPI_BITORDER_MSBFIRST, SPI_MODE0, _spi) {}

/**************************************************************************/
/*!
    @brief  Setup the HW

    @return True if the device was successfully initialized, otherwise false.
*/
/**************************************************************************/
bool JM_MAX6675::begin(void) {
  initialized = spi_dev.begin();

  return initialized;
}

/**************************************************************************/
/*!
    @brief  Read the thermocouple temperature.

    @return The thermocouple temperature in degrees Celsius.
*/
/**************************************************************************/
double JM_MAX6675::readCelsius(void) {

  int32_t v;

  v = spiread16();

  if (v & 0x4) {
    // uh oh, no thermocouple attached!
    return NAN;
    // return -100;
  }

  v >>= 3;

  return v * 0.25;
}

/**************************************************************************/
/*!
    @brief  Read the error state.

    @return The error state.
*/
/**************************************************************************/
uint8_t JM_MAX6675::readError() { return spiread16() & 0x4; }

/**************************************************************************/
/*!
    @brief  Read the thermocouple temperature.

    @return The thermocouple temperature in degrees Fahrenheit.
*/
/**************************************************************************/
double JM_MAX6675::readFahrenheit(void) {
  float f = readCelsius();
  f *= 9.0;
  f /= 5.0;
  f += 32;
  return f;
}

/**************************************************************************/
/*!
    @brief  Read 4 bytes (32 bits) from breakout over SPI.

    @return The raw 32 bit value read.
*/
/**************************************************************************/
uint16_t JM_MAX6675::spiread16(void) {
  uint16_t d = 0;
  uint8_t buf[2];

  // backcompatibility!
  if (!initialized) {
    begin();
  }

  spi_dev.read(buf, 4);

  d = buf[0];
  d <<= 8;
  d |= buf[1];

  // Serial.println(d, HEX);

  return d;
}
