#include "JeepLights.h"
FogMM FogFL(0,FOGFL_PIN,6);
FogMM FogFR(1,FOGFR_PIN,6);
Spot SpotL(2,SPOTL_PIN,1);
Spot SpotR(3,SPOTR_PIN,1);
#include "Next-Info.h"

void setup() 
{
  delay(500);
  setup_Nex();
  delay(500);
}

void loop() 
{
  nexLoop(nex_listen_list);
}
