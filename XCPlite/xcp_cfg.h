#pragma once

/*----------------------------------------------------------------------------
| File:
|   xcp_cfg.h
|
| Description:
|   User configuration file for XCP protocol layer parameters
 ----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Version */

// Driver version (GET_COMM_MODE_INFO)
#define XCP_DRIVER_VERSION 0x01

// Protocol layer version
//#define XCP_PROTOCOL_LAYER_VERSION 0x0101
//#define XCP_PROTOCOL_LAYER_VERSION 0x0103  // GET_DAQ_CLOCK_MULTICAST, GET_TIME_CORRELATION_PROPERTIES
#define XCP_PROTOCOL_LAYER_VERSION 0x0104  // PACKED_MODE, CC_START_STOP_SYNCH prepare

/*----------------------------------------------------------------------------*/
/* Driver features */

// #define XCP_ENABLE_DYN_ADDRESSING // Enable addr_ext=1 indicating relative addr format (event<<16)|offset 


/*----------------------------------------------------------------------------*/
/* Protocol features */
#define OPTION_ENABLE_CAL_SEGMENT ON
#if OPTION_ENABLE_CAL_SEGMENT
#define XCP_ENABLE_CHECKSUM // Enable checksum calculation command
#define XCP_ENABLE_CAL_PAGE // Enable cal page switch
#endif

/*----------------------------------------------------------------------------*/
/* GET_ID command */

// #define XCP_ENABLE_IDT_A2L_UPLOAD // Upload A2L via XCP enabled

/*----------------------------------------------------------------------------*/
/* DAQ features and parameters */

 #define XCP_ENABLE_DAQ_EVENT_INFO // Enable XCP_GET_EVENT_INFO, if this is enabled, A2L file event information will be ignored
 #define XCP_ENABLE_DAQ_EVENT_LIST // Enable event list
 #define XCP_MAX_EVENT 16 // Maximum number of events, size of event table
// #define XCP_ENABLE_MULTITHREAD_EVENTS // Make XcpEvent thread safe for same event coming from different threads
// #define XCP_ENABLE_PACKED_MODE // Enable packed mode 

#define XCP_DAQ_MEM_SIZE (5*200*1000) // Amount of memory for DAQ tables, each ODT entry (e.g. measurement variable) needs 5 bytes

#define XCP_DAQ_CLOCK_32BIT  // Use 32 Bit time stamps in GET_DAQ_CLOCK

#if CLOCK_TICKS_PER_S == 1000000  // Settings for 32 bit us since application start (CLOCK_USE_APP_TIME_US)

#define XCP_TIMESTAMP_UNIT DAQ_TIMESTAMP_UNIT_1US // unit DAQ_TIMESTAMP_UNIT_xxx
#define XCP_TIMESTAMP_TICKS 1  // ticks per unit

#endif
#if CLOCK_TICKS_PER_S == 1000000000  // Settings for 32 bit ns since application start (CLOCK_USE_UTC_TIME_NS)

#define XCP_TIMESTAMP_UNIT DAQ_TIMESTAMP_UNIT_1NS // unit DAQ_TIMESTAMP_UNIT_xxx
#define XCP_TIMESTAMP_TICKS 1  // ticks per unit

#endif


//-------------------------------------------------------------------------------
// Debug 


// Enable extended error checks, performance penalty !!!
//#define XCP_ENABLE_TEST_CHECKS

// Enable debug prints depending on XCP_DBG_LEVEL
#define XCP_ENABLE_DEBUG_PRINTS

#ifdef XCP_ENABLE_DEBUG_PRINTS

extern uint32_t gDebugLevel;
#define XCP_DBG_LEVEL gDebugLevel

#define XCP_DBG_PRINTF_ERROR(format, ...) printf(format, __VA_ARGS__)
#define XCP_DBG_PRINTF(level, format, ...) if (XCP_DBG_LEVEL>=level) printf(format, __VA_ARGS__)
#define XCP_DBG_PRINTF1(format, ...) if (XCP_DBG_LEVEL>=1) printf(format, __VA_ARGS__)
#define XCP_DBG_PRINTF2(format, ...) if (XCP_DBG_LEVEL>=2) printf(format, __VA_ARGS__)
#define XCP_DBG_PRINTF3(format, ...) if (XCP_DBG_LEVEL>=3) printf(format, __VA_ARGS__)
#define XCP_DBG_PRINTF4(format, ...) if (XCP_DBG_LEVEL>=4) printf(format, __VA_ARGS__)

#define XCP_DBG_PRINT_ERROR(s) printf(s)
#define XCP_DBG_PRINT(level, s) if (XCP_DBG_LEVEL>=level) printf(s)
#define XCP_DBG_PRINT1(s) if (XCP_DBG_LEVEL>=1) printf(s)
#define XCP_DBG_PRINT2(s) if (XCP_DBG_LEVEL>=2) printf(s)
#define XCP_DBG_PRINT3(s) if (XCP_DBG_LEVEL>=3) printf(s)
#define XCP_DBG_PRINT4(s) if (XCP_DBG_LEVEL>=4) printf(s)

#else

#define XCP_DBG_PRINTF_ERROR(format, ...)
#define XCP_DBG_PRINTF(level, format, ...)
#define XCP_DBG_PRINTF1(format, ...)
#define XCP_DBG_PRINTF2(format, ...)
#define XCP_DBG_PRINTF3(format, ...)
#define XCP_DBG_PRINTF4(format, ...)

#define XCP_DBG_PRINT_ERROR(format, ...)
#define XCP_DBG_PRINT(level, format, ...)
#define XCP_DBG_PRINT1(format, ...)
#define XCP_DBG_PRINT2(format, ...)
#define XCP_DBG_PRINT3(format, ...)
#define XCP_DBG_PRINT4(format, ...)

#endif

