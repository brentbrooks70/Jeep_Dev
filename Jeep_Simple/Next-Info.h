#include <Nextion.h>

NexButton btnFogF = NexButton(0, 1, "btnFogF");
NexButton btnSpot = NexButton(0, 2, "btnSpot");
NexButton btnFogB = NexButton(0, 5, "btnFogB");
NexDSButton dsFogF = NexDSButton(0, 6, "dsFogF");
NexDSButton dsSpot = NexDSButton(0, 7, "dsSpot");
NexDSButton dsFogB = NexDSButton(0, 8, "dsFogB");

NexTouch *nex_listen_list[] = 
{
    &btnFogF, &btnSpot, &btnFogB,
    &dsFogF, &dsSpot, &dsFogB,
    NULL
};


void btnFogF_PopCallback(void *ptr)
{
  dbSerialPrint("btnFogF_PopCallback ");
  uint32_t n1 = FogFR.cycleMode();
  uint32_t n2 = FogFL.cycleMode();
  if (n1==n2)       //if both lights agree on same mode...
  {
    btnFogF.Set_background_crop_picc(n1);  //Set button backgrnd to curMode
  }
  else
  {
    btnFogF.Set_background_crop_picc(0);   //OR set the button background to off
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

void btnFogB_PopCallback(void *ptr)
{
  dbSerialPrint("btnFogB_PopCallback ");
  uint32_t n1 = FogBR.cycleMode();
  uint32_t n2 = FogBL.cycleMode();
  if (n1==n2)       //if both lights agree on same mode...
  {
    btnFogB.Set_background_crop_picc(n1);  //Set button backgrnd to curMode
  }
  else
  {
    btnFogB.Set_background_crop_picc(0);   //OR set the button background to off
  }
  dbSerialPrintln(n1);
}

void dsFogF_PopCallback(void *ptr)
{
  dbSerialPrint("dsFogF_PopCallback ");
  uint32_t ds;
  dsFogF.getValue(&ds);
  if (ds)       //if lights are on...
  {
    btnFogF.Set_background_crop_picc(2);  //Set button backgrnd to Amber (initial mode for MM!?!)
    FogFR.turnOn();
    FogFL.turnOn();
    delay(90);
    FogFR.cycleMode();
    FogFL.cycleMode();
  }
  else
  {
    btnFogF.Set_background_crop_picc(0);   //OR set the button background to off
    FogFR.turnOff();
    FogFL.turnOff();
  }
  dbSerialPrintln(ds);
}

void dsSpot_PopCallback(void *ptr)    
{
  dbSerialPrint("dsSpot_PopCallback ");
  uint32_t ds;
  dsSpot.getValue(&ds);
  if (ds)       //if lights are on...
  {
    btnSpot.Set_background_crop_picc(1);   //Set button backgrnd to curMode
    SpotR.turnOn();
    SpotL.turnOn();
  }
  else
  {
    btnSpot.Set_background_crop_picc(0);   //OR set the button background to off
    SpotR.turnOff();
    SpotL.turnOff();
  }
  dbSerialPrintln(ds);
}

void dsFogB_PopCallback(void *ptr)
{
  dbSerialPrint("dsFogB_PopCallback ");
  uint32_t ds;
  dsFogB.getValue(&ds);
  if (ds)       //if lights are on...
  {
    btnFogB.Set_background_crop_picc(2);  //Set button backgrnd to Amber (initial mode for MM!?!)
    FogBR.turnOn();
    FogBL.turnOn();
    delay(90);
    FogBR.cycleMode();
    FogBL.cycleMode();
  }
  else
  {
    btnFogB.Set_background_crop_picc(0);   //OR set the button background to off
  }
  dbSerialPrintln(ds);
}




void setup_Nex()
{
	nexInit();
	btnFogF.attachPop(btnFogF_PopCallback, &btnFogF);
  btnSpot.attachPop(btnSpot_PopCallback, &btnSpot);
  btnFogB.attachPop(btnFogB_PopCallback, &btnFogB);
  dsFogF.attachPop(dsFogF_PopCallback, &dsFogF);
  dsSpot.attachPop(dsSpot_PopCallback, &dsSpot);
  dsFogB.attachPop(dsFogB_PopCallback, &dsFogB);
}
