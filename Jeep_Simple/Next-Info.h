#include <Nextion.h>

NexButton btnFog = NexButton(0, 1, "btnFog");
NexButton btnSpot = NexButton(0, 2, "btnSpot");
NexTouch *nex_listen_list[] = 
{
    &btnFog, &btnSpot,
    NULL
};


void btnFog_PopCallback(void *ptr)
{
  dbSerialPrint("btnFog_PopCallback ");
  uint32_t n1 = FogFR.cycleMode();
  uint32_t n2 = FogFL.cycleMode();
  if (n1==n2)       //if both lights agree on same mode...
  {
    btnFog.Set_background_crop_picc(n1);  //Set button backgrnd to curMode
  }
  else
  {
    btnFog.Set_background_crop_picc(0);   //OR set the button background to off
  }
  dbSerialPrintln(n1);
}

void btnSpot_PopCallback(void *ptr)    
{
  dbSerialPrint("btnSpot_PopCallback ");
  uint32_t n1 = SpotR.cycleMode(); 
  uint32_t n2 = SpotL.cycleMode();
  if (n1==n2) //if both lights agree on same mode...
  {
    btnSpot.Set_background_crop_picc(n1);   //Set button backgrnd to curMode
  }
  else
  {
    btnSpot.Set_background_crop_picc(0);   //OR set the button background to off
  }
  dbSerialPrintln(n1);
}


void setup_Nex()
{
	nexInit();
	btnFog.attachPop(btnFog_PopCallback, &btnFog);
  btnSpot.attachPop(btnSpot_PopCallback, &btnSpot);
}
