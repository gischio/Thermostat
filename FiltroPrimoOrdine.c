/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "FiltroPrimoOrdine.h"


float yVar[2];
float uVar[2];
float alfaF;
float betaF;
float gammaF;
float aGain,mtau,tSampl;
BYTE iStart;

void CalcFilt(void);

//--------------------------------------------------------------------
// CFilterInput
//--------------------------------------------------------------------
void CFilterInput(float Ts, float tau)
{
  yVar[0]=0.0;yVar[1]=0.0;
  uVar[0]=0.0;uVar[1]=0.0;
  tSampl = Ts;
  aGain = 1.0;
  alfaF = aGain * tSampl/(2*mtau+tSampl);
  betaF = aGain * tSampl/(2*mtau+tSampl);
  gammaF = (tSampl-2*mtau)/(2*mtau+tSampl);
}
//--------------------------------------------------------------------
// SetNewValue
//--------------------------------------------------------------------
void SetNewValue(float dVal)
{
  uVar[0]=dVal;
  CalcFilt();
}
//--------------------------------------------------------------------
// GetFilteredValue
//--------------------------------------------------------------------
float GetFilteredValue(void)
{
  return yVar[0];
}
//--------------------------------------------------------------------
// CalcFilt
//--------------------------------------------------------------------
void CalcFilt(void)
{
  if (iStart<2)
  {
      yVar[0]=uVar[0];
      iStart++;
  }
  else
  {
      yVar[0]=alfaF*uVar[0]+betaF*uVar[1]-gammaF*yVar[1];
  }

  yVar[1]=yVar[0];
  uVar[1]=uVar[0];
}

//--------------------------------------------------------------------
// Start
//--------------------------------------------------------------------
void Start(void)
{
  iStart=0;
  yVar[0]=0.0;yVar[1]=0.0;
  uVar[0]=0.0;uVar[1]=0.0;
}
//--------------------------------------------------------------------
// FiltroPrimoOrdineInit
//--------------------------------------------------------------------
void FiltroPrimoOrdineInit(void)
{
  CFilterInput(60,600);
  Start();
}

//--------------------------------------------------------------------
// FiltroPrimoOrdine
//--------------------------------------------------------------------
void FiltroPrimoOrdine(void)
{
  SetNewValue(0);
  GetFilteredValue();
}
