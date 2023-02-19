#include <MsTimer2.h>
#include "sevensig.h"
#include <EEPROM.h>
#include <avr/wdt.h> 

uint8_t VOLT_ID = 6;
uint8_t DIAG_ID = 7;
uint8_t BUTTONS_ID = 1;

int button_mode = 0;
int button_up = 128;
int button_down = 170;

boolean TodMode = true;
int LockMode = -1; //lock mode
uint16_t VoltCnt = 0;
uint8_t vCnt=0;

byte store_mode = 0;
byte v_cor = 100;

//segments control
volatile unsigned char dispD[] =
{
 0x00, //0
 0x00, //1
 0x00, //2
 0x00, //3
};

volatile unsigned char dispC[] =
{
 0x00, //0
 0x00, //1
 0x00, //2
 0x00, //3
};

volatile unsigned char nums[] =
{
 L_O, //0
 L_1, //1
 L_2, //0
 L_3, //1
 L_4, //0
 L_5, //1
 L_6, //0
 L_7, //1
 L_8, //0
 L_9, //1
};

volatile unsigned char numsC[] =
{
 L_O_c, //0
 L_1_c, //1
 L_2_c, //0
 L_3_c, //1
 L_4_c, //0
 L_5_c, //1
 L_6_c, //0
 L_7_c, //1
 L_8_c, //0
 L_9_c, //1
};

volatile unsigned char nums_point[] =
{
 L_O_point, //0
 L_1_point, //1
 L_2_point, //0
 L_3_point, //1
 L_4_point, //0
 L_5_point, //1
 L_6_point, //0
 L_7_point, //1
 L_8_point, //0
 L_9_point, //1
};

volatile unsigned char nums_pointC[] =
{
 L_O_point_c, //0
 L_1_point_c, //1
 L_2_point_c, //0
 L_3_point_c, //1
 L_4_point_c, //0
 L_5_point_c, //1
 L_6_point_c, //0
 L_7_point_c, //1
 L_8_point_c, //0
 L_9_point_c, //1
};

long counter = 0;
volatile byte sdv = 0;

void mypwm()
{
 sdv++;
 if (sdv>=4) sdv=0;
 SEG_OFF; 
 if (sdv==0) {PORTD|=dispD[0];PORTC|=dispC[0];SEG_1;} //1 seg
 if (sdv==1) {PORTD|=dispD[1];PORTC|=dispC[1];SEG_2;} //2 seg
 if (sdv==2) {PORTD|=dispD[2];PORTC|=dispC[2];SEG_3;} //3 seg 
 if (sdv==3) {PORTD|=dispD[3];PORTC|=dispC[3];SEG_4;} //4 seg
}

void ShowAuto()
{
 dispD[0]=L_A;
 dispD[1]=L_u;
 dispD[2]=L_t;
 dispD[3]=L_o;

 dispC[0]=L_A_c;
 dispC[1]=L_u_c;
 dispC[2]=L_t_c;
 dispC[3]=L_o_c;
}

void Show2H()
{
 dispD[0]=L_empty;
 dispD[1]=L_2;
 dispD[2]=L_defis;
 dispD[3]=L_H;

 dispC[0]=L_empty_c;
 dispC[1]=L_2_c;
 dispC[2]=L_defis_c;
 dispC[3]=L_H_c;
}

void ShowVC()
{
 dispD[0]=L_S;
 dispD[1]=L_defis;
 dispD[2]=L_u;
 dispD[3]=L_C;

 dispC[0]=L_S_c;
 dispC[1]=L_defis_c;
 dispC[2]=L_u_c;
 dispC[3]=L_C_c;
}

void ShowVersion()
{
  dispD[1]=L_defis;
  dispD[0]=L_L;
  dispD[2]=L_3_point;
  dispD[3]=L_1;

  dispC[1]=L_defis_c;
  dispC[0]=L_L_c;
  dispC[2]=L_3_point_c;
  dispC[3]=L_1_c;  
}

void ShowPreset2H()
{
 dispD[0]=L_S;
 dispD[1]=L_defis;
 dispD[2]=L_2;
 dispD[3]=L_H;

 dispC[0]=L_S_c;
 dispC[1]=L_defis_c;
 dispC[2]=L_2_c;
 dispC[3]=L_H_c;
}

void ShowPresetAu()
{
 dispD[0]=L_S;
 dispD[1]=L_defis;
 dispD[2]=L_A;
 dispD[3]=L_u;

 dispC[0]=L_S_c;
 dispC[1]=L_defis_c;
 dispC[2]=L_A_c;
 dispC[3]=L_u_c;
}

void Show4H()
{
 dispD[0]=L_empty;
 dispD[1]=L_4;
 dispC[0]=L_empty_c;
 dispC[1]=L_4_c;
 dispD[2]=L_defis;
 dispC[2]=L_defis_c;
 dispD[3]=L_H;
 dispC[3]=L_H_c;
}

void ShowMem()
{
 dispD[0]=L_defis;
 dispD[1]=L_defis;
 dispD[2]=L_defis;
 dispD[3]=L_defis;

 dispC[0]=L_defis_c;
 dispC[1]=L_defis_c;
 dispC[2]=L_defis_c;
 dispC[3]=L_defis_c;
 DIODE_ALL_ON;
}

void ShowPause()
{
 dispD[0]=L_empty;
 dispD[1]=L_empty;
 dispD[2]=L_empty;
 dispD[3]=L_empty;

 dispC[0]=L_empty_c;
 dispC[1]=L_empty_c;
 dispC[2]=L_empty_c;
 dispC[3]=L_empty_c;
 DIODE_ALL_OFF;
}

void ShowValue(int value)
{
 unsigned char i;
 unsigned int tval = 0;
 if (value < 0)
 {
   dispD[0]=L_defis;
   dispC[0]=L_defis_c;
 }
 else
 {
   dispD[0]=L_empty;
   dispC[0]=L_empty_c;
 }
 
 tval = abs(value);
 i=tval/100;		   //Display voltage
 tval=tval%100;
 if (i==0) {dispD[1]=L_empty; dispC[1]=L_empty_c;} else {dispD[1]=nums[i]; dispC[1]=numsC[i];};
 i=tval/10;
 tval=tval%10;
 dispD[2]=nums_point[i];
 dispD[3]=nums[tval];  
 dispC[2]=nums_pointC[i];
 dispC[3]=numsC[tval];  
}

void volt()
{
  int voltage = analogRead(VOLT_ID)/4 + 9 + (v_cor-100);
  ShowValue(voltage);
}

void reboot()
{
  delay(2001); //wdt = 2000ms
}

void PresetSummerMode()
{
  int btnTmp = analogRead(BUTTONS_ID)/4;
  boolean trigger = true;
  if ((btnTmp>(button_up - 10)) && (btnTmp<(button_up + 10)))
  {
    delay(500);
    btnTmp = analogRead(BUTTONS_ID)/4;
    if ((btnTmp>(button_up - 10)) && (btnTmp<(button_up + 10)))
    {
      store_mode = EEPROM.read(0);
      if (store_mode>2)
      {
        store_mode=0;
        EEPROM.write(0, store_mode);
      }
      
      for (int i=0; i<9; i++)
      {
        trigger = !trigger;
        (trigger)?ShowMem():ShowPause();
        delay(250);
        wdt_reset();
      }
      
      (store_mode==0)?ShowPresetAu():ShowPreset2H();
      while(1)
      {
        //ShowValue(store_mode);
        (store_mode==0)?ShowPresetAu():ShowPreset2H();
        if ((analogRead(BUTTONS_ID)/4)<10)
        {
          delay(50);
          if ((analogRead(BUTTONS_ID)/4)<10)
          {
            EEPROM.write(0, store_mode);
            reboot();
          }          
        }
        btnTmp = analogRead(BUTTONS_ID)/4;                    
        if ((btnTmp>(button_up - 10)) && (btnTmp<(button_up + 10)))
          {
            delay(50);
            btnTmp = analogRead(BUTTONS_ID)/4; 
            if ((btnTmp>(button_up - 10)) && (btnTmp<(button_up + 10)))
            { 
              if (store_mode < 1)
              {
                store_mode++;
              }
              delay(100);
            }
          }
          btnTmp = analogRead(BUTTONS_ID)/4; 
          if ((btnTmp>(button_down - 10)) && (btnTmp<(button_down + 10)))
          {
            delay(50);
            btnTmp = analogRead(BUTTONS_ID)/4; 
            if ((btnTmp>(button_down - 10)) && (btnTmp<(button_down + 10)))
            { 
              if (store_mode > 0)
              {
                store_mode--;
              }
              delay(100);
            }
          }       
          wdt_reset();
      }               
    } 
  }
}

void PresetVoltCorrection()
{
  int btnTmp = analogRead(BUTTONS_ID)/4;
  if ((btnTmp>(button_down - 10)) && (btnTmp<(button_down + 10)))
  {
    delay(500);
    btnTmp = analogRead(BUTTONS_ID)/4;
    if ((btnTmp>(button_down - 10)) && (btnTmp<(button_down + 10)))
    {

      ShowVC();
      for (int j=0; j<4; j++)
      {
        DIODE_ALL_ON;
        delay(200);        
        wdt_reset();
        DIODE_ALL_OFF;
        delay(200);        
        wdt_reset();
      }
      
      ShowValue(v_cor-100);
      wdt_reset();
      
      delay(1400);
      wdt_reset();
      while (1)
      {  
        ShowValue(v_cor-100);
        if (analogRead(BUTTONS_ID)/4<10)
        {
          delay(100);
          if (analogRead(BUTTONS_ID)/4<10)
          {
            EEPROM.write(2, v_cor);
            delay(2);
            DIODE_ALL_ON;
            delay(200);
            DIODE_ALL_OFF;
            //reboot();
            break;
          }          
        }
        btnTmp = analogRead(BUTTONS_ID)/4;
        if ((btnTmp>(button_up - 10)) && (btnTmp<(button_up + 10)))
          {
            delay(50);
            btnTmp = analogRead(BUTTONS_ID)/4;
            if ((btnTmp>(button_up - 10)) && (btnTmp<(button_up + 10)))
            { 
              if (v_cor < 150)
              {
                v_cor++;
              }
              delay(100);
            }
          }
          btnTmp = analogRead(BUTTONS_ID)/4;
          if ((btnTmp>(button_down - 10)) && (btnTmp<(button_down + 10)))
          {
            delay(50);
            btnTmp = analogRead(BUTTONS_ID)/4;
            if ((btnTmp>(button_down - 10)) && (btnTmp<(button_down + 10)))
            { 
              if (v_cor > 50)
              {
                v_cor--;
              }
              delay(100);
            }
          }
        wdt_reset();
      }
    }
  }  
}

void setup()
{
  //init MCU
  DDRB = B01111111;
  PORTB = B01011011;  
  DDRD |= B11111100;
  PORTD |= B11111100;
  DDRC |= B00111101;

  MsTimer2::set(1, mypwm);
  MsTimer2::start();
  
  ShowVersion();
  delay(100);
  for (int i=0; i<3; i++)
  {
    delay(200);
    _DIODE_AUTO_ON;
    delay(200);
    _DIODE_2H_ON;
    delay(200);
    _DIODE_4H_ON;
    delay(200);
    DIODE_ALL_OFF;
  }
  wdt_enable(WDTO_2S);
    
  ShowAuto();
  _4H_OFF;
  
  v_cor = EEPROM.read(2);
  if ((v_cor > 150) || (v_cor < 50))
  {
    EEPROM.write(2, 100);
    v_cor = 100;
  }
  store_mode = EEPROM.read(0);
  if (store_mode==1)
  {
    LockMode=1;
    TodMode=false;
    digitalWrite(ActivatePin, HIGH);
    PrintLockMode();            
  }  
}

void PrintLockMode()
{
  if (LockMode == 0)
  {
    DIODE_ALL_OFF;
    Show4H();
    _DIODE_4H_ON;
  }
  if (LockMode == 1)
  {
    DIODE_ALL_OFF;
    Show2H();
    _DIODE_2H_ON;
  }
}

void _Up()
{
  int btnTmp = analogRead(BUTTONS_ID)/4;
  if ((btnTmp>(button_up - 10)) && (btnTmp<(button_up + 10)))
  {
    delay(50);
    btnTmp = analogRead(BUTTONS_ID)/4;
    if ((btnTmp>(button_up - 10)) && (btnTmp<(button_up + 10)))
    {
      delay(500);
      btnTmp = analogRead(BUTTONS_ID)/4;
      if ((btnTmp>(button_up - 10)) && (btnTmp<(button_up + 10)))
      {
         PresetSummerMode();
         
         //reboot(); 
      }
      else
      {
          digitalWrite(ActivatePin, HIGH);
          _4H_OFF;
          LockMode=1;
          TodMode=false;
          //digitalWrite(ActivatePin, HIGH);
          PrintLockMode();
          //MsTimer2::start();
        delay(250);
      }
    }
  }  
}

//118
void _Down()
{
  int btnTmp = analogRead(BUTTONS_ID)/4;
  if ((btnTmp>(button_down - 10)) && (btnTmp<(button_down + 10)))
  {
    
    delay(50);
    btnTmp = analogRead(BUTTONS_ID)/4;
    if ((btnTmp>(button_down - 10)) && (btnTmp<(button_down + 10)))
    {
      delay(500);
      btnTmp = analogRead(BUTTONS_ID)/4;
      if ((btnTmp>(button_down - 10)) && (btnTmp<(button_down + 10)))
      {
        PresetVoltCorrection();
        //reboot();
      }
      else
      {
          _DIODE_AUTO_ON;
          digitalWrite(ActivatePin, LOW);
          _4H_OFF;
          delay(200);
          LockMode=-1;
          TodMode=true;
          DIODE_ALL_OFF;
          ShowAuto();
      }
      delay(250);
    }
  }  
}

// 0
void _Mode()
{
  if (analogRead(BUTTONS_ID)<30)
  {
    delay(50);
    if (analogRead(BUTTONS_ID)<30)
    {
      TodMode = false;
      digitalWrite(ActivatePin, HIGH);
      _4H_ON;
      delay(1);
      LockMode=0;
      PrintLockMode();
      delay(250);
    }
  }
}

void loop() {
  wdt_reset(); 
  
  VoltCnt++;
  if ((VoltCnt>=5000) && (TodMode))
  {
    VoltCnt=0;
    volt();
  }
  _Mode();
  _Up();
  _Down();
}
