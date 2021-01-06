#ifndef SERIAL_ENDPOINT_H
#define SERIAL_ENDPOINT_H

#include <Arduino.h>
#include <Mesh.h>
#include "StatusLeds.h"
#include "Slip.h"
#include "CRC.h"
#include "Storage.h"

#define SERIAL_BUFF_SIZE 254
#define LEN_INDEX 0
#define PLLEN_INDEX 5

#define DEFAULT_PAN 0x01

class SerialEndpointClass
{
private:
  Slip slip;
  uint8_t buffer[SERIAL_BUFF_SIZE];
  void sendCommand(uint8_t command);
  void sendCommandValue8(uint8_t command, uint8_t value); //for int and  bool values
  void sendCommandValue16(uint8_t command, uint16_t value); //for float and  uint16_t values

  uint8_t parseValue8(char * buff); //parse and get int or bool value 
  uint16_t parseValue16(char * buff); //parse and get uint16_t 

public:
  bool pairMode;
  SerialEndpointClass();
  void begin();
  void loop();
  void sendAck();
  void sendNack();

  // Master Device API Methods

  /* Get Sensor Value*/
  void getSensorValue(uint8_t sensorCommand);

  /* Get Pump State */
  void getPumpState(uint8_t pumpCommand);

  /* Set Pump State */
  void setPumpState(uint8_t pumpCommand, uint8_t state);
};

extern SerialEndpointClass SerialEndpoint;

#endif //SERIAL_ENDPOINT_H
