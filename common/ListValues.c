#include <string.h>
#include "Application.h"
#include "ListValues.h"
                                   
//------------------------------------------------------------------                                   
//------------------------------------------------------------------
// liste ......                                
//------------------------------------------------------------------
//------------------------------------------------------------------
//================================================================== 
//   Cyclic sending interval                   
//==================================================================
const WORD tCyclicSendingInterval[] = { // sec
  0, 30, 1*60, 2*60, 3*60, 4*60, 5*60, 7*60, 10*60, 15*60, 20*60, 30*60, 
  45*60, 60*60, 90*60, 120*60
};
//------------------------------------------------------------------
// GetCyclicSendingInterval
//------------------------------------------------------------------
WORD GetCyclicSendingInterval( BYTE p )
{
  if ( p < NELEMS(tCyclicSendingInterval) ) return tCyclicSendingInterval[p];
  else return tCyclicSendingInterval[0];
}
//================================================================== 
//   Cyclic reading interval                   
//==================================================================
const WORD tCyclicReadingInterval[] = { // sec
  0, 30, 1*60, 2*60, 3*60, 4*60, 5*60, 7*60, 10*60, 15*60, 20*60, 30*60, 
  45*60, 60*60, 90*60, 120*60
};
//------------------------------------------------------------------
// GetCyclicReadingInterval
//------------------------------------------------------------------
WORD GetCyclicReadingInterval( BYTE p )
{
  if ( p < NELEMS(tCyclicReadingInterval) ) return tCyclicReadingInterval[p];
  else return tCyclicReadingInterval[0];
}
//================================================================== 
//   Relative weight                            
//==================================================================
const BYTE tRelativeWeight[][2] = { // %
  { 100,   0},{  90,  10},{  80,  20},{  70,  30},{  60,  40},{  50,  50},    
  {  40,  60},{  30,  70},{  20,  80},{  10,  90},{   0, 100},  
};
//------------------------------------------------------------------
// GetRelativeWeight
//------------------------------------------------------------------
BYTE GetRelativeWeight( BYTE p )
{
  if ( p < (sizeof(tRelativeWeight)/2) ) return p;
  else return 0;
}       
//================================================================== 
//   Time to return to default diplay or to save                            
//==================================================================
const BYTE tTimeToReturn[] = { // sec
  5, 10, 15, 20, 25, 30 , 45, 60
};
//------------------------------------------------------------------
// GetTimeToReturn
//------------------------------------------------------------------
BYTE GetTimeToReturn ( BYTE p )
{
  if ( p < NELEMS(tTimeToReturn) ) return tTimeToReturn[p];
  else return tTimeToReturn[0];
} 
//================================================================== 
//   Time to delete changes                   
//==================================================================
const BYTE tTimeToDeleteChanges[] = { // sec
  5, 10, 15, 20, 25, 30 , 45, 60
};
//------------------------------------------------------------------
// GetTimeToDeleteChanges
//------------------------------------------------------------------
BYTE GetTimeToDeleteChanges ( BYTE p )
{
  if ( p < NELEMS(tTimeToDeleteChanges) ) return tTimeToDeleteChanges[p];
  else return tTimeToDeleteChanges[0];
} 
//================================================================== 
//   Time to save manual change                   
//==================================================================
const BYTE tTimeToSaveManual[] = { // sec
  2, 4, 6, 8, 10, 12
};
//------------------------------------------------------------------
// GetTimeToSaveManual
//------------------------------------------------------------------
BYTE GetTimeToSaveManual ( BYTE p )
{
  if ( p < NELEMS(tTimeToSaveManual) ) return tTimeToSaveManual[p];
  else return tTimeToSaveManual[0];
} 
//================================================================== 
//   Temperature integration               
//==================================================================
const int tTemperatureIntegration[] = { // 1/100 K/m
   50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200,
   210, 220, 230, 240, 250, 260, 270, 280, 290, 300
};
//------------------------------------------------------------------
// GetTemperatureDifferential
//------------------------------------------------------------------
int GetTemperatureIntegration ( char p )
{
  if ( p < NELEMS(tTemperatureIntegration) ) return tTemperatureIntegration[p];
  else return tTemperatureIntegration[0];
}
//================================================================== 
//   Temperature offset                          
//==================================================================
const signed char tTemperatureOffset[] = { // 1/10 K
  -50, -49, -48, -47, -46, -45, -44, -43, -42, -41,
  -40, -39, -38, -37, -36, -35, -34, -33, -32, -31,
  -30, -29, -28, -27, -26, -25, -24, -23, -22, -21, 
  -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, 
  -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  
    10,  11,  12,  13,  14,  15,  16,  17, 18,  19,
    20,  21,  22,  23,  24,  25,  26,  27, 28,  29,    
    30,  31,  32,  33,  34,  35,  36,  37, 38,  39,
    40,  41,  42,  43,  44,  45,  46,  47, 48,  49,
    50,
};
//------------------------------------------------------------------
// GetTemperatureOffset 
//------------------------------------------------------------------
signed char GetTemperatureOffset ( char p )
{
  if ( p < NELEMS(tTemperatureOffset) ) return tTemperatureOffset[p];
  else return tTemperatureOffset[0];
  
} 
//================================================================== 
//   Rh offset                          
//==================================================================
const signed char tRhOffset[] = { // &
  -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 10
};
//------------------------------------------------------------------
// GetRhOffset 
//------------------------------------------------------------------
signed char GetRhOffset ( char p )
{
  if ( p < NELEMS(tRhOffset) ) return tRhOffset[p];
  else return tRhOffset[10];
  
} 
//================================================================== 
//   CO2 offset                          
//==================================================================
const signed int tCO2Offset[] = { // &
  -400,  -350,  -300,  -250,  -200,  -150,  -100,  -50,  0, 
    50,   100,   150,   200,   250,   300,   350,   400
};
//------------------------------------------------------------------
// GetCO2Offset 
//------------------------------------------------------------------
signed int GetCO2Offset ( char p )
{
  if ( p < NELEMS(tCO2Offset) ) return tCO2Offset[p];
  else return tCO2Offset[8];
  
} 
//================================================================== 
//   Temperature differential                        
//==================================================================
const int tTemperatureDifferential[] = { // 1/100 K/m
   25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400
};
//------------------------------------------------------------------
// GetTemperatureDifferential
//------------------------------------------------------------------
int GetTemperatureDifferential ( char p )
{
  if ( p < NELEMS(tTemperatureDifferential) ) return tTemperatureDifferential[p];
  else return tTemperatureDifferential[0];
} 
//================================================================== 
//   Max temperature/setpoint change                        
//==================================================================
const WORD tMaxTemperatureChange[] = { // K  +/-
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
};
//------------------------------------------------------------------
// GetMaxTemperatureChang
//------------------------------------------------------------------
WORD GetMaxTemperatureChange ( char p )
{
  if ( p < NELEMS(tMaxTemperatureChange) ) return tMaxTemperatureChange[p];
  else return tMaxTemperatureChange[0];
}         
//================================================================== 
//   Offset from setpoint
//==================================================================
const WORD tOffsetFromSetpoint[] = { // 1/10 K
  0, 2, 4, 6, 8, 10, 15, 20, 25, 30
};
//------------------------------------------------------------------
// GetOffsetFromSetpoint
//------------------------------------------------------------------
WORD GetOffsetFromSetpoint ( char p )
{
  if ( p < NELEMS(tOffsetFromSetpoint) ) return tOffsetFromSetpoint[p];
  else return tOffsetFromSetpoint[0];
}     
//================================================================== 
//   Hysteresis
//==================================================================
const BYTE tHysteresis[] = { // 1/10 K
  2, 3, 4, 5, 6, 8, 10, 15, 20, 25, 30
};
//------------------------------------------------------------------
// GetHysteresist
//------------------------------------------------------------------
BYTE GetHysteresis ( char p )
{
  if ( p < NELEMS(tHysteresis) ) return tHysteresis[p];
  else return tHysteresis[0];
}      
//================================================================== 
//   LightHysteresis
//==================================================================
const BYTE tLightHysteresiss[] = { // lux
  5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 100, 120, 140, 160, 180 , 200
};
//------------------------------------------------------------------
// GetLightHysteresis
//------------------------------------------------------------------
BYTE GetLightHysteresis ( char p )
{
  if ( p < NELEMS(tLightHysteresiss) ) return tLightHysteresiss[p];
  else return tLightHysteresiss[0];
} 
//================================================================== 
//   Ventilation start/stop delay
//==================================================================
const WORD tVentilationDelay[] = { // sec
  0/*?*/,  0 , 10,  20,  60,  90, 120, 150, 180, 300, 360, 480, 600, 720  
};
//------------------------------------------------------------------
// GetVentilationDelay
//------------------------------------------------------------------
WORD GetVentilationDelay ( char p )
{
  if ( p < NELEMS(tVentilationDelay) ) return tVentilationDelay[p];
  else return tVentilationDelay[0];
}       
//================================================================== 
//   Display brightness
//==================================================================
const BYTE tBrightness[] = { // %
  0,10, 20, 30, 40, 50, 60, 70, 80, 90, 100
};
//------------------------------------------------------------------
// GetBrightness
//------------------------------------------------------------------
BYTE GetBrightness( char p )
{
  if ( p < NELEMS(tBrightness) ) return tBrightness[p];
  else return tBrightness[0];
}  
//================================================================== 
//   Display standby brightness
//==================================================================
const BYTE tStandbyBrightness[] = { // %
  0, 2, 5, 10, 15, 20, 25, 30
};
//------------------------------------------------------------------
// GetStandbyBrightnesss
//------------------------------------------------------------------
BYTE GetStandbyBrightness( char p )
{
  if ( p < NELEMS(tStandbyBrightness) ) return tStandbyBrightness[p];
  else return tStandbyBrightness[0];
}                                     
//================================================================== 
//   Leds intensity
//==================================================================
const BYTE tLedsIntensity[] = { // %
  0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
};
//------------------------------------------------------------------
// GetLedsIntensity
//------------------------------------------------------------------
BYTE GetLedsIntensity( char p )
{
  if ( p < NELEMS(tLedsIntensity) ) return tLedsIntensity[p];
  else return tLedsIntensity[0];
} 
//================================================================== 
//   Humidity Hysteresis
//==================================================================
const BYTE tHumidityHysteresis[] = { // 1/10 %
  5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 25, 30, 35, 40
};
//------------------------------------------------------------------
// GetHumidityHysteresis
//------------------------------------------------------------------
BYTE GetHumidityHysteresis( char p )
{
  if ( p < NELEMS(tHumidityHysteresis) ) return tHumidityHysteresis[p];
  else return tHumidityHysteresis[0];
}
//================================================================== 
//   Dewpoint/Temperature min chage to send
//==================================================================
const BYTE tTemperatureMinChangeToSend[] = { // 1/10 K
  0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30
};
//------------------------------------------------------------------
// GetTemperatureMinChangeToSend
//------------------------------------------------------------------
BYTE GetTemperatureMinChangeToSend( char p )
{
  if ( p < NELEMS(tTemperatureMinChangeToSend) ) return tTemperatureMinChangeToSend[p];
  else return tTemperatureMinChangeToSend[0];
} 
//================================================================== 
//   Enthalpy min chage to send
//==================================================================
const float tEnthalpyMinChangeToSend[] = { // kJ/kg
  0.0, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0
};
//------------------------------------------------------------------
// GetEnthalpyMinChangeToSend
//------------------------------------------------------------------
float GetEnthalpyMinChangeToSend( char p )
{
  if ( p < NELEMS(tEnthalpyMinChangeToSend) ) return tEnthalpyMinChangeToSend[p];
  else return tEnthalpyMinChangeToSend[0];
}  
//================================================================== 
//   Absolute humidity min chage to send
//==================================================================
const float tAbsHumidityMinChangeToSend[] = { //  gv/kga
  0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5
};
//------------------------------------------------------------------
// GetAbsHumidityMinChangeToSend
//------------------------------------------------------------------
float GetAbsHumidityMinChangeToSend( char p )
{
  if ( p < NELEMS(tAbsHumidityMinChangeToSend) ) return tAbsHumidityMinChangeToSend[p];
  else return tAbsHumidityMinChangeToSend[0];
}
//================================================================== 
//   Time before energy saving
//==================================================================
const BYTE tTimeBeforEnergySaving[] = { // sec
  10 , 15, 30, 45, 60
};
//------------------------------------------------------------------
// GetTimeBeforEnergySaving
//------------------------------------------------------------------
BYTE GetTimeBeforEnergySaving( char p )
{
  if ( p < NELEMS(tTimeBeforEnergySaving) ) return tTimeBeforEnergySaving[p];
  else return tTimeBeforEnergySaving[0];
}        
//================================================================== 
//   End of manual operation
//==================================================================
const WORD tEndOfManualOperation[] = { // min
  0 , 30, 1*60, 2*60, 3*60, 4*60, 6*60, 9*60, 12*60, 15*60, 18*60, 
  24*60, 30*60, 36*60, 42*60, 48*60
};
//------------------------------------------------------------------
// GetEndOfManualOperationg
//------------------------------------------------------------------
WORD GetEndOfManualOperation( char p )
{
  if ( p < NELEMS(tEndOfManualOperation) ) return tEndOfManualOperation[p];
  else return tEndOfManualOperation[0];
}
//================================================================== 
//   End of comfort externsion
//==================================================================
const WORD tEndOfComfortExtension[] = { // min
  0 , 15, 30, 45, 60, 90, 120, 150, 180, 210, 240
};
//------------------------------------------------------------------
// GeEndOfComfortExtension
//------------------------------------------------------------------
WORD GeEndOfComfortExtension( char p )
{
  if ( p < NELEMS(tEndOfComfortExtension) ) return tEndOfComfortExtension[p];
  else return tEndOfComfortExtension[0];
}
//================================================================== 
//   Valve protection frequency
//==================================================================
const WORD tValveProtectionFrequency[] = { // min
  1440 , 10080, 43200
};
//------------------------------------------------------------------
// GetValveProtectionFrequency
//------------------------------------------------------------------
WORD GetValveProtectionFrequency( char p )
{
  if ( p < NELEMS(tValveProtectionFrequency) ) return tValveProtectionFrequency[p];
  else return tValveProtectionFrequency[NELEMS(tValveProtectionFrequency)-1];
}
//================================================================== 
//   Valve protection time
//==================================================================
const WORD tValveProtectionTime[] = { // sec
  5 , 10, 20, 30, 300, 600, 900, 1200
};
//------------------------------------------------------------------
// GetValveProtectionTime
//------------------------------------------------------------------
WORD GetValveProtectionTime( char p )
{
  if ( p < NELEMS(tValveProtectionTime) ) return tValveProtectionTime[p];
  else return tValveProtectionTime[0];
}
//================================================================== 
//   Ventilation at timeout
//==================================================================
const BYTE tVentilationAtTimeOut[] = { // percentage
  0 , 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
};
//------------------------------------------------------------------
// GetVentilationAtTimeOut
//------------------------------------------------------------------
BYTE GetVentilationAtTimeOut( char p )
{
  if ( p < NELEMS(tVentilationAtTimeOut) ) return tVentilationAtTimeOut[p];
  else return tVentilationAtTimeOut[0];
}
//================================================================== 
//   Coeficient temperature compensation
//==================================================================
const BYTE tCoefCompensation[] = { // °C
  8, 7, 6, 5, 4, 3, 2, 1
};
//------------------------------------------------------------------
// GetCoefCompensation
//------------------------------------------------------------------
BYTE GetCoefCompensation( char p )
{
  if ( p < NELEMS(tCoefCompensation) ) return tCoefCompensation[p];
  else return tCoefCompensation[0];
}
//================================================================== 
//   Compensation COV
//==================================================================
const int tCompensationCOV[] = { // 1/100°C
  0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280,
  300, 320, 340, 360, 380, 400, 420, 440, 460, 480, 500 
};
//------------------------------------------------------------------
// GetCompensationCOV
//------------------------------------------------------------------
int GetCompensationCOV( char p )
{
  if ( p < NELEMS(tCompensationCOV) ) return tCompensationCOV[p];
  else return tCompensationCOV[0];
}
//================================================================== 
//   Percentage continuous speed limit
//==================================================================
const BYTE tFanLimitContinuous[] = { // percentage (0..255)
  0, 
  10*255/100, 20*255/100, 30*255/100, 40*255/100, 50*255/100, 60*255/100, 
  70*255/100, 80*255/100, 90*255/100
};
//------------------------------------------------------------------
// GetFanLimitContinuous
//------------------------------------------------------------------
BYTE GetFanLimitContinuous( char p )
{
  if ( p < NELEMS(tFanLimitContinuous) ) return tFanLimitContinuous[p];
  else return tFanLimitContinuous[0];
}
//================================================================== 
//   invio ciclico filtro
//==================================================================
const WORD tFilterCyclicSend[] = { // 1 ora
  1, 2, 4, 24, 84, 168
};
//------------------------------------------------------------------
// GetFilterCyclicSend
//------------------------------------------------------------------
WORD GetFilterCyclicSend( char p )
{
  if ( p < NELEMS(tFilterCyclicSend) ) return tFilterCyclicSend[p];
  else return tFilterCyclicSend[0];
}
//================================================================== 
//   Flow Temperature anticondensation
//==================================================================
const int tTemperatureAnticondensation[] = { // 1/100 °C
   1400, 1450, 1500, 1550, 1600, 1650, 1700, 1750, 1800, 1850, 1900,
   1950, 2000
};
//------------------------------------------------------------------
// GetTemperatureAnticondensation
//------------------------------------------------------------------
int GetTemperatureAnticondensation ( char p )
{
  if ( p < NELEMS(tTemperatureAnticondensation) ) return tTemperatureAnticondensation[p];
  else return tTemperatureAnticondensation[0];
}
//================================================================== 
//   CO2 threshold hysteresis
//==================================================================
const int tCO2ThresholdHysteresis[] = { // ppm
   50, 100, 150, 200, 250, 300, 350, 400, 500
};
//------------------------------------------------------------------
// GetCO2ThresholdHysteresis
//------------------------------------------------------------------
int GetCO2ThresholdHysteresis ( char p )
{
  if ( p < NELEMS(tCO2ThresholdHysteresis) ) return tCO2ThresholdHysteresis[p];
  else return tCO2ThresholdHysteresis[0];
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------                                   

