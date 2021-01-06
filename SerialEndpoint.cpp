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
#define CMD_SET_WATER_PUMP 0x0B
#define CMD_SET_NUTRIENT_PUMP 0x0C
#define CMD_SET_DOWNER_PUMP 0x0D
#define CMD_SET_MIXER_PUMP 0x0E



void SerialEndpointClass::sendAck()
{
  // First, command
  buffer[0] = CMD_ACK;
  uint8_t size = 1; 
  size = appendCrc((char*)buffer, size);
  slip.send((char*)buffer, size);
}

void SerialEndpointClass::sendNack()
{
  buffer[0] = CMD_NACK;
  uint8_t size = 1; 
  size = appendCrc((char*)buffer, size);
  slip.send((char*)buffer, size);
}


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
}

SerialEndpointClass::SerialEndpointClass()
{
  self = this;
  pairMode = false;
}

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



SerialEndpointClass SerialEndpoint;
