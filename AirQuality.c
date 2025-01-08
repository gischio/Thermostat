/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include "ApplicationUtility.h"
#include "AirQuality.h"


//-----------------------------------------------------------------------------
// AirQualityInit
//-----------------------------------------------------------------------------
void AirQualityInit(void)
{
    airQuality = SENSOR_ERROR;
}

//-----------------------------------------------------------------------------
// AirQuality
//-----------------------------------------------------------------------------
BYTE AirQuality (void)
{

    if ( __IsSensorFromBusUpdated(FROMBUS_AIR_QUALITY) )
    {
        if ( __IsSensorFromBusFail(FROMBUS_AIR_QUALITY))
        {
            airQuality = SENSOR_ERROR;
        }
        else
        {
            airQuality = ShortF2LongI(OBJ_VALUE->AirQualityFromBus)/1000;
        }
    }
    
    return 1;
}                                                   


