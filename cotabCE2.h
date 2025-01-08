/* << START COPY */

/* [data: 05/04/2018 10.21.35] 
   SYSTEM 0700          */


DEFINE_COMM_OBJ(CO_TechnicalAlarm                            ,ETS_GROUP3(1,0,0)     ,RAM_PTR(TechnicalAlarm)                           ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LedsIntensityFB                           ,ETS_GROUP3(1,0,1)     ,RAM_PTR(LedsIntensityFB)                          ,CO_TypeUint8   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_TempValue                                 ,ETS_GROUP3(1,0,2)     ,RAM_PTR(TempValue)                                ,CO_TypeByte2   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_TempThreshold1Switch                      ,ETS_GROUP3(1,0,3)     ,RAM_PTR(TempThreshold1Switch)                     ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_TempThreshold1Lock                        ,ETS_GROUP3(1,0,4)     ,RAM_PTR(TempThreshold1Lock)                       ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_TempThreshold1Value                       ,ETS_GROUP3(1,0,5)     ,RAM_PTR(TempThreshold1Value)                      ,CO_TypeByte2   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_TempThreshold2Switch                      ,ETS_GROUP3(1,0,6)     ,RAM_PTR(TempThreshold2Switch)                     ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_TempThreshold2Lock                        ,ETS_GROUP3(1,0,7)     ,RAM_PTR(TempThreshold2Lock)                       ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_TempThreshold2Value                       ,ETS_GROUP3(1,0,8)     ,RAM_PTR(TempThreshold2Value)                      ,CO_TypeByte2   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CO2Value                                  ,ETS_GROUP3(1,0,9)     ,RAM_PTR(CO2Value)                                 ,CO_TypeByte2   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhValue_2bytes                            ,ETS_GROUP3(1,0,10)    ,RAM_PTR(RhValue_2bytes)                           ,CO_TypeByte2   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhValue_1byte                             ,ETS_GROUP3(1,0,11)    ,RAM_PTR(RhValue_1byte)                            ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhThreshold1Switch                        ,ETS_GROUP3(1,0,12)    ,RAM_PTR(RhThreshold1Switch)                       ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhThreshold1Lock                          ,ETS_GROUP3(1,0,13)    ,RAM_PTR(RhThreshold1Lock)                         ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhThreshold1_2bytesValue                  ,ETS_GROUP3(1,0,14)    ,RAM_PTR(RhThreshold1_2bytesValue)                 ,CO_TypeByte2   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhThreshold1_1byteValue                   ,ETS_GROUP3(1,0,15)    ,RAM_PTR(RhThreshold1_1byteValue)                  ,CO_TypeUint8   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhThreshold2Switch                        ,ETS_GROUP3(1,0,16)    ,RAM_PTR(RhThreshold2Switch)                       ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhThreshold2Lock                          ,ETS_GROUP3(1,0,17)    ,RAM_PTR(RhThreshold2Lock)                         ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhThreshold2_2bytesValue                  ,ETS_GROUP3(1,0,18)    ,RAM_PTR(RhThreshold2_2bytesValue)                 ,CO_TypeByte2   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RhThreshold2_1byteValue                   ,ETS_GROUP3(1,0,19)    ,RAM_PTR(RhThreshold2_1byteValue)                  ,CO_TypeUint8   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RoomTempFB                                ,ETS_GROUP3(1,0,20)    ,RAM_PTR(RoomTempFB)                               ,CO_TypeByte2   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_Humidity2bytesFB                          ,ETS_GROUP3(1,0,21)    ,RAM_PTR(Humidity2bytesFB)                         ,CO_TypeByte2   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_Humidity1byteFB                           ,ETS_GROUP3(1,0,22)    ,RAM_PTR(Humidity1byteFB)                          ,CO_TypeUint8   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_AntistratificationTempFB                  ,ETS_GROUP3(1,0,23)    ,RAM_PTR(AntistratificationTempFB)                 ,CO_TypeByte2   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_OutdoorTempFB                             ,ETS_GROUP3(1,0,24)    ,RAM_PTR(OutdoorTempFB)                            ,CO_TypeByte2   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CoilTempFB                                ,ETS_GROUP3(1,0,25)    ,RAM_PTR(CoilTempFB)                               ,CO_TypeByte2   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FloorTempFB                               ,ETS_GROUP3(1,0,26)    ,RAM_PTR(FloorTempFB)                              ,CO_TypeByte2   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FlowTempFB                                ,ETS_GROUP3(1,0,27)    ,RAM_PTR(FlowTempFB)                               ,CO_TypeByte2   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_WindowsContactSensor1FB                   ,ETS_GROUP3(1,0,28)    ,RAM_PTR(WindowsContactSensor1FB)                  ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_WindowsContactSensor2FB                   ,ETS_GROUP3(1,0,29)    ,RAM_PTR(WindowsContactSensor2FB)                  ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_PresenceSensor1FB                         ,ETS_GROUP3(1,0,30)    ,RAM_PTR(PresenceSensor1FB)                        ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_PresenceSensor2FB                         ,ETS_GROUP3(1,0,31)    ,RAM_PTR(PresenceSensor2FB)                        ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_ContactHolderFB                           ,ETS_GROUP3(1,0,32)    ,RAM_PTR(ContactHolderFB)                          ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_AnticondensationFB                        ,ETS_GROUP3(1,0,33)    ,RAM_PTR(AnticondensationFB)                       ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_WeightedTemp                              ,ETS_GROUP3(1,0,34)    ,RAM_PTR(WeightedTemp)                             ,CO_TypeByte2   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HeatingCoolingStatusOut                   ,ETS_GROUP3(1,0,35)    ,RAM_PTR(HeatingCoolingStatusOut)                  ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HeatingCoolingStatusIn                    ,ETS_GROUP3(1,0,36)    ,RAM_PTR(HeatingCoolingStatusIn)                   ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HVACModeIn                                ,ETS_GROUP3(1,0,37)    ,RAM_PTR(HVACModeIn)                               ,CO_TypeUint8   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HVACForcedModeIn                          ,ETS_GROUP3(1,0,38)    ,RAM_PTR(HVACForcedModeIn)                         ,CO_TypeUint8   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HVACModeOut                               ,ETS_GROUP3(1,0,39)    ,RAM_PTR(HVACModeOut)                              ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HVACManualMode                            ,ETS_GROUP3(1,0,40)    ,RAM_PTR(HVACManualMode)                           ,CO_TypeUint8   ,1    ,CO_RTWU   ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HVACProtectionModeStatus                  ,ETS_GROUP3(1,0,41)    ,RAM_PTR(HVACProtectionModeStatus)                 ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_ChronoModeStatus                          ,ETS_GROUP3(1,0,42)    ,RAM_PTR(ChronoModeStatus)                         ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_ActualSetpoint                            ,ETS_GROUP3(1,0,43)    ,RAM_PTR(ActualSetpoint)                           ,CO_TypeByte2   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_ManualSetpoint                            ,ETS_GROUP3(1,0,44)    ,RAM_PTR(ManualSetpoint)                           ,CO_TypeByte2   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_ManualSetpointStatus                      ,ETS_GROUP3(1,0,45)    ,RAM_PTR(ManualSetpointStatus)                     ,CO_TypeUint1   ,1    ,CO_RTWU   ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_SetpointComfort_1                         ,ETS_GROUP3(1,0,46)    ,RAM_PTR(SetpointComfort               [0])        ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
// alias => SetpointIn
DEFINE_COMM_OBJ(CO_SetpointComfort_2                         ,ETS_GROUP3(1,0,47)    ,RAM_PTR(SetpointComfort               [1])        ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_SetpointStandby_1                         ,ETS_GROUP3(1,0,48)    ,RAM_PTR(SetpointStandby               [0])        ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
// alias => OffsetStandby_1
DEFINE_COMM_OBJ(CO_SetpointStandby_2                         ,ETS_GROUP3(1,0,49)    ,RAM_PTR(SetpointStandby               [1])        ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
// alias => OffsetStandby_2
DEFINE_COMM_OBJ(CO_SetpointEconomy_1                         ,ETS_GROUP3(1,0,50)    ,RAM_PTR(SetpointEconomy               [0])        ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
// alias => OffsetEconomy_1
DEFINE_COMM_OBJ(CO_SetpointEconomy_2                         ,ETS_GROUP3(1,0,51)    ,RAM_PTR(SetpointEconomy               [1])        ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
// alias => OffsetEconomy_2
DEFINE_COMM_OBJ(CO_SetpointBuildProtection_1                 ,ETS_GROUP3(1,0,52)    ,RAM_PTR(SetpointBuildProtection       [0])        ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_SetpointBuildProtection_2                 ,ETS_GROUP3(1,0,53)    ,RAM_PTR(SetpointBuildProtection       [1])        ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_DisableTempController                     ,ETS_GROUP3(1,0,54)    ,RAM_PTR(DisableTempController)                    ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_RoomTempControllerStatus                  ,ETS_GROUP3(1,0,55)    ,RAM_PTR(RoomTempControllerStatus)                 ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HeatingOut                                ,ETS_GROUP3(1,0,56)    ,RAM_PTR(HeatingOut)                               ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
// alias => HeatingOut1bit
// alias => HeatingCoolingOut1byte
// alias => HeatingCoolingOut1bit
DEFINE_COMM_OBJ(CO_CoolingOut                                ,ETS_GROUP3(1,0,57)    ,RAM_PTR(CoolingOut)                               ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
// alias => CoolingOut1bit
DEFINE_COMM_OBJ(CO_AuxiliaryHeatingOut                       ,ETS_GROUP3(1,0,58)    ,RAM_PTR(AuxiliaryHeatingOut)                      ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
// alias => AuxiliaryHeatingCoolingOut
DEFINE_COMM_OBJ(CO_AuxiliaryCoolingOut                       ,ETS_GROUP3(1,0,59)    ,RAM_PTR(AuxiliaryCoolingOut)                      ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_AuxiliaryHeatingDisable                   ,ETS_GROUP3(1,0,60)    ,RAM_PTR(AuxiliaryHeatingDisable)                  ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_AuxiliaryCoolingDisable                   ,ETS_GROUP3(1,0,61)    ,RAM_PTR(AuxiliaryCoolingDisable)                  ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanContinuousSpeed                        ,ETS_GROUP3(1,0,62)    ,RAM_PTR(FanContinuousSpeed)                       ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanSpeed_1                                ,ETS_GROUP3(1,0,63)    ,RAM_PTR(FanSpeed                      [0])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanSpeed_2                                ,ETS_GROUP3(1,0,64)    ,RAM_PTR(FanSpeed                      [1])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanSpeed_3                                ,ETS_GROUP3(1,0,65)    ,RAM_PTR(FanSpeed                      [2])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanControlDisable                         ,ETS_GROUP3(1,0,66)    ,RAM_PTR(FanControlDisable)                        ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanStepSpeed                              ,ETS_GROUP3(1,0,67)    ,RAM_PTR(FanStepSpeed)                             ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanManualSpeedStep                        ,ETS_GROUP3(1,0,68)    ,RAM_PTR(FanManualSpeedStep)                       ,CO_TypeUint8   ,1    ,CO_RTWU   ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanManualSpeedPercentage                  ,ETS_GROUP3(1,0,69)    ,RAM_PTR(FanManualSpeedPercentage)                 ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanManualOperation                        ,ETS_GROUP3(1,0,70)    ,RAM_PTR(FanManualOperation)                       ,CO_TypeUint1   ,1    ,CO_RTWU   ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_FanManualOffStatus                        ,ETS_GROUP3(1,0,71)    ,RAM_PTR(FanManualOffStatus)                       ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_TempControllerAlarm                       ,ETS_GROUP3(1,0,72)    ,RAM_PTR(TempControllerAlarm)                      ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_DewPointTemp                              ,ETS_GROUP3(1,0,73)    ,RAM_PTR(DewPointTemp)                             ,CO_TypeByte2   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HumiditySetpointForDehum                  ,ETS_GROUP3(1,0,74)    ,RAM_PTR(HumiditySetpointForDehum)                 ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HumiditySetpointForHum                    ,ETS_GROUP3(1,0,75)    ,RAM_PTR(HumiditySetpointForHum)                   ,CO_TypeByte2   ,1    ,CO_RTWU   ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_DehumCommand                              ,ETS_GROUP3(1,0,76)    ,RAM_PTR(DehumCommand)                             ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_DehumWaterBatteryCommand                  ,ETS_GROUP3(1,0,77)    ,RAM_PTR(DehumWaterBatteryCommand)                 ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_DehumIntegrationControl                   ,ETS_GROUP3(1,0,78)    ,RAM_PTR(DehumIntegrationControl)                  ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_DehumControlDisable                       ,ETS_GROUP3(1,0,79)    ,RAM_PTR(DehumControlDisable)                      ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HumidCommand                              ,ETS_GROUP3(1,0,80)    ,RAM_PTR(HumidCommand)                             ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HumidControlDisable                       ,ETS_GROUP3(1,0,81)    ,RAM_PTR(HumidControlDisable)                      ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_AnticondensationAlarm                     ,ETS_GROUP3(1,0,82)    ,RAM_PTR(AnticondensationAlarm)                    ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_ThermalGeneratorLock                      ,ETS_GROUP3(1,0,83)    ,RAM_PTR(ThermalGeneratorLock)                     ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_HVACScene                                 ,ETS_GROUP3(1,0,84)    ,RAM_PTR(HVACScene)                                ,CO_TypeUint8   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_AlarmText                                 ,ETS_GROUP3(1,0,85)    ,RAM_PTR(AlarmText)                                ,CO_TypeByte14  ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CO2Treshold1Percentage                    ,ETS_GROUP3(1,0,86)    ,RAM_PTR(CO2Treshold1Percentage)                   ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
// alias => CO2Treshold1Counter
// alias => CO2Treshold1Switch
DEFINE_COMM_OBJ(CO_CO2Threshold1Lock                         ,ETS_GROUP3(1,0,87)    ,RAM_PTR(CO2Threshold1Lock)                        ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CO2Treshold2Percentage                    ,ETS_GROUP3(1,0,88)    ,RAM_PTR(CO2Treshold2Percentage)                   ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
// alias => CO2Treshold2Counter
// alias => CO2Treshold2Switch
DEFINE_COMM_OBJ(CO_CO2Threshold2Lock                         ,ETS_GROUP3(1,0,89)    ,RAM_PTR(CO2Threshold2Lock)                        ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CO2Treshold3Percentage                    ,ETS_GROUP3(1,0,90)    ,RAM_PTR(CO2Treshold3Percentage)                   ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
// alias => CO2Treshold3Counter
// alias => CO2Treshold3Switch
DEFINE_COMM_OBJ(CO_CO2Threshold3Lock                         ,ETS_GROUP3(1,0,91)    ,RAM_PTR(CO2Threshold3Lock)                        ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CO2VentilationPercentage                  ,ETS_GROUP3(1,0,92)    ,RAM_PTR(CO2VentilationPercentage)                 ,CO_TypeUint8   ,1    ,CO_RT     ,CO_PRIO_L )
// alias => CO2VentilationCounter
// alias => CO2VentilationBit1
DEFINE_COMM_OBJ(CO_CO2VentilationBit2                        ,ETS_GROUP3(1,0,93)    ,RAM_PTR(CO2VentilationBit2            )           ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CO2VentilationBit3                        ,ETS_GROUP3(1,0,94)    ,RAM_PTR(CO2VentilationBit3            )           ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CO2VentilationBit4                        ,ETS_GROUP3(1,0,95)    ,RAM_PTR(CO2VentilationBit4            )           ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CO2VentilationLock                        ,ETS_GROUP3(1,0,96)    ,RAM_PTR(CO2VentilationLock)                       ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_CO2VentilationDisableFB                   ,ETS_GROUP3(1,0,97)    ,RAM_PTR(CO2VentilationDisableFB)                  ,CO_TypeUint1   ,1    ,CO_W      ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_1_1                       ,ETS_GROUP3(1,0,98)    ,RAM_PTR(LogicFunctionIn               [0][0])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_1_2                       ,ETS_GROUP3(1,0,99)    ,RAM_PTR(LogicFunctionIn               [0][1])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_1_3                       ,ETS_GROUP3(1,0,100)   ,RAM_PTR(LogicFunctionIn               [0][2])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_1_4                       ,ETS_GROUP3(1,0,101)   ,RAM_PTR(LogicFunctionIn               [0][3])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionOut_1                        ,ETS_GROUP3(1,0,102)   ,RAM_PTR(LogicFunctionOut              [0])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_2_1                       ,ETS_GROUP3(1,0,103)   ,RAM_PTR(LogicFunctionIn               [1][0])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_2_2                       ,ETS_GROUP3(1,0,104)   ,RAM_PTR(LogicFunctionIn               [1][1])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_2_3                       ,ETS_GROUP3(1,0,105)   ,RAM_PTR(LogicFunctionIn               [1][2])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_2_4                       ,ETS_GROUP3(1,0,106)   ,RAM_PTR(LogicFunctionIn               [1][3])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionOut_2                        ,ETS_GROUP3(1,0,107)   ,RAM_PTR(LogicFunctionOut              [1])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_3_1                       ,ETS_GROUP3(1,0,108)   ,RAM_PTR(LogicFunctionIn               [2][0])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_3_2                       ,ETS_GROUP3(1,0,109)   ,RAM_PTR(LogicFunctionIn               [2][1])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_3_3                       ,ETS_GROUP3(1,0,110)   ,RAM_PTR(LogicFunctionIn               [2][2])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_3_4                       ,ETS_GROUP3(1,0,111)   ,RAM_PTR(LogicFunctionIn               [2][3])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionOut_3                        ,ETS_GROUP3(1,0,112)   ,RAM_PTR(LogicFunctionOut              [2])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_4_1                       ,ETS_GROUP3(1,0,113)   ,RAM_PTR(LogicFunctionIn               [3][0])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_4_2                       ,ETS_GROUP3(1,0,114)   ,RAM_PTR(LogicFunctionIn               [3][1])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_4_3                       ,ETS_GROUP3(1,0,115)   ,RAM_PTR(LogicFunctionIn               [3][2])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_4_4                       ,ETS_GROUP3(1,0,116)   ,RAM_PTR(LogicFunctionIn               [3][3])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionOut_4                        ,ETS_GROUP3(1,0,117)   ,RAM_PTR(LogicFunctionOut              [3])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_5_1                       ,ETS_GROUP3(1,0,118)   ,RAM_PTR(LogicFunctionIn               [4][0])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_5_2                       ,ETS_GROUP3(1,0,119)   ,RAM_PTR(LogicFunctionIn               [4][1])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_5_3                       ,ETS_GROUP3(1,0,120)   ,RAM_PTR(LogicFunctionIn               [4][2])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_5_4                       ,ETS_GROUP3(1,0,121)   ,RAM_PTR(LogicFunctionIn               [4][3])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionOut_5                        ,ETS_GROUP3(1,0,122)   ,RAM_PTR(LogicFunctionOut              [4])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_6_1                       ,ETS_GROUP3(1,0,123)   ,RAM_PTR(LogicFunctionIn               [5][0])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_6_2                       ,ETS_GROUP3(1,0,124)   ,RAM_PTR(LogicFunctionIn               [5][1])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_6_3                       ,ETS_GROUP3(1,0,125)   ,RAM_PTR(LogicFunctionIn               [5][2])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_6_4                       ,ETS_GROUP3(1,0,126)   ,RAM_PTR(LogicFunctionIn               [5][3])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionOut_6                        ,ETS_GROUP3(1,0,127)   ,RAM_PTR(LogicFunctionOut              [5])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_7_1                       ,ETS_GROUP3(1,0,128)   ,RAM_PTR(LogicFunctionIn               [6][0])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_7_2                       ,ETS_GROUP3(1,0,129)   ,RAM_PTR(LogicFunctionIn               [6][1])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_7_3                       ,ETS_GROUP3(1,0,130)   ,RAM_PTR(LogicFunctionIn               [6][2])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_7_4                       ,ETS_GROUP3(1,0,131)   ,RAM_PTR(LogicFunctionIn               [6][3])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionOut_7                        ,ETS_GROUP3(1,0,132)   ,RAM_PTR(LogicFunctionOut              [6])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_8_1                       ,ETS_GROUP3(1,0,133)   ,RAM_PTR(LogicFunctionIn               [7][0])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_8_2                       ,ETS_GROUP3(1,0,134)   ,RAM_PTR(LogicFunctionIn               [7][1])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_8_3                       ,ETS_GROUP3(1,0,135)   ,RAM_PTR(LogicFunctionIn               [7][2])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionIn_8_4                       ,ETS_GROUP3(1,0,136)   ,RAM_PTR(LogicFunctionIn               [7][3])     ,CO_TypeUint1   ,1    ,CO_TWU    ,CO_PRIO_L )
DEFINE_COMM_OBJ(CO_LogicFunctionOut_8                        ,ETS_GROUP3(1,0,137)   ,RAM_PTR(LogicFunctionOut              [7])        ,CO_TypeUint1   ,1    ,CO_RT     ,CO_PRIO_L )


END_TAB()




/* ALIAS */
#define CO_SetpointIn                             CO_SetpointComfort_1
#define CO_OffsetStandby_1                        CO_SetpointStandby_1
#define CO_OffsetStandby_2                        CO_SetpointStandby_2
#define CO_OffsetEconomy_1                        CO_SetpointEconomy_1
#define CO_OffsetEconomy_2                        CO_SetpointEconomy_2
#define CO_HeatingOut1bit                         CO_HeatingOut
#define CO_HeatingCoolingOut1byte                 CO_HeatingOut
#define CO_HeatingCoolingOut1bit                  CO_HeatingOut
#define CO_CoolingOut1bit                         CO_CoolingOut
#define CO_AuxiliaryHeatingCoolingOut             CO_AuxiliaryHeatingOut
#define CO_CO2Treshold1Counter                    CO_CO2Treshold1Percentage
#define CO_CO2Treshold1Switch                     CO_CO2Treshold1Percentage
#define CO_CO2Treshold2Counter                    CO_CO2Treshold2Percentage
#define CO_CO2Treshold2Switch                     CO_CO2Treshold2Percentage
#define CO_CO2Treshold3Counter                    CO_CO2Treshold3Percentage
#define CO_CO2Treshold3Switch                     CO_CO2Treshold3Percentage
#define CO_CO2VentilationCounter                  CO_CO2VentilationPercentage
#define CO_CO2VentilationBit1                     CO_CO2VentilationPercentage

/* >> END COPY */


/*************************************************************************
*
*   numero di CO = 138

*
*    0   (1 Bit)   0x0       TechnicalAlarm                           1Byte
*    1   (1 Byte)  0x1       LedsIntensityFB                          1Byte
*    2   (2 Bytes) 0x2       TempValue                                2Byte
*    3   (1 Bit)   0x4       TempTreshold1Switch                      1Byte
*    4   (1 Bit)   0x5       TempThreshold1Lock                       1Byte
*    5   (2 Bytes) 0x6       TempThreshold1Value                      2Byte
*    6   (1 Bit)   0x8       TempTreshold2Switch                      1Byte
*    7   (1 Bit)   0x9       TempThreshold2Lock                       1Byte
*    8   (2 Bytes) 0xA       TempThreshold2Value                      2Byte
*    9   (2 Bytes) 0xC       CO2Value                                 2Byte
*    10  (2 Bytes) 0xE       RhValue_2bytes                           2Byte
*    11  (1 Byte)  0x10      RhValue_1byte                            1Byte
*    12  (1 Bit)   0x11      RhTreshold1Switch                        1Byte
*    13  (1 Bit)   0x12      RhThreshold1Lock                         1Byte
*    14  (2 Bytes) 0x13      RhThreshold1_2bytesValue                 2Byte
*    15  (1 Byte)  0x15      RhThreshold1_1byteValue                  1Byte
*    16  (1 Bit)   0x16      RhTreshold2Switch                        1Byte
*    17  (1 Bit)   0x17      RhThreshold2Lock                         1Byte
*    18  (2 Bytes) 0x18      RhThreshold2_2bytesValue                 2Byte
*    19  (1 Byte)  0x1A      RhThreshold2_1byteValue                  1Byte
*    20  (2 Bytes) 0x1B      RoomTempFB                               2Byte
*    21  (2 Bytes) 0x1D      Humidity2bytesFB                         2Byte
*    22  (1 Byte)  0x1F      Humidity1byteFB                          1Byte
*    23  (2 Bytes) 0x20      AntistratificationTempFB                 2Byte
*    24  (2 Bytes) 0x22      OutdoorTempFB                            2Byte
*    25  (2 Bytes) 0x24      CoilTempFB                               2Byte
*    26  (2 Bytes) 0x26      FloorTempFB                              2Byte
*    27  (2 Bytes) 0x28      FlowTempFB                               2Byte
*    28  (1 Bit)   0x2A      WindowsContactSensor1FB                  1Byte
*    29  (1 Bit)   0x2B      WindowsContactSensor2FB                  1Byte
*    30  (1 Bit)   0x2C      PresenceSensor1FB                        1Byte
*    31  (1 Bit)   0x2D      PresenceSensor2FB                        1Byte
*    32  (1 Bit)   0x2E      ContactHolderFB                          1Byte
*    33  (1 Bit)   0x2F      AnticondensationFB                       1Byte
*    34  (2 Bytes) 0x30      WeightedTemp                             2Byte
*    35  (1 Bit)   0x32      HeatingCoolingStatusOut                  1Byte
*    36  (1 Bit)   0x33      HeatingCoolingStatusIn                   1Byte
*    37  (1 Byte)  0x34      HVACModeIn                               1Byte
*    38  (1 Byte)  0x35      HVACForcedModeIn                         1Byte
*    39  (1 Byte)  0x36      HVACModeOut                              1Byte
*    40  (1 Byte)  0x37      HVACManualMode                           1Byte
*    41  (1 Bit)   0x38      HVACProtectionModeStatus                 1Byte
*    42  (1 Bit)   0x39      ChronoModeStatus                         1Byte
*    43  (2 Bytes) 0x3A      ActualSetpoint                           2Byte
*    44  (2 Bytes) 0x3C      ManualSetpoint                           2Byte
*    45  (1 Bit)   0x3E      ManualSetpointStatus                     1Byte
*    46  (2 Bytes) 0x3F      SetpointComfort_1                        2Byte
*    47  (2 Bytes) 0x41      SetpointComfort_2                        2Byte
*    48  (2 Bytes) 0x43      SetpointStandby_1                        2Byte
*    49  (2 Bytes) 0x45      SetpointStandby_2                        2Byte
*    50  (2 Bytes) 0x47      SetpointEconomy_1                        2Byte
*    51  (2 Bytes) 0x49      SetpointEconomy_2                        2Byte
*    52  (2 Bytes) 0x4B      SetpointBuildProtection_1                2Byte
*    53  (2 Bytes) 0x4D      SetpointBuildProtection_2                2Byte
*    54  (1 Bit)   0x4F      DisableTempController                    1Byte
*    55  (1 Bit)   0x50      RoomTempControllerStatus                 1Byte
*    56  (1 Byte)  0x51      HeatingOut                               1Byte
*    57  (1 Byte)  0x52      CoolingOut                               1Byte
*    58  (1 Bit)   0x53      AuxiliaryHeatingOut                      1Byte
*    59  (1 Bit)   0x54      AuxiliaryCoolingOut                      1Byte
*    60  (1 Bit)   0x55      AuxiliaryHeatingDisable                  1Byte
*    61  (1 Bit)   0x56      AuxiliaryCoolingDisable                  1Byte
*    62  (1 Byte)  0x57      FanContinuousSpeed                       1Byte
*    63  (1 Bit)   0x58      FanSpeed_1                               1Byte
*    64  (1 Bit)   0x59      FanSpeed_2                               1Byte
*    65  (1 Bit)   0x5A      FanSpeed_3                               1Byte
*    66  (1 Bit)   0x5B      FanControlDisable                        1Byte
*    67  (1 Byte)  0x5C      FanStepSpeed                             1Byte
*    68  (1 Byte)  0x5D      FanManualSpeedStep                       1Byte
*    69  (1 Byte)  0x5E      FanManualSpeedPercentage                 1Byte
*    70  (1 Bit)   0x5F      FanManualOperation                       1Byte
*    71  (1 Bit)   0x60      FanManualOffStatus                       1Byte
*    72  (1 Bit)   0x61      TempControllerAlarm                      1Byte
*    73  (2 Bytes) 0x62      DewPointTemp                             2Byte
*    74  (2 Bytes) 0x64      HumiditySetpointForDehum                 2Byte
*    75  (2 Bytes) 0x66      HumiditySetpointForHum                   2Byte
*    76  (1 Bit)   0x68      DehumCommand                             1Byte
*    77  (1 Bit)   0x69      DehumWaterBatteryCommand                 1Byte
*    78  (1 Bit)   0x6A      DehumIntegrationControl                  1Byte
*    79  (1 Bit)   0x6B      DehumControlDisable                      1Byte
*    80  (1 Bit)   0x6C      HumidCommand                             1Byte
*    81  (1 Bit)   0x6D      HumidControlDisable                      1Byte
*    82  (1 Bit)   0x6E      AnticondensationAlarm                    1Byte
*    83  (1 Bit)   0x6F      ThermalGeneratorLock                     1Byte
*    84  (1 Byte)  0x70      HVACScene                                1Byte
*    85  (14 Bytes)0x71      AlarmText                                14Byte
*    86  (1 Byte)  0x7F      CO2Treshold1Percentage                   1Byte
*    87  (1 Bit)   0x80      CO2Threshold1Lock                        1Byte
*    88  (1 Byte)  0x81      CO2Treshold2Percentage                   1Byte
*    89  (1 Bit)   0x82      CO2Threshold2Lock                        1Byte
*    90  (1 Byte)  0x83      CO2Treshold3Percentage                   1Byte
*    91  (1 Bit)   0x84      CO2Threshold3Lock                        1Byte
*    92  (1 Byte)  0x85      CO2VentilationPercentage                 1Byte
*    93  (1 Bit)   0x86      CO2VentilationBit2                       1Byte
*    94  (1 Bit)   0x87      CO2VentilationBit3                       1Byte
*    95  (1 Bit)   0x88      CO2VentilationBit4                       1Byte
*    96  (1 Bit)   0x89      CO2VentilationLock                       1Byte
*    97  (1 Bit)   0x8A      CO2VentilationDisableFB                  1Byte
*    98  (1 Bit)   0x8B      LogicFunctionIn_1_1                      1Byte
*    99  (1 Bit)   0x8C      LogicFunctionIn_1_2                      1Byte
*    100 (1 Bit)   0x8D      LogicFunctionIn_1_3                      1Byte
*    101 (1 Bit)   0x8E      LogicFunctionIn_1_4                      1Byte
*    102 (1 Bit)   0x8F      LogicFunctionOut_1                       1Byte
*    103 (1 Bit)   0x90      LogicFunctionIn_2_1                      1Byte
*    104 (1 Bit)   0x91      LogicFunctionIn_2_2                      1Byte
*    105 (1 Bit)   0x92      LogicFunctionIn_2_3                      1Byte
*    106 (1 Bit)   0x93      LogicFunctionIn_2_4                      1Byte
*    107 (1 Bit)   0x94      LogicFunctionOut_2                       1Byte
*    108 (1 Bit)   0x95      LogicFunctionIn_3_1                      1Byte
*    109 (1 Bit)   0x96      LogicFunctionIn_3_2                      1Byte
*    110 (1 Bit)   0x97      LogicFunctionIn_3_3                      1Byte
*    111 (1 Bit)   0x98      LogicFunctionIn_3_4                      1Byte
*    112 (1 Bit)   0x99      LogicFunctionOut_3                       1Byte
*    113 (1 Bit)   0x9A      LogicFunctionIn_4_1                      1Byte
*    114 (1 Bit)   0x9B      LogicFunctionIn_4_2                      1Byte
*    115 (1 Bit)   0x9C      LogicFunctionIn_4_3                      1Byte
*    116 (1 Bit)   0x9D      LogicFunctionIn_4_4                      1Byte
*    117 (1 Bit)   0x9E      LogicFunctionOut_4                       1Byte
*    118 (1 Bit)   0x9F      LogicFunctionIn_5_1                      1Byte
*    119 (1 Bit)   0xA0      LogicFunctionIn_5_2                      1Byte
*    120 (1 Bit)   0xA1      LogicFunctionIn_5_3                      1Byte
*    121 (1 Bit)   0xA2      LogicFunctionIn_5_4                      1Byte
*    122 (1 Bit)   0xA3      LogicFunctionOut_5                       1Byte
*    123 (1 Bit)   0xA4      LogicFunctionIn_6_1                      1Byte
*    124 (1 Bit)   0xA5      LogicFunctionIn_6_2                      1Byte
*    125 (1 Bit)   0xA6      LogicFunctionIn_6_3                      1Byte
*    126 (1 Bit)   0xA7      LogicFunctionIn_6_4                      1Byte
*    127 (1 Bit)   0xA8      LogicFunctionOut_6                       1Byte
*    128 (1 Bit)   0xA9      LogicFunctionIn_7_1                      1Byte
*    129 (1 Bit)   0xAA      LogicFunctionIn_7_2                      1Byte
*    130 (1 Bit)   0xAB      LogicFunctionIn_7_3                      1Byte
*    131 (1 Bit)   0xAC      LogicFunctionIn_7_4                      1Byte
*    132 (1 Bit)   0xAD      LogicFunctionOut_7                       1Byte
*    133 (1 Bit)   0xAE      LogicFunctionIn_8_1                      1Byte
*    134 (1 Bit)   0xAF      LogicFunctionIn_8_2                      1Byte
*    135 (1 Bit)   0xB0      LogicFunctionIn_8_3                      1Byte
*    136 (1 Bit)   0xB1      LogicFunctionIn_8_4                      1Byte
*    137 (1 Bit)   0xB2      LogicFunctionOut_8                       1Byte
*************************************************************************/


/* FINE */
