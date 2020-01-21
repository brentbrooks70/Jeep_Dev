/*  ---Define Relay Pins--- */
#define SPOTL_PIN 32
#define SPOTR_PIN 33
#define FOGFL_PIN 26
#define FOGFR_PIN 25
#define FOGBL_PIN 14
#define FOGBR_PIN 15
#define NUM_LIGHTS 6
#define NUM_FOGS 2
#define NUM_SPOTS 2

uint8_t lightCount = 0;
bool masterPower = false;
class Light
{
//  private:

  public:
    uint8_t id;
    uint8_t pin;
    uint8_t type = 4;           //1=Spot, 2=SpotMM, 3=Fog, 4=FogMM, 5=Bar, 6=BarMM
    uint8_t numbModes;      //How many built-in modes (i.e. 6 for FogMM or 1 for Spot)not including custom overrides (i.e. Hazzards, Turn Signals, Reverse, etc.)
    uint8_t curMode = 0;        //0=Off, [FogMM: 1=White, 2=Amber, 3=AmbWhi, 4=WhiStrobe, 5=AmbStrobe, 6=AmbWhiStrobe], [Spot: 1=On]
    uint8_t prevMode = 0;       //To store current mode (Return from int/overide functions or testing prev state for....)
    int8_t pendingMode = -1;     //Next mode to run -1 = no change
    uint16_t minOff = 30;        //Minimum time off to change mode
    uint16_t maxOff = 3300;        //Maximum time off to change mode before reset to mode 1
    uint16_t minOn = 90;         //Minimum time on to qualify as a "cycle"
    unsigned long lastOn = 0;   //millis() of last time it was turned off, maybe use NTP | RTC and timestamp
    unsigned long lastOff = 0;   //millis() of last time it was turned on, maybe use NTP | RTC and timestamp
    unsigned long nextOn = 0;   //millis() or RTC for next on cycle, used for overrides only
    unsigned long nextOff = 0;

    uint8_t cycleMode();
    uint8_t turnOff();
    uint8_t turnOn();

};


uint8_t Light::cycleMode()
{
  if (this->curMode)  //If the light is on, turn it off first
  {
    digitalWrite(this->pin, LOW);
    delay(this->minOff);
  }  //If on last mode, next mode is off, which we just did so return as such
  if (this->curMode == this->numbModes) {this->curMode=0;   return this->curMode;}
  else
  {  //Else turn on next mode, increment mode and return
    digitalWrite(this->pin, HIGH);
    this->curMode++;
    return this->curMode;
  }
}

uint8_t Light::turnOff()
{
  Serial.print("Turning off: ");
  Serial.println(this->id);
  if (this->lastOff + this->minOn > millis())
  {
      Serial.println("Not time yet...");
      return this->curMode;
  }
  digitalWrite(this->pin, LOW);
  this->lastOn = millis();
  this->curMode = 0;
  Serial.println("Done");
  return this->curMode;
}

uint8_t Light::turnOn()
{
  Serial.print("Turning on: ");
  Serial.println(this->id);
  if (this->lastOn + this->minOff > millis())
  {
    Serial.println("Not time yet...");
    return this->curMode;
  }
//  this->prevMode = this->curMode;   ////this should be done before calling, in callback?
  digitalWrite(this->pin, HIGH);
  this->lastOff = millis();
  Serial.println("Done");

  if ( this->lastOn + this->maxOff < millis() )
  {
    this->curMode = 1;
    Serial.println("Mode timed out and reset to 1");
  }
  else
  {
    this->curMode++;
  }
//  this->pendingMode = -1;   ////this should be handled in check
  return this->curMode;
}

class FogMM : public Light
{
  public:
    FogMM(uint8_t id, uint8_t pin, uint8_t numbModes);
};

FogMM::FogMM(uint8_t id, uint8_t pin, uint8_t numbModes) 
{
  this->numbModes = numbModes;
  this->id = id;
  this->pin = pin;
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
  pinMode(pin, OUTPUT);
}
