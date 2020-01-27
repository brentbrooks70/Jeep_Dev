#define Ver "0.0.3"

#include "JeepLights.h"
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
  for (int i = 0; i < NUM_LIGHTS; i++)
  {
    Lights[i].srvMain();
  }
  FogFR.srvMain();
  FogFL.srvMain();
}
