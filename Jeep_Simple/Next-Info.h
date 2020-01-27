#include <Nextion.h>

NexButton btnFogF = NexButton(0, 1, "btnFogF");
NexButton btnSpot = NexButton(0, 2, "btnSpot");
NexButton btnFogB = NexButton(0, 5, "btnFogB");
NexDSButton dsFogF = NexDSButton(0, 6, "dsFogF");
NexDSButton dsSpot = NexDSButton(0, 7, "dsSpot");
NexDSButton dsFogB = NexDSButton(0, 8, "dsFogB");
NexSlider h1 = NexSlider(0, 9, "h1");

NexTouch *nex_listen_list[] = 
{
    &btnFogF, &btnSpot, &btnFogB,
    &dsFogF, &dsSpot, &dsFogB,
    &h1,
    NULL
};


void btnFogF_PopCallback(void *ptr)
{
  dbSerialPrint("btnFogF_PopCallback ");
//  FogFR.cycleMode();
//  FogFL.cycleMode();
  FogFR.pendingMode = FogFR.curMode + 1;
  FogFL.pendingMode = FogFL.curMode + 1;
  btnFogF.Set_background_crop_picc(FogFL.pendingMode);  //Set button backgrnd to curMode
  dsFogF.setValue(1);
}

void btnSpot_PopCallback(void *ptr)    
{
  dbSerialPrint("btnSpot_PopCallback ");
//  SpotR.cycleMode();
//  SpotL.cycleMode();
  SpotR.pendingMode = SpotR.curMode + 1;
  SpotL.pendingMode = SpotL.curMode + 1;
  btnSpot.Set_background_crop_picc(SpotR.pendingMode);   //Set button backgrnd to curMode
  dsSpot.setValue(1);
}

void btnFogB_PopCallback(void *ptr)
{
  dbSerialPrint("btnFogB_PopCallback ");
//  FogBR.cycleMode();
//  FogBL.cycleMode();
  FogBR.pendingMode = FogBR.curMode + 1;
  FogBL.pendingMode = FogBL.curMode + 1;
  btnFogB.Set_background_crop_picc(FogBL.pendingMode);  //Set button backgrnd to curMode
  dsFogB.setValue(1);
}

void dsFogF_PopCallback(void *ptr)
{
  dbSerialPrint("dsFogF_PopCallback ");
  uint32_t ds;
  dsFogF.getValue(&ds);
  if (ds == 1)       //if lights are on...
  {
      FogFR.pendingMode = FogFR.defMode;
      FogFL.pendingMode = FogFL.defMode;
//    FogFR.turnOn();
//    FogFL.turnOn();
//    delay(FogFL.minOn);
//    FogFR.cycleMode();
//    FogFL.cycleMode();
  }
  else
  {
  FogFR.pendingMode = 0;
  FogFL.pendingMode = 0;
  }
  Serial.println(FogFL.pendingMode);
  btnFogF.Set_background_crop_picc(FogFL.pendingMode);  //Set button backgrnd to Amber (initial mode for MM!?!)
}

void dsSpot_PopCallback(void *ptr)    
{
  dbSerialPrint("dsSpot_PopCallback ");
  uint32_t ds;
  dsSpot.getValue(&ds);
  if (ds == 1)       //if lights are on...
  {
    SpotR.pendingMode = SpotR.defMode;
    SpotL.pendingMode = SpotR.defMode;
//    SpotR.turnOn();
//    SpotL.turnOn();
  }
  else
  {
    SpotR.pendingMode = 0;
    SpotL.pendingMode = 0;
  }
  btnSpot.Set_background_crop_picc(SpotL.pendingMode);   //Set button backgrnd to curMode
}

void dsFogB_PopCallback(void *ptr)
{
  dbSerialPrint("dsFogB_PopCallback ");
  uint32_t ds;
  dsFogB.getValue(&ds);
  if (ds == 1)       //if lights are on...
  {
    FogBR.pendingMode = FogBR.defMode;
    FogBL.pendingMode = FogBL.defMode;
//    FogBR.turnOn();
//    FogBL.turnOn();
//    delay(FogBL.minOn);
//    FogBR.cycleMode();
//    FogBL.cycleMode();
  }
  else
  {
    FogBR.pendingMode = 0;
    FogBL.pendingMode = 0;
//    FogBR.turnOff();
//    FogBL.turnOff();
  }
  btnFogB.Set_background_crop_picc(FogBL.pendingMode);  //Set button backgrnd to Amber (initial mode for MM!?!)
}

void h1_PopCallback(void *ptr)
{
    dbSerialPrint("h1_PopCallback ");
    uint32_t number = 0;
    h1.getValue(&number);
    SpotR.minOn = number;
    SpotL.minOn = number;
    SpotR.minOff = number;
    SpotL.minOff = number;
    char temp[10] = {0};
    utoa(number, temp, 10);
    dbSerialPrintln(number);
    dbSerialPrintln(temp);
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
  h1.attachPop(h1_PopCallback, &h1);
}
