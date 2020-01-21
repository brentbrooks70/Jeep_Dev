#include "JeepLights.h"
FogMM FogFL(0,FOGFL_PIN,6);
FogMM FogFR(1,FOGFR_PIN,6);
Spot SpotL(2,SPOTL_PIN,1);
Spot SpotR(3,SPOTR_PIN,1);
FogMM FogBL(4,FOGBL_PIN, 6);
FogMM FogBR(5,FOGBR_PIN, 6);
Light Lights[NUM_LIGHTS]={FogFL, FogFR, SpotR, SpotL, FogBR, FogBL};
Light Fogs[NUM_FOGS]={FogFL, FogFR, FogBR, FogBL};
Light Spots[NUM_SPOTS]={SpotR, SpotL};
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
