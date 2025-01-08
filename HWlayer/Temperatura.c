/**********************************************************************
* $Id$		Temperatura.c		16-01-2013
*//**
* @file		Temperatura.c
* @brief
* @version	1.0
* @date		16. Gennaio. 2013
* @author	Luca Ottone
*
* Copyright(C) 2012, SBS s.r.l.
* All rights reserved.
*
***********************************************************************/

#include "Temperatura.h"
#include "TabTemperatura.h"

/*!
 * \fn int getNTCTemp(unsigned int Valore)
 *
 * \param[in] valore analogico acquisito.
 * \return valore di temperatora.
 ********************************************************/
int getNTCTemp(unsigned int Conversione)
{
  unsigned char cnt=0;
  int q=0;

  while (Conversione > Tc[cnt])
  {
    cnt++;
    if (cnt > TABLEN)
    {
      return(SONDA_CORTO);
    }
  }
  if (cnt ==0)
  {
    return(SONDA_APERTA);
  }
  cnt--;

  q=(cnt*TSTEP)+PZERO;
  Conversione=Conversione-Tc[cnt];
  Conversione=Conversione*Tm[cnt];
  return((Conversione/100)+q);
}
