/// Stampa messaggi switch di compilazione


#ifdef EK_ET2_TP
#warning ========================================================
#warning EKINEX EK-ET2-TP
#warning ========================================================
#endif

#ifdef TEST_CE
#warning >>> TEST_CE ABILITATO!
#endif
#ifdef SHT21_SEND_RAW_VALUE
#warning >>> SHT21_SEND_RAW_VALUE ABILITATO!
#endif
#ifdef SGP30_SEND_RAW_VALUE
#warning >>> SGP30_SEND_RAW_VALUE ABILITATO!
#endif
#ifdef CO2_VALUE_FROM_BUS
#warning >>> CO2_VALUE_FROM_BUS ABILITATO!
#endif
#ifdef RH_VALUE_FROM_BUS
#warning >>> RH_VALUE_FROM_BUS ABILITATO!
#endif
#ifdef TEMP_VALUE_FROM_BUS
#warning >>> TEMP_VALUE_FROM_BUS ABILITATO!
#endif
#ifdef SETPONT_STANDBY_ECONOMY_FLOAT
#warning >>> Formato dei setpoint ETS: float
#else
#warning >>> Formato dei setpoint ETS: unsigned integer
#endif
#ifdef GESTIONE_ETS_SETPOINT_VERS1
#warning >>> ETS inserimento setpoint standby e comfort RELATIVI
#else
#warning >>> ETS inserimento setpoint standby e comfort ASSOLUTI
#endif
#ifdef FIR
#warning >>> FIR filtro primo ordine abilitato
#endif
#warning ========================================================


