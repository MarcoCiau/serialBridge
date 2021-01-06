#include "SerialEndpoint.h"

static SerialEndpointClass *self;
static uint8_t recBuffer[SERIAL_BUFF_SIZE];

/* Serial Bridge Commands

  - Get Commands Request Format  [CMD][CRC_LOW][CRC_HIGH]
  - Get Commands Response Format [CMD][VALUE][CRC_LOW][CRC_HIGH]


  - Set Commands Request Format  [CMD][VALUE][CRC_LOW][CRC_HIGH]
  - Set Commands Response Format [CMD][VALUE][CRC_LOW][CRC_HIGH]
*/

//Connections and Errors Control Commands
#define CMD_NACK                      0x00
#define CMD_ACK                       0x01
#define CMD_BAD_CRC                   0xFE
#define CMD_ERROR                     0xFF

//Get Commands
#define CMD_GET_WATER_LEVEL 0x02
#define CMD_GET_NUTRIENT_LEVEL 0x03
#define CMD_GET_PH_DOWNER_LEVEL 0x04
#define CMD_GET_TDS 0x05
#define CMD_GET_PH 0x06

#define CMD_GET_WATER_PUMP_STATE 0x07
#define CMD_GET_NUTRIENT_PUMP_STATE 0x08
#define CMD_GET_PH_DOWNER_PUMP_STATE 0x09
#define CMD_GET_MIXER_PUMP_STATE 0x0A

//Set Commands
#define CMD_SET_WATER_PUMP_STATE 0x0B
#define CMD_SET_NUTRIENT_PUMP_STATE 0x0C
#define CMD_SET_DOWNER_PUMP_STATE 0x0D
#define CMD_SET_MIXER_PUMP_STATE 0x0E


static void attendSerial(char *data, uint8_t size)
{
  // Data format here:
  // [command][data][crc][crc]
  // [command][crc][crc]
  // Attend
  if(size < 3) return self->sendNack(); // bad data


  if (!checkCrc(data, size)) // bad crc
  {
    self->sendNack();
    return;
  }

  size = size - 2; //update buffer size (without CRC)

  uint8_t msgType = data[0];

  //TODO:Check for commands

  //Slave Device Commands Attends

  //Attend Sensor Readings
  if (self->attendGetSensorValueReq(msgType)) return;

  //Attend Get Relay
  if (self->attendGetPumpStateReq(msgType)) return;

  //Attend Set Relay
  if (self->attendSetPumpState(msgType, data))
  {
    self->sendConfirmation();
    return;
  } 
 
}

SerialEndpointClass::SerialEndpointClass()
{
  self = this;
  pairMode = false;
}

//Private Methods
void SerialEndpointClass::sendCommand(uint8_t command)
{
  buffer[0] = command;
  uint8_t size = 1;
  size = appendCrc((char*)buffer, size);
  slip.send((char*)buffer, size);
}

void SerialEndpointClass::sendCommandValue8(uint8_t command, uint8_t value) //for int and  bool values
{
  buffer[0] = command;
  buffer[1] = value;
  uint8_t size = 2;
  size = appendCrc((char*)buffer, size);
  this->send((char*)buffer, size);
}

void SerialEndpointClass::sendCommandValue16(uint8_t command, uint16_t value) //for float and  uint16_t values
{
  buffer[0] = command;
  buffer[1] = value & 0xFF; //value LOW Byte
  buffer[2] = (value >> 8) & 0xFF; //value HIGH Byte
  uint8_t size = 3;
  size = appendCrc((char*)buffer, size);// apend [crcLow][crcHigh]. New size = size + 2
  this->send((char*)buffer, size);
}

uint8_t SerialEndpointClass::parseValue8(char * buff) //parse and get int or bool value 
{
  return buffer[1];
}

uint16_t SerialEndpointClass::parseValue16(char * buff) //parse and get uint16_t 
{
  uint16_t val;
  val = buffer[1] & 0x00FF;
  val |= ((uint16_t)buffer[2] << 8) & 0xFF00;
  return val;
}

//Public Methods

void SerialEndpointClass::begin()
{
  Storage.begin();
  slip.begin(115200, attendSerial);
  loadPreferences();
  sendAck();  // Send ack for flushing any pending messages sent from the gateway before we were ready
}

void SerialEndpointClass::loop()
{
  slip.loop();
}

void SerialEndpointClass::sendAck()
{
  this->sendCommand(CMD_ACK);
}

void SerialEndpointClass::sendNack()
{
  this->sendCommand(CMD_NACK);
}


// Master Device API Methods

void SerialEndpointClass::getSensorValueReq(uint8_t sensorCommand)
{
  this->sendCommand(sendCommand);
}

void SerialEndpointClass::getPumpStateReq(uint8_t pumpCommand)
{
  this->sendCommand(pumpCommand);
}

void SerialEndpointClass::setPumpStateReq(uint8_t pumpCommand, uint8_t state)
{
  this->sendCommandValue8(pumpCommand, state);
}

// Slave Device API Methods

void SerialEndpointClass::sendConfirmation()
{
  this->sendNack();
}

// void SerialEndpointClass::getSensorValueRes(uint8_t sensorCommand, uint16_t value)
// {
//   this->sendCommandValue16(sensorCommand, value);
// }


// void SerialEndpointClass::getPumpStateRes(uint8_t pumpCommand, uint8_t state)
// {
//   this->sendCommandValue8(pumpCommand, state);
// }

bool SerialEndpointClass::attendGetSensorValueReq(uint8_t sensorCommand)
{
  bool status = false;
  uint16_t sensorVal = 0;
  if (sensorCommand == CMD_GET_WATER_LEVEL)
  {
    //TODO: get water level
    status = true;
  }
  else if (sensorCommand == CMD_GET_NUTRIENT_LEVEL)
  {
    //TODO: get nutrient level
    status = true;
  }
  else if (sensorCommand == CMD_GET_PH_DOWNER_LEVEL)
  {
    //TODO: get ph downer level
    status = true;
  }
  else if (sensorCommand == CMD_GET_TDS)
  {
    //TODO: get tds val
    status = true;
  }
  else if (sensorCommand == CMD_GET_PH)
  {
    //TODO: get ph val
    status = true;
  }
  /* Send sensor value */
  this->sendCommandValue16(sensorCommand, sensorVal);
  return status;
}

bool SerialEndpointClass::attendGetPumpStateReq(uint8_t pumpCommand)
{
  bool status = false;
  uint8_t pumpState = 0;
  if (pumpCommand == CMD_GET_WATER_PUMP_STATE)
  {
    //TODO: get water pump state
    status = true;
  }
   else if (pumpCommand == CMD_GET_NUTRIENT_PUMP_STATE)
  {
    //TODO: get nutrient pump state
    status = true;
  }
   else if (pumpCommand == CMD_GET_DOWNER_PUMP_STATE)
  {
    //TODO: get downer pump state
    status = true;
  }
   else if (pumpCommand == CMD_GET_MIXER_PUMP_STATE)
  {
    //TODO: get mixer pump state
    status = true;
  }
  this->sendCommand8(pumpCommand, pumpState);
  return status;  
}

bool SerialEndpointClass::attendSetPumpStateReq(uint8_t pumpCommand, char * buffData)
{
  bool status = false;
  uint8_t pumpState = buffData[1];
  if (pumpCommand == CMD_SET_WATER_PUMP_STATE)
  {
    //TODO: set water pump state
    status = true;
  }
   else if (pumpCommand == CMD_SET_NUTRIENT_PUMP_STATE)
  {
    //TODO: set nutrient pump state
    status = true;
  }
   else if (pumpCommand == CMD_SET_DOWNER_PUMP_STATE)
  {
    //TODO: set downer pump state
    status = true;
  }
   else if (pumpCommand == CMD_SET_MIXER_PUMP_STATE)
  {
    //TODO: set mixer pump state
    status = true;
  }

  return status;
}


SerialEndpointClass SerialEndpoint;
