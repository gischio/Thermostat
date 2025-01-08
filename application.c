/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Temperatura.h"
#include "application.h"
#include "ApplicationTimers.h"
#include "ApplicationUtility.h"
#include "LogicFunctions.h"
#include "FiltersFunction.h"
#include "TemperatureSensor.h"
#include "SensorFromBus.h"
#include "HumiditySensor.h"
#include "Humidification.h"
#include "Dehumidification.h"
#include "FlowTemperature.h"
#include "OutdoorTemperature.h"
#include "WeightedTemperature.h"
#include "Thermostat.h"
#include "Fancoil.h"
#include "PsychrometricValues.h"
#include "ValveProtection.h"
#include "Setpoints.h"
#include "ManualFunction.h"
#include "Anticondensation.h"
#include "HumiDehumiSetpoints.h"
#include "AlarmsAndErrors.h"
#include "sensirion_common.h"
#include "led1642gw.h"
#include "ListValues.h"
#include "CO2Sensor.h"
#include "CO2Functions.h"
#ifdef FIR  
#include "FiltroPrimoOrdine.h"
#endif

#include "i2c.h"
#include "SHT21.h"

#define xxxSPEED_1MIN

#ifdef SPEED_1MIN
    #warning ===============================================================================
    #warning >>>>> ATTENZIONE !
    #warning >>>>> TEST VELOCE FLAG 1 MINUTO
    #warning ===============================================================================
#endif

WORD              delayAfterBusOn;
BYTE              validBusRecovery;         /* ripristino bus terminato */
BYTE              sReady;
MS_State          mainState;
BYTE              stato;
DWORD             alarms;
DWORD             warnings;
EZ_tmStruct       EZ_tmData[EZ_TIMER_NR];

#pragma dataseg=UDATA
__no_init EZ_tmStruct     NI_tmData[NI_TIMER_NR];
__no_init DWORD           a_marker;
__no_init DWORD           z_marker;
__no_init struct  {
                          TYPE_SAVED_DATA;
                  };
#pragma dataseg=default

BYTE manualEnabled;

__no_init unsigned long partialDownload;        

struct APP_Flash  *actualEep;
struct APP_Flash  *emptyEep;

struct APP_Flash  shadow_eeprom;
BYTE              anChn;
unsigned short    adc_temp;
unsigned short    sens_ref;
unsigned char     adcValidFlag;
unsigned int      adValue[5];
unsigned int      adAccValue[5];
long              rTemperature[INPUTS]; 
long              fTemperature[INPUTS];  
long              rCO2Value;
#ifdef FIR
long              fAppCO2Value;
#endif
long              fCO2Value;
long              rRhValue;
long              fRhValue;

BYTE              adAccCnt;
BYTE              _V1(hvacModeUsed,MAX_THERM);

WORD              ledBrightness[NUM_LED_CHANNELS];
WORD              ledMap[NUM_LED1642GW_ICs]={0xFFFF};

#define IMPULSO   40  // 475usec * 40 = 20msec
WORD              startTimerR1;  
WORD              startTimerR2;
BYTE              cmdRelay;
BYTE              statusRelay=3;


unsigned char   saveDelay;

void LedManager(void);
long getTemperature( unsigned short adc_t, unsigned short adc_ref );
void hwI2CInit(void);

//------------------------------------------------------------------

//------------------------------------------------------------------
// LoadApplicationParameters
//------------------------------------------------------------------
void LoadApplicationParameters(void)
{
  for ( BYTE nTherm=0; nTherm<MAX_THERM; nTherm++ )  
  {
      _therm(nTherm).hvacAuto                  = 1; 
      _therm(nTherm).hvacModeForced            = 0;              
      _therm(nTherm).hvacModeFromBus           = 1;              
      _therm(nTherm).hvacMode                  = 1;                    
      _therm(nTherm).hvacModeManual            = 1;              
      _therm(nTherm).DisableTempController     = NO;  
      _therm(nTherm).setpointHumidification    = _PARTHERM(nTherm).HumidificationSetPoint;
      _therm(nTherm).setpointDehumidification  = _PARTHERM(nTherm).DehumidificationSetPoint;      
      _therm(nTherm).setpointBaseManuale       = 2000;
      _therm(nTherm).setpointManuale           = 2000;
      _therm(nTherm).setpointOffset            = 0; // non utilizzato
      _therm(nTherm).neutralZone               = 100;                  
      _therm(nTherm).fanSpeed                  = 6;                     
      _therm(nTherm).manualFanSpeed            = 1;               
      
      if ( _PARTHERM(nTherm).HvacModeAfterDownload == HVAC_AFTER_DOWNLOAD_HEATING )
          _therm(nTherm).heatingCoolingMode    = HEATING;           
      if ( _PARTHERM(nTherm).HvacModeAfterDownload == HVAC_AFTER_DOWNLOAD_COOLING )
          _therm(nTherm).heatingCoolingMode    = COOLING;           
      
      _therm(nTherm).fansDisabled              = NO;                 
      _therm(nTherm).humidificationDisable     = NO;        
      _therm(nTherm).dehumidificationDisable   = NO;      
      _therm(nTherm).auxiliaryHeatingDisabled  = NO;
      _therm(nTherm).auxiliaryCoolingDisabled  = NO;  
      _therm(nTherm).manualActive              = NO;     
  }

  tempThreshold1 = ShortF2LongI(PARAMETER.TempThreshold1Value);
  tempThreshold2 = ShortF2LongI(PARAMETER.TempThreshold2Value);    
  tempThreshold1Lock = FALSE;
  tempThreshold2Lock = FALSE;  
  rhThreshold1 = PARAMETER.RhThreshold1Value * 100;
  rhThreshold2 = PARAMETER.RhThreshold2Value * 100;  
  rhThreshold1Lock = FALSE;
  rhThreshold1Lock = FALSE;  
  CO2VentilationLock = FALSE;
  CO2VentilationValueAtBusOff = 0;
  CO2ThresholdLock[0] = FALSE;
  CO2ThresholdLock[1] = FALSE;
  CO2ThresholdLock[2] = FALSE;  
  CO2VentilationDisableFB = FALSE;
  
  
  if ( PARAMETER.LedsIntensityFromBus == YES )
      ledIntensityFromBus = 127;
  else 
      ledIntensityFromBus = GetBrightness(PARAMETER.LedsIntensity)*255L/100L;
  
  SetpointsAdjustAfterDownload();       
}
extern const DM_SEEPROM cpyDM_eeprom;
//------------------------------------------------------------------
// LoadEEpromActivePage
//------------------------------------------------------------------
void LoadEEpromActivePage(void)
{
  actualEep = GetActivePage();  
  if (actualEep == (struct APP_Flash*)&EEPROM_VAR.page1) 
  {
      emptyEep = (struct APP_Flash*)&EEPROM_VAR.page2;
  }
  else if (actualEep == (struct APP_Flash*)&EEPROM_VAR.page2) 
  {
      emptyEep = (struct APP_Flash*)&EEPROM_VAR.page1;
  }
  else
  { 
      UT_CopyMem ((unsigned char *)&cpyDM_eeprom.eeprom.page1, (unsigned char *)&EEPROM_VAR.page1, sizeof(shadow_eeprom) );
      actualEep = (struct APP_Flash*)&EEPROM_VAR.page1;
      emptyEep  = (struct APP_Flash*)&EEPROM_VAR.page2;
  }

  ErasePage((BYTE*)emptyEep);        
}
//------------------------------------------------------------------
// LoadEEpromParameters
//------------------------------------------------------------------
void LoadEEpromParameters(void)
{
  
  memcpy ((unsigned char *)&shadow_eeprom, (unsigned char *)actualEep, sizeof(shadow_eeprom) );  
  memcpy( &SAVE_START, &shadow_eeprom.SAVE_START, sizeof(TYPE_SAVED_DATA) );
}

//------------------------------------------------------------------
// initialization
//------------------------------------------------------------------
void (* const FInit[])(void) = {
  LogicFunctionsInit,
  FiltersInit,
  TemperatureSensorInit,
  SensorFromBusInit,
  HumiditySensorInit,
  FlowTemperatureInit,
  OutdoorTemperatureInit,
  WeightedTemperatureInit,
  ThermostatInit,
  FancoilInit,
  ValveProtectionInit,
  ValveInit,  
  ManualFunctionInit,
  SetpointsInit,
  AnticondensationInit,
  HumiDehumiSetpointsInit,  
  DehumidificationInit,
  HumidificationInit,
  HumiDehumiSetpointsInit,
  AlarmsAndErrorsInit,
  CO2SensorInit,
  CO2FunctionsInit,
#ifdef FIR  
  FiltroPrimoOrdineInit,
#endif  
};

void APP_Init(void)
{ 

    if ( !VALID_RAM_DATA() )
    {                                   // reset from power down state
        memset( &SAVE_START, 0, sizeof(TYPE_SAVED_DATA) );
    }    
    
    LoadEEpromActivePage();
      
    if ( PARAMETER_DOWNLOADED() )      
    {
        LoadApplicationParameters();
    }
    else
    {
        if ( !VALID_RAM_DATA() )
            LoadEEpromParameters();
    }
    
    OBJ_VALUE->LedsIntensityFB = ledIntensityFromBus;
    
    CLR_PARAMETER_DOWNLOADED();
    validBusRecovery = NO;
    mainState = MS_wait_power_recovery;
    EZ_StartTimer( TimerDelayAfterBusRecovery, GetIntegerConst(&PARAMETER.DelayAfterBusVoltageRecovery) * 10L + 1L, TM_MODE_1MS );
    hwI2CInit();
    led1642gw_init();
    ledMaxBrightness[0] = 0x8FF; 
    ledMaxBrightness[1] = 0x8FF; 
    ledMaxBrightness[2] = 0x8FF; 
    ledMaxBrightness[3] = 0x8FF; 
    ledMaxBrightness[4] = 0x8FF; 
    ledMaxBrightness[5] = 0x8FF; 
    ledMaxBrightness[6] = 0x8FF; 
    ledMaxBrightness[7] = 0x8FF; 
    ledMaxBrightness[8] = 0x8FF; 
    ledMaxBrightness[9] = 0x8FF; 
    ledMaxBrightness[10] = 0x8FF; 
    ledMaxBrightness[11] = 0x8FF; 
    ledMaxBrightness[12] = 0x8FF; 
    ledMaxBrightness[13] = 0x8FF; 
    ledMaxBrightness[14] = 0x8FF; 
    ledMaxBrightness[15] = 0x8FF;    
    
    
    for ( BYTE cnt=0;cnt <NUM_LED_CHANNELS;cnt++ )
        ledBrightness[cnt] =  0; //((long)ledMaxBrightness[cnt]*ledIntensityFromBus)/255L; 
    
    rCO2Value = -9998;
    rRhValue  = -9998;
    
    led1642gw_turn_all_on();
}

//------------------------------------------------------------------
// main routine
//------------------------------------------------------------------
void APP_Main(void)
{ 
  BYTE nrFunc; 

  TimeElapsed();
  
  LedManager();
  
  switch (mainState)
  {
  case MS_wait_power_recovery: 
      // funzione non presente
      // break;
  case MS_wait_bus_recovery:
      if ((validBusRecovery == NO ))//||(sReady != AD_READY))
      {
          if ( EZ_GetState(TimerDelayAfterBusRecovery) )
              validBusRecovery = YES;
      }
      else
      {
          mainState = MS_co_read_on_init;
      }
      break;
      
  case MS_co_read_on_init:
      mainState = MS_enter_normal_run;
      break;

  case MS_enter_normal_run:
      SetThermFunctions();
      SetFancoilFunctions();
      nrFunc = 0;
      while ( nrFunc < sizeof(FInit)/4 ) FInit[nrFunc++]();
      mainState = MS_normal_run;
      break;
      
  case MS_normal_run:
    
      if ( tm.elapsed1sec )
      {
          LogicFunctions();
      }
      for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
      {  
          ENTER_CRITICAL_SECTION()
          rTemperature[0] = getTemperature(adc_temp,sens_ref);
          EXIT_CRITICAL_SECTION()
#ifdef TEMP_VALUE_FROM_BUS
          rTemperature[0] = ShortF2LongI(_OBJV1(TempValue,nTherm))/10;
#endif            
          FilterFunction();
          CO2Sensor();
          if ( !(_V1(thermFunctions,nTherm) & FUNCT_THERMOSTAT) ) continue;
          SensorFromBusTimeoutAndError(nTherm);
          HumiditySensor(nTherm);
          FlowTemperature(nTherm);
          OutdoorTemperature(nTherm);
          _V1(temperature,nTherm) = RoomTemperature(nTherm);
          PsychrometricValue(nTherm);
          HumiDehumiSetpoints(nTherm);          
          Humidification(_V1(hvacModeUsed,nTherm),nTherm);
          Dehumidification(_V1(hvacModeUsed,nTherm),nTherm);          
          _V1(hvacModeUsed,nTherm) = Thermostat(_V1(hvacModeUsed,nTherm),nTherm);
          AlarmsAndErrors(nTherm);
          TemperatureSensor(nTherm);

          CO2Functions();
      }
      SET_VALID_RAM_DATA()
      break;

  case MS_enter_manual_run:
      // funzione non presente
      break;        
      
  case MS_manual_run:
      // funzione non presente
      break;    
  }  
  
  if ( tm.elapsed1sec )
  { 
      if ( saveDelay < SAVE_TIME_DELAY ) saveDelay++;
  }

  TimeElapsedFlagsClear();
}
//------------------------------------------------------------------
// Save program of application 
//------------------------------------------------------------------
void APP_Save(void)
{
  P5OUT = 0x14; // Relay OFF !!!!
  
  if ( saveDelay < SAVE_TIME_DELAY ) return;
  P5OUT &= 0x14;
  saveDelay = 0;
#ifdef TEST_EEPROM_WRITE   
  P3OUT |= BIT4;
#endif  
  CLEAR_VALID_RAM_DATA();
  memcpy( &shadow_eeprom, actualEep, sizeof(struct APP_Flash) );
  memcpy( &shadow_eeprom.SAVE_START, &SAVE_START, sizeof(TYPE_SAVED_DATA) );
#ifdef TEST_EEPROM_WRITE
  shadow_eeprom.before_bus_off_CounterValue[0]++;
#endif  
  
  actualEep = SaveUserData(&shadow_eeprom);
#ifdef TEST_EEPROM_WRITE   
    P3OUT &= ~BIT4;
#endif        
}
//------------------------------------------------------------------
//ACHTUNG: in APPHW... sind keine Zugriffe auf die Kommunikationsobjekte erlaubt
//------------------------------------------------------------------

//------------------------------------------------------------------
//ACHTUNG: in APPHW_Init parameter or objects may not be available
//------------------------------------------------------------------
extern const BYTE cnl_avg[];

void APPHW_Init(void)
{
 
    /* P1 */
    P1DIR |=  (BIT0|BIT3); 
    P1OUT &= ~(BIT0|BIT3);    
    P1SEL |=  (BIT2|BIT1);              // A1,A0
    /* P2 */
    P2OUT =   0;
    P2DIR |=  (BIT2|BIT3|BIT4|BIT6);   
    /* P3 */
    P3DIR |=  (BIT0|BIT1|BIT2|BIT3|BIT4);  
    P3OUT |=  (BIT4|BIT5|BIT6);   
    P3REN |=  (BIT4|BIT5|BIT6);
    P3OUT |=  (BIT0); // Vcc sensor    
    /* P4 */
    P4DIR |=  (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);       
    P4OUT =   0;    
    /* P5 */
    P5DIR |=  (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);   
    P5OUT =   0; 
    /* P6 */
    P6DIR |=  (BIT4|BIT5|BIT6|BIT7); 
    P6REN |=  (BIT0|BIT1|BIT2|BIT3);     
    P6OUT =   0x8F;    
    /* PJ */
    PJDIR = 0x0F;

    ADC10CTL0 = ADC10ON + ADC10SHT_8 + ADC10MSC;      // Turn on and set up ADC10
    ADC10CTL1 = ADC10DIV_7 | ADC10SHP  + ADC10SSEL1 ; // Use sampling timer   
    anChn = 0;                                        // FIRST_ADC_CHANNEL;
    ADC10MCTL0 = ADC10SREF_0 + anChn;                 // Vr+=Vref+
    ADC10IE = 0x0001;                                 // Enable ADC12IFG.0/7
    for (int i=0; i<0x3600; i++);                     // Delay for reference start-up
    ADC10CTL0 |=( ADC10ENC | ADC10SC);                // Enable conversions    
    
#ifdef TEST_EEPROM_WRITE       
    P3DIR |= 0x30;
#endif 
    
    //P6OUT =   0x4F; 

}

//------------------------------------------------------------------
void APPHW_Cycle(void)
{
    static unsigned long last100Ms = 0;
    static unsigned long last10Ms = 0;
    unsigned long delta;    
    
    if ( DM_applicationRunState == 0 )  
    {
        SET_PARAMETER_DOWNLOADED();
    }
    else
    {
        delta = TM_counter1ms - last10Ms;
        if (delta > 10)
        {
            led1642gw_ledbrightness_update(ledBrightness,FLUSH_ENABLED,/*FADE_DISABLED*/FADE_ENABLED,ledblinking);
            led1642gw_ledswitch_update(ledMap,FLUSH_DISABLED,FADE_ENABLED);
            last10Ms = TM_counter1ms;
        }      
        delta = TM_counter1ms - last100Ms;
        if (delta > 100)
        {
            I2C_poll(0);
            last100Ms = TM_counter1ms;
        }    
    }
      
}

#if SC_APPLICATION_TYPE == SC_APPL_CALL_EVENT
void APP_EventHandler(SC_tEventMask event)
{
  if ((event & SC_EVENT_INIT) != 0)
    APP_Init();
//  if ((event & SC_EVENT_100MS) != 0)
//    GetPushButtons();
  if ((event & SC_EVENT_CYCLE) != 0)
    APP_Main();
  //SC_SendEventAppl(SC_TASK_APP1, SC_EVENT_CYCLE);
}
#endif
 
#if SC_APPLICATION_TYPE == SC_APPL_CALL_EVENT
void APPHW_EventHandler(SC_tEventMask event)
{
  if ((event & SC_EVENT_INIT) != 0)
    APPHW_Init();
  if ((event & ~SC_EVENT_INIT) != 0)
    APPHW_Cycle();
}
#endif
//------------------------------------------------------------------
// debug code
//------------------------------------------------------------------
#ifdef DEBUG_WIN32
void App_PrintStatus(void)
{
  T_TRACE0("------------------------------------------\n");
}

#endif //DEBUG_WIN32
 
/*!------------------------------------------------------------------
   Keyboard/output led manager
   ------------------------------------------------------------------*/ 
void LedManager(void)
{
    BYTE ledChn=0;
    unsigned long remainingTime;
    static unsigned char half = 0;
    
    ledIntensityFromBus = OBJ_VALUE->LedsIntensityFB;
    
    ledMap[0] = 0;  // solo un elemento nell'array !!!!
    
    if ( OBJ_VALUE->TechnicalAlarm )
    {
        EZ_GetInfo( TimerLed, &remainingTime );
        if ( remainingTime == 0 )
        {
            EZ_StartTimer( TimerLed, 500L, TM_MODE_1MS ); 
            half ^= 0x01;
        }
        if ( half )
        {
            ledMap[ledChn] |= LED_HEATING;
            ledMap[ledChn] |= LED_CO2_THRESH3; 
        }
    }
    else
    {
        if ( PARAMETER.LedTermEnable )
        {
            if ( _V1(statoRegolazioneHeating,0) == ON )  
            {
                ledMap[ledChn] |= LED_HEATING;
            }
            else if ( _therm(nTherm).heatingCoolingMode == HEATING ) 
            {
                ledMap[ledChn] |= LED_HEATING_MODE;
            }
            if ( _V1(statoRegolazioneCooling,0) == ON )  
            { 
                ledMap[ledChn] |= LED_COOLING;
            }
            else if ( _therm(nTherm).heatingCoolingMode == COOLING ) 
            {
                ledMap[ledChn] |= LED_COOLING_MODE;
            }          
        }
        if ( PARAMETER.LedRhEnable )
        {
              
            if ( reqDeh[0] == ON) 
            {
                ledMap[ledChn] |= LED_DEHUM;
            }
            else if (DEHUMIDIFICATION_ENABLE(0)) 
            {
                //ledMap[ledChn] |= LED_DEHUM_ENABLE;
            }
            
            if ( _OBJV1(HumidCommand,0) == ON )
            {
                ledMap[ledChn] |= LED_HUM;
            }
            else if (HUMIDIFICATION_ENABLE(0)) 
            {
                //ledMap[ledChn] |= LED_HUM_ENABLE;
            }
        }    
        if ( PARAMETER.LedCO2Enable )
        {
            //if ( CO2ThrLevel >= 0 )
                ledMap[ledChn] |= LED_CO2_THRESH0;
            if ( CO2ThrLevel >= 1 )
                ledMap[ledChn] |= LED_CO2_THRESH1;
            if ( CO2ThrLevel >= 2 )
                ledMap[ledChn] |= LED_CO2_THRESH2;
            if ( CO2ThrLevel >= 3 )
                ledMap[ledChn] |= LED_CO2_THRESH3; 
        }    
    }
    
    for (BYTE f=0; f<NUM_LED_CHANNELS;f++ ) 
    {
        ledChn = f/16;
        if ( ledMap[ledChn] & ( 0x0001 << (f%16) ) )
        {
            ledBrightness[f] =  ((long)ledMaxBrightness[f/*/LEDS_X_GROUP*/]*ledIntensityFromBus)/255L; 
        }
        else
        {
            ledBrightness[f] =  0; 
        }
    }     
            
}
//-----------------------------------------------------------------------------
// Interrupt Service Routine for the ADC12 (reading capacitor and bus voltages)
//
// T exec   =   475us
//-----------------------------------------------------------------------------
unsigned short * const pntAdValue[2] = {
  &sens_ref,
  &adc_temp,
};
#pragma vector=ADC10_VECTOR
__interrupt void ADC10ISR (void)
{
    ADC10CTL0 &=~ADC10ENC;  
    *pntAdValue[anChn] = ADC10MEM0;
    anChn >= 1 ? anChn = 0 : anChn++;
    ADC10MCTL0 = (ADC10MCTL0 & 0xF8) + anChn;
    ADC10CTL0 |=( ADC10ENC | ADC10SC);
    if ( anChn == 0 ) adcValidFlag = 1;
}                                               //  ADC10ISR
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#define K_TEMP      15 //150     // -1,5 °C
#define CONV_BITS   1023L   
long            getTemperature( unsigned short adc_t, unsigned short adc_r )
{
    unsigned int full_value;
    full_value = (unsigned long)adc_t * CONV_BITS / ((unsigned long)adc_r * 2L);
    int t = getNTCTemp(full_value);
    return ((long)t) - K_TEMP + STARATURA_TEMPERATURA;
}

///@}

