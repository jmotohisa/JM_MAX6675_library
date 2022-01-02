/*!
 * @file JM_MAX6675.h
 *
 * JM's MAX6675 thermocouple driver based on Adafruit MAX31855 library
 * Original follows
 * This is the documentation for Adafruit's MAX31855 thermocouple breakout
 * driver for the Arduino platform.  It is designed specifically to work with
 * the Adafruit MAX31855 breakout: https://www.adafruit.com/products/269
 *
 * These sensors use SPI to communicate, 3 pins are required
 * to interface with the breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * BSD license, all text above must be included in any redistribution.
 *
 */

#ifndef ADAFRUIT_MAX6675_H
#define ADAFRUIT_MAX6675_H

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_SPIDevice.h>

/**************************************************************************/
/*!
    @brief  Sensor driver for the Adafruit MAX6675 thermocouple breakout.
*/
/**************************************************************************/
class JM_MAX6675 {
public:
  JM_MAX6675(int8_t _sclk, int8_t _cs, int8_t _miso);
  JM_MAX6675(int8_t _cs, SPIClass *_spi = &SPI);

  bool begin(void);
  double readInternal(void);
  double readCelsius(void);
  double readFahrenheit(void);
  uint8_t readError();

private:
  Adafruit_SPIDevice spi_dev;
  bool initialized = false;

  uint16_t spiread16(void);
};

#endif
