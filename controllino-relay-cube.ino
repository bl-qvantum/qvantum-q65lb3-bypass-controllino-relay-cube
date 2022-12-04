#include <Controllino.h>
#include "BlinkyBus.h"
#define BAUD_RATE  19200
#define commLEDPin    -1

#define BLINKYBUSBUFSIZE  3
union BlinkyBusUnion
{
  struct
  {
    int16_t state;
    int16_t watchdog;
    int16_t relayState;
  };
  int16_t buffer[BLINKYBUSBUFSIZE];
} bb;
BlinkyBus blinkyBus(bb.buffer, BLINKYBUSBUFSIZE, Serial, commLEDPin);

void setup() 
{
  bb.state = 1;
  pinMode(CONTROLLINO_R0, OUTPUT);
  pinMode(CONTROLLINO_R1, OUTPUT);
  pinMode(CONTROLLINO_R2, OUTPUT);
  pinMode(CONTROLLINO_R3, OUTPUT);
  pinMode(CONTROLLINO_R4, OUTPUT);
  pinMode(CONTROLLINO_R5, OUTPUT);
  bb.relayState = 0;
  setRelays();
  Serial.begin(BAUD_RATE);
  blinkyBus.start();
}

void loop() 
{
  int ipoll = blinkyBus.poll();
  if (ipoll > 0)
  {
    bb.watchdog = bb.watchdog + 1;
    if (bb.watchdog > 32760) bb.watchdog = 0;
  }
  if (ipoll == 2) 
  {
    if (blinkyBus.getLastWriteAddress() == 2) setRelays();
  }
}

void setRelays()
{
  digitalWrite(CONTROLLINO_R2, 0);    
  digitalWrite(CONTROLLINO_R3, 0);
  digitalWrite(CONTROLLINO_R4, 0);
  digitalWrite(CONTROLLINO_R5, 0);
      
  if (bb.relayState == 0)
  {
    digitalWrite(CONTROLLINO_R0, 1);    
    digitalWrite(CONTROLLINO_R1, 1);
    digitalWrite(CONTROLLINO_R2, 1);
    digitalWrite(CONTROLLINO_R5, 1);
  }    
  if (bb.relayState == 1)
  {
    digitalWrite(CONTROLLINO_R0, 0);    
    digitalWrite(CONTROLLINO_R1, 0);
    digitalWrite(CONTROLLINO_R3, 1);
    digitalWrite(CONTROLLINO_R4, 0);
  }    
}
