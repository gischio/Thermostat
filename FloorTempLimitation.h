/*
 * @brief ....
 *
 * @note
 * Copyright(C) SBS, 2014
 * All rights reserved.
 *
 * @par
 * fdsfsdfsdf
 * @par
 * fdfsdfsdfsd
 */
//------------------------------------------------------------------
#ifndef FLOORTEMPLIMITATION_H
#define FLOORTEMPLIMITATION_H

#define FLOOR_TEMP_LIMIT_ENABLE(x)    ( ( (_PARTHERM(x).HeatingType == FLOOR_RADIANT)|| \
                                          (_PARTHERM(x).HeatingType == ELECTRIC) )&& \
                                          (_PARTHERM(x).FloorTempFromBus != DISABLED) )

typedef struct {
  BYTE          heatingTempLimitActivated   :1; ///< stato , attivo o non attivo , della funzione limitazione temperatura pavimento
} TYPE_FLOORTEMPLIMIT;

extern long _V1(FloorTemp,MAX_THERM);
extern TYPE_FLOORTEMPLIMIT _V1(ftc,MAX_THERM);

/**
 * @brief	
 * @return	nd
 */
BYTE            FloorTemperatureLimitation (BYTE nTherm);



#endif // FLOORTEMPLIMITATION_H