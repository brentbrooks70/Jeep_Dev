#include <Arduino.h>

/*  ---Define Relay States--- */
#define RLY_ON HIGH
#define RLY_OFF LOW

/*  ---Define Relay Pins--- */
#define SPOTL_PIN 32
#define SPOTR_PIN 33
#define FOGFL_PIN 26
#define FOGFR_PIN 25
#define FOGBL_PIN 14
#define FOGBR_PIN 15
#define NUM_LIGHTS 6
//#define NUM_FOGS 4
//#define NUM_SPOTS 2


class Light
{
//  private:

  public:
    uint8_t id;
    uint8_t pin;
    uint8_t type;               //1=Spot, 2=SpotMM, 3=Fog, 4=FogMM, 5=Bar, 6=BarMM
    uint8_t numbModes;          //How many built-in modes (i.e. 6 for FogMM or 1 for Spot)not including custom overrides (i.e. Hazzards, Turn Signals, Reverse, etc.)
    uint8_t curMode = 0;        //0=Off, [FogMM: 1=White, 2=Amber, 3=AmbWhi, 4=WhiStrobe, 5=AmbStrobe, 6=AmbWhiStrobe], [Spot: 1=On]
    uint8_t prevMode = 0;       //To store curMode during off (until timed reset) or Overides, maybe even while cycling 
    uint8_t defMode = 1;        //Default mode in case you want a non "1" mode to be power on default
    int8_t pendingMode = -1;    //Next mode to run -1 = no change
    uint32_t minOff;            //Minimum time off to change mode
    uint16_t maxOff;            //Maximum time off to change mode before reset to mode 1
    uint32_t minOn;             //Minimum time on to qualify as a "cycle"
    bool isCycling = false;     //Is the current light in the act of cycling between modes
    uint8_t pendingCycles = 0;  //How modes are left to cycle through
    unsigned long lastOn = 0;   //millis() of last time it was turned off, maybe use NTP | RTC and timestamp
    unsigned long lastOff = 0;  //millis() of last time it was turned on, maybe use NTP | RTC and timestamp
    unsigned long nextOn = 0;   //millis() or RTC for next on cycle, used for cycling and maybe overrides only
    unsigned long nextOff = 0;  //millis() or RTC for next off cycle, used for cycling and maybe overrides only
    
    bool checkReset();          //This will test if the maxOff time has elapsed = true
    bool checkTimes();          //This should return true if the appropriate minOff or minOn time has elaped
    uint8_t cycleMode();
    uint8_t turnOff();
    uint8_t turnOn();
    void srvRelay();
    void srvMain();

};

bool Light::checkReset()        //This will test if the maxOff time has elapsed = true
{
  if (this->lastOn > 0)
  {
    Serial.print("Check Reset: ");
    if ((this->lastOn + this->maxOff) < millis())
    {
      Serial.println("TRUE");
      return true;
    }
    else
    {
      Serial.println("FALSE");
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool Light::checkTimes()        //This should return true if the appropriate minOff or minOn time has elaped
{
  Serial.print("Check Times: ");
  if (this->lastOff == 0 && this->lastOn == 0)    //SHOULD ONLY BE POSSIBLE IF JUST STARTED AND HAD NOT BEEN ON YET & never after EEPROM/Preferences
  {
    Serial.println("Passed with TWO OFFS!!");
    return true;
  }
  else if (this->lastOff > 0 && this->lastOn == 0)
  {
    if ( (this->lastOff + this->minOn) < millis() )
    {
      Serial.println("Passed minOn");
      return true;
    }
    else
    {
      Serial.println("Failed minOn");
      return false;
    }
  }
  else if (this->lastOn > 0 && this->lastOff == 0)
  {
    if ((this->lastOn + this->minOff) < millis())
    {
      Serial.println("Passed minOff");
      return true;
    }
    else
    {
      Serial.println("Failed minOff");
      return false;
    }
  }
  else
  {
    Serial.println("******FAILED with TWO ONS!!");
    return false;
  }
//  Serial.println("******FAILED BY FALLING THROUGH TO BOTTOM!!");
//  return false;
}

uint8_t Light::cycleMode()
{
  if (this->curMode)  //If the light is on, turn it off first
  {
    digitalWrite(this->pin, RLY_OFF);
    this->lastOn = millis();
    delay(this->minOff);      ////BLOCKING-NEED TO REMOVE-use nextOn and test for it?!?!
  }  //If on last mode, wrap back to begining (0 as it will get incremented to 1 below)
  if (this->curMode == this->numbModes) {this->curMode=0;}
  digitalWrite(this->pin, RLY_ON);  //Turn on next mode, increment mode and return
  this->lastOff = millis();
  this->curMode++;
  return this->curMode;
}

uint8_t Light::turnOff()
{
  Serial.print("Turning off: ");
  Serial.println(this->id);
//  if (checkTimes() == false)
//  {
//      Serial.println(" - Not time yet...");
//      return this->curMode;
//  }
  digitalWrite(this->pin, RLY_OFF);
  this->lastOn = millis();
  this->lastOff = 0;
    if (isCycling == false)
  {
    this->curMode = 0;
  }Serial.println("Done");
  return this->curMode;
}

uint8_t Light::turnOn()
{
  Serial.print("Turning on: ");
  Serial.println(this->id);
//  if (checkTimes() == false)
//  {
//    Serial.println("Not time yet...");
//    return this->curMode;
//  }
  digitalWrite(this->pin, RLY_ON);
  this->lastOff = millis();
  this->lastOn = 0;
  Serial.println("Done");
  this->curMode++;
  return this->curMode;
}

void Light::srvRelay()
{
  if (this->nextOn > 0 && this->checkTimes() == true) 
  {
    this->turnOn();
    this->pendingCycles--;
    if (this->curMode != this->pendingMode)
    {
      this->nextOff = this->lastOff + this->minOn;
    }
    else if (this->pendingCycles ==0)
    {
      this->isCycling = false;
    }
  }
  if (this->nextOff > 0 && this->checkTimes() == true) 
  {
    this->turnOff();
    this->nextOn = this->lastOn + this->minOff;
  }
}

void Light::srvMain()
{
  if (isCycling == true)
  {
    Serial.print("Cycling: ");
    Serial.print(this->id);
    this->srvRelay();
    return;
  }
  if (this->pendingMode == -1)    //Nothing to do here...except
  {
    if (this->curMode == 0 && checkReset())   //if off and been off long enoughto reset mode
    {
      Serial.println("Mode timed out & reset to 1");
      this->prevMode = this->defMode;
    }
  }
  else if (this->pendingMode == 0)     //Turn OFF
  {
    Serial.println("Case 0");
    this->prevMode = this->curMode;
    if (checkTimes())
    {
      this->turnOff();
      this->pendingMode = -1;
    }
  }
  else
  {
    this->isCycling = true;
    Serial.print("Light: ");
    Serial.print(this->id);
    Serial.print(" is cycling ");
    if (this->curMode < this->pendingMode)
    {
      
      this->pendingCycles = this->pendingMode - this->curMode;
    }
    else
    {
      this->pendingCycles = (this->numbModes - this->curMode) + (this->numbModes - this->pendingMode);
    }
    Serial.print(this->pendingCycles);
    Serial.println(" cycles.");
    this->nextOn = this->lastOn + this->minOff;
  }
}

class FogMM : public Light
{
  public:
    FogMM(uint8_t id, uint8_t pin, uint8_t numbModes, uint8_t defMode);
};

FogMM::FogMM(uint8_t id, uint8_t pin, uint8_t numbModes, uint8_t defMode) 
{
  this->numbModes = numbModes;
  this->id = id;
  this->pin = pin;
  this->defMode = defMode;
  this->minOn = 90;
  this->minOff = 30;
  this->maxOff = 3300;
  pinMode(pin, OUTPUT);
}

class Spot : public Light
{
  public:
    Spot(uint8_t id, uint8_t pin, uint8_t numbModes);
};

Spot::Spot(uint8_t id, uint8_t pin, uint8_t numbModes) 
{
  this->numbModes = numbModes;
  this->id = id;
  this->pin = pin;
  this->minOn = 500;
  this->minOff = 500;
  this->maxOff = 10;
  pinMode(pin, OUTPUT);
}

FogMM FogFL(0, FOGFL_PIN, 6, 2);  /*(id, pin, numbModes, defMode)*/
FogMM FogFR(1, FOGFR_PIN, 6, 2);  /*(id, pin, numbModes, defMode)*/
Spot SpotL(2, SPOTL_PIN, 5);      /*(id, pin, numbModes)*/
Spot SpotR(3, SPOTR_PIN, 5);      /*(id, pin, numbModes)*/
FogMM FogBL(4, FOGBL_PIN, 6, 2);  /*(id, pin, numbModes, defMode)*/
FogMM FogBR(5, FOGBR_PIN, 6, 2);  /*(id, pin, numbModes, defMode)*/
Light Lights[NUM_LIGHTS]={FogFL, FogFR, SpotR, SpotL, FogBR, FogBL};
//Light Fogs[NUM_FOGS]={FogFL, FogFR, FogBR, FogBL};
//Light Spots[NUM_SPOTS]={SpotR, SpotL};
