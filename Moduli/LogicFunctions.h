/**
 * @brief ....
 *
 * @note
 * Copyright(C) SBS, 2014
 * All rights reserved.
 *
 * @par
 * @addtogroup 
 * @{
 */
#ifndef LOGICFUNCTIONS_H
#define LOGICFUNCTIONS_H

#define LOBJ_IN_idx(lf,lobj)   (CO_LogicFunctionIn_1_1 + (lf*(MAX_LOGIC_OBJECTS+1)) + lobj) 
#define LOBJ_OUT_idx(lf)       (CO_LogicFunctionOut_1 + (lf*(MAX_LOGIC_OBJECTS+1))) 

#define LOGIC_OPERATION_TYPE_OR     0x00
#define LOGIC_OPERATION_TYPE_AND    0x01
#define LOGIC_OPERATION_TYPE_XOR    0x02

#define LF_OUT_ALL                              0
#define LF_OUTONLY_0                            1
#define LF_OUTONLY_1                            2

typedef enum  {
  LF_INIT_BUS_ON_DELAY = 0,
  LF_WAIT_BUS_ON_DELAY,
  LF_INIT,
  LF_WAIT_UPDATE,
  LF_PREREADY,
  LF_READY,
  LF_DISABLED
} LF_STATUS;

/**
 * @brief	
 * @param	
 * @return	
 * @note        
 *   
 */
void              LogicFunctionsInit(void);

/**
 * @brief	
 * @param	
 * @return	
 * @note        
 *   
 */
void              LogicFunctions(void);


/**
 * @}
 */
#endif // LOGICFUNCTIONS_H