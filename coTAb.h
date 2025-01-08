//------------------------------------------------------------------
// $Workfile: app_data.c $   
// $Archive: /eib_stk/appl_example/empty/src/app_data.c $     

// $Author: adler_k $         
// 
// Copyright Tapko Technologies GmbH
// 
// $Date: 2013-07-18 22:00:49 +0200 (Do, 18 Jul 2013) $
// $Revision: 529 $         
//------------------------------------------------------------------

#ifdef TEST_CE
#define KNX_DEF_INDIVIDUAL_ADDR EIB_ADDR(0x1F1F)
#define KNX_CURRENT_ADDR_TAB_LEN  135
#define KNX_CURRENT_ASSOC_TAB_LEN 135
#else
#define KNX_DEF_INDIVIDUAL_ADDR EIB_ADDR(0xFFFF)
#define KNX_CURRENT_ADDR_TAB_LEN  1
#define KNX_CURRENT_ASSOC_TAB_LEN 0
#endif
//------------------------------------------------------------------
// this block is used for generation of documentation
//------------------------------------------------------------------
#ifdef KAI_GENERATE_DOC
/**
This makro is used to define the addresses, association, communication objects and enums for the communication objects.
The order of the makros defines the order of the group communication objects.
\n There are restictions on the associated addresses. 
There is a 1 to 1 assignmet for group addresses to the group communication objects.
The group addresses has to be in ascending order.

@param index  name for the numeration of the group communication object
@param groupAddr group address for this communication object
@param dataPtr pointer to the data of the communication object
@param objType type of the communication object
@param DPT data point type (DPT) of the communication object
@param flags configuration flags of the communication object
@param priority priority of the communication object0

@code 
DEFINE_COMM_OBJ(CO_in0,  ETS_GROUP3(2,0,0), RAM_PTR(in0), CO_TypeUint1, 1, CO_WU, CO_PRIO_L),
@endcode 
*/
#define DEFINE_COMM_OBJ(index,  groupAddr, dataPtr, objType, DPT, flags, priority),
#endif

#define CO_TWU    (CO_commEnable | CO_transmitEnable | CO_writeEnable | CO_readResponseEnable)

START_TAB()

#if BUILD_APP == EK_ET2
    #include "cotabCE2.h"
#endif




#define	CO_TempValue_1				  CO_TempValue
#define	CO_TempThreshold1Switch_1	          CO_TempThreshold1Switch
#define	CO_TempThreshold2Switch_1		  CO_TempThreshold2Switch
#define	CO_RoomTempFB_1			          CO_RoomTempFB
#define	CO_Humidity2bytesFB_1			  CO_Humidity2bytesFB
#define	CO_Humidity1byteFB_1		          CO_Humidity1byteFB
#define	CO_AntistratificationTempFB_1	          CO_AntistratificationTempFB
#define	CO_OutdoorTempFB_1		          CO_OutdoorTempFB
#define	CO_CoilTempFB_1				  CO_CoilTempFB
#define	CO_FloorTempFB_1			  CO_FloorTempFB
#define	CO_FlowTempFB_1				  CO_FlowTempFB
#define	CO_WindowsContactSensor1FB_1		  CO_WindowsContactSensor1FB
#define	CO_WindowsContactSensor2FB_1		  CO_WindowsContactSensor2FB
#define	CO_PresenceSensor1FB_1		          CO_PresenceSensor1FB
#define	CO_PresenceSensor2FB_1			  CO_PresenceSensor2FB
#define	CO_ContactHolderFB_1			  CO_ContactHolderFB
#define	CO_AnticondensationFB_1			  CO_AnticondensationFB
#define	CO_WeightedTemp_1			  CO_WeightedTemp
#define	CO_HeatingCoolingStatusOut_1		  CO_HeatingCoolingStatusOut
#define	CO_HeatingCoolingStatusIn_1		  CO_HeatingCoolingStatusIn
#define	CO_HVACModeIn_1				  CO_HVACModeIn
#define	CO_HVACForcedModeIn_1			  CO_HVACForcedModeIn
#define	CO_HVACModeOut_1			  CO_HVACModeOut
#define	CO_HVACManualMode_1			  CO_HVACManualMode
#define	CO_ChronoModeStatus_1			  CO_ChronoModeStatus
#define	CO_ActualSetpoint_1			  CO_ActualSetpoint
#define	CO_ManualSetpoint_1			  CO_ManualSetpoint
#define	CO_SetpointComfort_1_1			  CO_SetpointComfort_1
#define	CO_SetpointComfort_1_2			  CO_SetpointComfort_2
#define	CO_SetpointStandby_1_1			  CO_SetpointStandby_1
#define	CO_SetpointStandby_1_2			  CO_SetpointStandby_2
#define	CO_SetpointEconomy_1_1			  CO_SetpointEconomy_1
#define	CO_SetpointEconomy_1_2			  CO_SetpointEconomy_2
#define	CO_SetpointBuildProtection_1_1		  CO_SetpointBuildProtection_1
#define	CO_SetpointBuildProtection_1_2		  CO_SetpointBuildProtection_2
#define	CO_RoomTempControllerStatus_1		  CO_RoomTempControllerStatus
#define	CO_HeatingOut_1				  CO_HeatingOut
#define	CO_CoolingOut_1				  CO_CoolingOut
#define	CO_AuxiliaryHeatingOut_1		  CO_AuxiliaryHeatingOut
#define	CO_AuxiliaryCoolingOut_1		  CO_AuxiliaryCoolingOut
#define	CO_AuxiliaryHeatingDisable_1		  CO_AuxiliaryHeatingDisable
#define	CO_AuxiliaryCoolingDisable_1		  CO_AuxiliaryCoolingDisable
#define	CO_FanContinuousSpeed_1			  CO_FanContinuousSpeed
#define	CO_FanSpeed_1_1				  CO_FanSpeed_1
#define	CO_FanSpeed_1_2				  CO_FanSpeed_2
#define	CO_FanSpeed_1_3				  CO_FanSpeed_3
#define	CO_FanControlDisable_1			  CO_FanControlDisable
#define	CO_ThermAlarmText_1			  CO_ThermAlarmText
#define	CO_ManualModeStatus_1			  CO_ManualSetpointStatus
#define	CO_FanManualStepSpeed_1			  CO_FanManualSpeedStep
#define	CO_FanStepSpeed_1			  CO_FanStepSpeed
#define	CO_VentManualOperation_1		  CO_FanManualOperation
#define CO_CO_FanManualSpeedPercentage_1          CO_FanManualSpeedPercentage
#define	CO_TempControllerAlarm_1		  CO_TempControllerAlarm
#define	CO_DisableTempController_1		  CO_DisableTempController
#define	CO_DewPointTemp_1			  CO_DewPointTemp
#define	CO_HumiditySetpointForDehum_1		  CO_HumiditySetpointForDehum
#define	CO_HumiditySetpointForHum_1		  CO_HumiditySetpointForHum
#define	CO_DehumCommand_1			  CO_DehumCommand
#define	CO_DehumWaterBatteryCommand_1		  CO_DehumWaterBatteryCommand
#define	CO_DehumIntegrationControl_1		  CO_DehumIntegrationControl
#define	CO_DehumControlDisable_1		  CO_DehumControlDisable
#define	CO_HumidCommand_1			  CO_HumidCommand
#define	CO_HumidControlDisable_1		  CO_HumidControlDisable
#define	CO_AnticondensationAlarm_1		  CO_AnticondensationAlarm
#define	CO_ThermalGeneratorLock_1		  CO_ThermalGeneratorLock
#define	CO_HVACProtectionModeStatus_1		  CO_HVACProtectionModeStatus
#define	CO_FanManualSpeedPercentage_1		  CO_FanManualSpeedPercentage
#define	CO_FanManualOffStatus_1			  CO_FanManualOffStatus

#define CO_FanStepManualSpeed_1                   CO_FanManualSpeedStep
#define FanStepManualSpeed                        FanManualSpeedStep



