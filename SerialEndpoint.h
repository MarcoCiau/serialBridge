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
  void getSensorValueReq(uint8_t sensorCommand);

  /* Get Pump State */
  void getPumpStateReq(uint8_t pumpCommand);

  /* Set Pump State */
  void setPumpStateReq(uint8_t pumpCommand, uint8_t state);

  // Slave Device API Methods

  /* Send process confirmation */
  void sendConfirmation();

  /* Send Sensor Value */
  // void getSensorValueRes(uint8_t sensorCommand, uint16_t value);

  /* Send Pump State */
  // void getPumpStateRes(uint8_t pumpCommand, uint8_t state);

  /* Attend Get Sensor commands */
  bool attendGetSensorValueReq(uint8_t sensorCommand);

  /* Attend Get PUMP State commands */
  bool attendGetPumpStateReq(uint8_t pumpCommand);

  /* Attend Set PUMP State commands */
  bool attendSetPumpStateReq(uint8_t pumpCommand, char * buffData);


};

extern SerialEndpointClass SerialEndpoint;

#endif //SERIAL_ENDPOINT_H
