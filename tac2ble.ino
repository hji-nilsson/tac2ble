/*********************************************************************
 Project to add BLE support for retro joysticks.
 Project is based on the Adafruit Feather 32u4 Bluefruit LE module.
 
 Please support Adafruit and open-source hardware by purchasing
 products from Adafruit!
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"
// TODO: Use these defines instead of the hard coded values
// #include "HIDKeyboardCodes.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"

// Create the bluefruit object
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err)
{
  Serial.println(err);
  while (1);
}

/**************************************************************************
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
/**************************************************************************/
void setup(void)
{
  delay(500);

  Serial.begin(115200);
  Serial.println(F("TAC2 BLE converter initializing"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() )
    {
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Bluefruit Keyboard': "));
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Bluefruit Keyboard" )) )
  {
    error(F("Could not set device name?"));
  }

  /* Enable HID Service */
  Serial.println(F("Enable HID Service (including Keyboard): "));
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
      error(F("Could not enable Keyboard"));
    }
  }
  else
  {
    if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
      error(F("Could not enable Keyboard"));
    }
  }

  /* Add or remove service requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }

  Serial.println();

  pinMode(UP_PIN, INPUT);
  pinMode(DOWN_PIN, INPUT);
  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
  pinMode(FIRE_PIN, INPUT);
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  String tmp = "";
  static String buttonsPressed = "";
  tmp = readJoycon();

  if(!buttonsPressed.equals(tmp))
  {
    buttonsPressed = tmp;
    ble.println(buttonsPressed);
    ble.waitForOK();
    Serial.println(buttonsPressed);
  }
  delay(1);
}

String readJoycon(void)
{
  String buttonsPressed = "AT+BleKeyboardCode=00-00";
  
  if(UP)
  {
    buttonsPressed +="-52";
  }

  if(DOWN)
  {
    buttonsPressed +="-51";
  }

  if(LEFT)
  {
    buttonsPressed +="-50";
  }

  if(RIGHT)
  {
    buttonsPressed +="-4F";
  }

  if(FIRE)
  {
    buttonsPressed +="-28";
  }

  return buttonsPressed;
}
