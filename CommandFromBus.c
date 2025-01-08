/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------
#include <string.h>
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include "ListValues.h"
#include "ApplicationTimers.h"
#include "AlarmsAndErrors.h"
#include "CommandFromBus.h"

enum token_t 
{ 
  RA,   // read alarms
  RC,   // read communication object
  RV,   // read fw version
  END 
} ;

static struct entry_s {
    const char *key;
    enum token_t token;
} token_table[] = {
    { "RA", RA },
    { "RC", RC },
    { "RV", RV },    
    { NULL, END },
};



enum token_t lexer(const char *s,unsigned char n)
{
    struct entry_s *p = token_table;
    for(; p->key != NULL && strncmp(p->key, s,n) != 0; ++p);
    return p->token;
}

//-----------------------------------------------------------------------------
// CommandFromBusInit
//-----------------------------------------------------------------------------                                                   
void CommandFromBusInit(void) 
{

}
//-----------------------------------------------------------------------------
// CommandFromBus
//-----------------------------------------------------------------------------                                                   
BYTE CommandFromBus(BYTE nTherm) 
{
  char *coTextError;
  unsigned char coN;
  const char FWVer[] = {FW_VER};
  
  if (CheckUpdateFlag(CO_INX_THERM(nTherm,CO_AlarmText))) 
  {
      coTextError = (char*)_OBJV1(AlarmText,0); //OBJ_VALUE->ThermAlarmText[nTherm];
      if ( strncmp(coTextError,"Cmd",3) == 0 )
      {
          switch ( lexer(&coTextError[3],2) )
          {
          case RA:
              memset(&_V1(errorSended,0),0,sizeof(errorSended));
              break;
          case RC:
              coN = (coTextError[5]-'0')*100 + 
                    (coTextError[6]-'0')*10  +
                    (coTextError[7]-'0');  
              if ( coN < APP_objectTabSize )
              {
                  LUSetTransmitRequest(coN);
              }
              break;              
          case RV:
              memset(coTextError,0,sizeof(_OBJV1(AlarmText,0)));
              strcpy(coTextError, FWVer);
              LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_AlarmText));
              break;          
          }
      }
  }


  return 1;
}