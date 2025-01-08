#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include "ApplicationTimers.h"

TIME_ELAPSED tm;

/* Modifica 22/09/2016 :
  modificato unsigned char timerNr in unsigned int timerNr
*/

//------------------------------------------------------------------
// CyclicStartTimer: activate cyclic timer checking if enabled (>0) and minimal timing
//------------------------------------------------------------------
void CyclicStartTimer( unsigned int timerNr, unsigned short time )
{
    if ( time == 0 )
        return;                         // timer disabled
    if ( time < MIN_CYCLIC_TIME )
        time = MIN_CYCLIC_TIME;         // minimal time
    EZ_StartTimer( timerNr, time, TM_MODE_1S );
}                                             
//-------------------------------------------------------------------
// NI_StartTimer
// time -> massimo valore (0x7FFFFFFFL - 1)
//-------------------------------------------------------------------
void NI_StartTimer(unsigned int timerNr, unsigned long time, unsigned char mode)
{
    unsigned long counter = TM_GetSysTime();
  
    if ( timerNr >= NI_TIMER_NR )
        return;
    NI_tmData[timerNr].finalTime = (counter+time);
    if ( NI_tmData[timerNr].finalTime == 0 )
        NI_tmData[timerNr].finalTime += 1;
}
//-------------------------------------------------------------------
// NI_StopTimer
//-------------------------------------------------------------------
void NI_StopTimer(unsigned int timerNr)
{
    if ( timerNr >= NI_TIMER_NR )
        return;
    NI_tmData[timerNr].finalTime = 0;
}
//-------------------------------------------------------------------
// NI_GetState
//-------------------------------------------------------------------
TA_BOOL NI_GetState(unsigned int timerNr)
{
    unsigned long deltaT;
    unsigned long counter = TM_GetSysTime();

    if ( timerNr >= NI_TIMER_NR )
        return FALSE;
    if ( NI_tmData[timerNr].finalTime == 0 )
        return FALSE;
    deltaT = (NI_tmData[timerNr].finalTime-counter);
    if ( deltaT > 0x7FFFFFFFL || deltaT == 0 )
    {
        NI_StopTimer(timerNr);
        return TRUE;
    }
    return FALSE;
}
//-------------------------------------------------------------------
// Long timer manager, time base 1ms
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// EZ_StartTimer
//-------------------------------------------------------------------
void EZ_StartTimer( unsigned int timerNr, unsigned long time, unsigned char mode )
{
    
    if ( timerNr >= EZ_TIMER_NR )
        return;
    EZ_tmData[timerNr].finalTime = (TM_GetSysTime()/*counter*/+((mode==TM_MODE_1S) ? (time*1000L) : time));
    if ( EZ_tmData[timerNr].finalTime == 0 )
        EZ_tmData[timerNr].finalTime += 1;
}                                               
//-------------------------------------------------------------------
// EZ_StopTimer
//-------------------------------------------------------------------
void EZ_StopTimer( unsigned int timerNr )
{
  
    if ( timerNr >= EZ_TIMER_NR )
        return;
    EZ_tmData[timerNr].finalTime = 0;
}                                               
//-------------------------------------------------------------------
// EZ_GetState ( stop timer if elapsed )
//-------------------------------------------------------------------
TA_BOOL EZ_GetState( unsigned int timerNr )
{
    unsigned long deltaT;
    
    if ( timerNr >= EZ_TIMER_NR )
        return FALSE;
    if ( EZ_tmData[timerNr].finalTime == 0 )
        return FALSE;
    deltaT = (EZ_tmData[timerNr].finalTime-TM_GetSysTime()/*counter*/);
    if ( deltaT > 0x7FFFFFFFL || deltaT == 0  )
    {
        EZ_StopTimer(timerNr);
        return TRUE;
    }
    return FALSE;
}                                              
//-------------------------------------------------------------------
// EZ_GetInfo
//-------------------------------------------------------------------
TA_BOOL EZ_GetInfo(unsigned int timerNr, unsigned long *remainingTime)
{
    unsigned long deltaT;
    
    if ( remainingTime != NULL )
        *remainingTime = 0;
    if ( timerNr >= EZ_TIMER_NR )
        return FALSE;
    if ( EZ_tmData[timerNr].finalTime == 0 )
        return FALSE;
    deltaT = (EZ_tmData[timerNr].finalTime-TM_GetSysTime()/*counter*/);
    if ( deltaT == 0 || deltaT <= 0x7FFFFFFFL )
    {
        if ( remainingTime != NULL )
            *remainingTime = deltaT;
        return TRUE;
    }
    return FALSE;
}                                               
//-------------------------------------------------------------------
// EZ_GetInfoRunning
//-------------------------------------------------------------------
TA_BOOL EZ_GetInfoRunning( unsigned int timerNr)
{
  
    if ( timerNr >= EZ_TIMER_NR )
        return FALSE;
    if ( EZ_tmData[timerNr].finalTime == 0 )
        return FALSE;
    
    return TRUE;
} 
//-------------------------------------------------------------------
// EZ_StartNotRunningTimer
//-------------------------------------------------------------------
TA_BOOL EZ_StartNotRunningTimer( unsigned int timerNr, unsigned long time, unsigned char mode )
{
    
    if (( timerNr >= EZ_TIMER_NR )||( EZ_tmData[timerNr].finalTime != 0 ))
        return FALSE;

    EZ_tmData[timerNr].finalTime = (TM_GetSysTime()/*counter*/+((mode==TM_MODE_1S) ? (time*1000L) : time));
    if ( EZ_tmData[timerNr].finalTime == 0 )
        EZ_tmData[timerNr].finalTime += 1;

    return TRUE; 
} 

//-------------------------------------------------------------------
// TimeElapsed();
//-------------------------------------------------------------------
BYTE TimeElapsed(void)
{
  unsigned long delta;  
  unsigned long counter;
  static unsigned long last1min = 0;  
  static unsigned long last100msec = 0;    
  static unsigned long last250msec = 0;    
  static unsigned long last500msec = 0;    
  static unsigned long last1sec = 0;  
  static unsigned long last10sec = 0; 
  

  counter = TM_GetSysTime();
  delta = counter - last100msec;
  if (delta > 100) 
  {
      last100msec = counter;
      tm.elapsed100msec = YES;  
      tm.reverse100msec = !tm.reverse100msec;
  }     
  counter = TM_GetSysTime();
  delta = counter - last250msec;
  if (delta > 250) 
  {
      last250msec = counter;
      tm.elapsed250msec = YES;  
      tm.reverse250msec = !tm.reverse250msec;
  }    
  counter = TM_GetSysTime();
  delta = counter - last500msec;
  if (delta > 500) 
  {
      last500msec = counter;
      tm.elapsed500msec = YES;  
      tm.reverse500msec = !tm.reverse500msec;
  }  
  counter = TM_GetSysTime();
  delta = counter - last1sec;
  if (delta > 1000) 
  {
      last1sec = counter;
      tm.elapsed1sec = YES;  
      tm.reverse1sec = !tm.reverse1sec;
  }
  counter = TM_GetSysTime();
  delta = counter - last10sec;
  if (delta > 10000) 
  {
      last10sec = counter;
      tm.elapsed10sec = YES;  
  }    
  counter = TM_GetSysTime();
  delta = counter - last1min;
#ifdef SPEED_1MIN   
  if (delta > 6) 
#else      
  if (delta > 60000) 
#endif        
  {   //TM_counter1ms
      last1min = counter;
      tm.elapsed1min = YES;  
  }  
  return 0;
}

//-------------------------------------------------------------------
// TimeElapsedFlagsClear();
//-------------------------------------------------------------------
void TimeElapsedFlagsClear(void)
{
  tm.timeElapsed = 0;
}