﻿/*----------------------------------------------------------------------------
| File:
|   main.c
|
| Description:
|   Demo main for XCPite
|   Very basic demo to demonstrate integration into user application  
|
| Copyright (c) Vector Informatik GmbH. All rights reserved.
| Licensed under the MIT license. See LICENSE file in the project root for details.
|
 ----------------------------------------------------------------------------*/

#include "xcp_main.h"
#include "platform.h"
#include "options.h"
#include "util.h"
#include "xcpLite.h"
#if OPTION_ENABLE_A2L_GEN
#include "A2L.h"
#endif



// All OPTIONs defined in main_cfg.h

unsigned int gDebugLevel = OPTION_DEBUG_LEVEL;


//-------------------------------------------------------------------------------------------------


// Demo measurement event
uint16_t event = 0;

// Demo measurement signal
double channel1 = 0.0;
uint16_t counter = 0;

// Demo parameters
double ampl = 400.0;
double period = 3.0;
uint32_t cycleTime = 2000; // us


//-------------------------------------------------------------------------------------------------

BOOL xcpTasksRunning = FALSE;

// XCP command handler task
#ifdef _WIN
DWORD WINAPI rxTask(LPVOID p)
#else
void* rxTask(void* p)
#endif
{
  (void)p;
  while (xcpTasksRunning) {

    // Handle incoming XCP commands (blocking)
    if (!XcpTlHandleCommands(XCPTL_TIMEOUT_INFINITE)) break; // Error -> terminate 
  }
  xcpTasksRunning = FALSE;
  return 0;
}


// XCP transmit queue handler task
#ifdef _WIN
DWORD WINAPI txTask(LPVOID p)
#else
void* txTask(void* p)
#endif
{
  (void)p;
  while (xcpTasksRunning) {

    // Wait for transmit data available (50ms timeout)
    if (!XcpTlWaitForTransmitData(50)) XcpTlFlushTransmitBuffer(); // Flush after timerout to keep data visualization going

    // Transmit all pending complete messages from the transmit queue
    if (XcpTlHandleTransmitQueue() < 0) break; // Error -> terminate
  }
  xcpTasksRunning = FALSE;
  return 0;
}


//-------------------------------------------------------------------------------------------------

static BOOL checkKeyboard(void) { 
    if (_kbhit()) { if (_getch()==27) {  return FALSE; } } return TRUE; }

static void info(void) {
    printf("\nXCPlite - Simple Demo\n");
#ifdef __x86_64__
    printf("x86_64\n");
#elif defined(__arm64__)
    printf("arm64\n");
#else
    printf("Unbekannte Architektur!\n");
#endif
}

void *xcp_main(void *data) {

    (void)data;
    info();

    // Initialize high resolution clock for measurement event timestamping
    if (!clockInit()) return 0;

    // Initialize XCP protocol layer
    XcpInit();

    // Init network
    if (!socketStartup()) return 0;

    // Initialize XCP on ETH transport layer
    uint8_t ipAddr[] = OPTION_SERVER_ADDR;
    if (!XcpEthTlInit(ipAddr, OPTION_SERVER_PORT, OPTION_USE_TCP, OPTION_MTU-28 /* max size of XCP payload in UDP */, TRUE /* blocking rx */)) return 0;

    
    // Create ASAM A2L description file for measurement signals, calibration variables, events and communication parameters 
#if OPTION_ENABLE_A2L_GEN
    if (!A2lOpen(OPTION_A2L_FILE_NAME, OPTION_A2L_PROJECT_NAME)) return 0;
    extern char __data_start[];
    extern char _edata[];
    A2lCreate_MOD_PAR(ApplXcpGetAddr(__data_start), _edata - __data_start, &__data_start);
    event = XcpCreateEvent("mainLoop", 2000000, 0, 0, 0);
#ifdef __cplusplus // In C++, A2L objects datatype is detected at run time
    A2lCreateParameterWithLimits(ampl, "Amplitude of sinus signal in V", "V", 0, 800);
    A2lCreateParameterWithLimits(period, "Period of sinus signal in s", "s", 0, 10);
    A2lCreateParameterWithLimits(cycleTime, "Cycle time of demo event loop in us", "us", 0, 1000000);
    A2lSetFixedEvent(event); // Associate to the variables created below
    A2lCreatePhysMeasurement(channel1, "Sinus demo signal", 1.0, 0.0, "V");
    A2lCreateMeasurement(counter, "Event counter");
#else
    A2lCreateParameterWithLimits(gDebugLevel, A2L_TYPE_UINT32, "gDebugLevel", "unit", 0, 8);
    A2lCreateParameterWithLimits(ampl, A2L_TYPE_DOUBLE, "Amplitude of sinus signal in V", "V", 0, 800);
    A2lCreateParameterWithLimits(period, A2L_TYPE_DOUBLE, "Period of sinus signal in s", "s", 0, 10);
    A2lCreateParameterWithLimits(cycleTime, A2L_TYPE_UINT32, "Cycle time of demo event loop in us", "us", 0, 1000000);
    A2lSetFixedEvent(event); // Associate to the variables created below
    A2lCreatePhysMeasurement(channel1, A2L_TYPE_DOUBLE, "Sinus demo signal", 1.0, 0.0, "V");
    A2lCreateMeasurement(counter, A2L_TYPE_UINT32, "Event counter");
#endif
    A2lCreate_ETH_IF_DATA(OPTION_USE_TCP, ipAddr, OPTION_SERVER_PORT);
    A2lClose();
#endif    

    // Start XCP rx and tx handler threads
    xcpTasksRunning = TRUE;
    printf("Start XCP rx and tx task\n");
    tXcpThread rxThread;
    create_thread(&rxThread, rxTask); 
    tXcpThread txThread;
    create_thread(&txThread, txTask);

    // Start XCP
    XcpStart();

    // Mainloop 
    while (xcpTasksRunning) {

        // XCP Measurement Demo
        counter++;
        channel1 = ampl * sin(M_2PI * ((double)clockGet() / CLOCK_TICKS_PER_S) / period);
        XcpEvent(event); // Trigger XCP measurement data aquisition event 

        sleepNs(cycleTime*1000);

        if (!checkKeyboard()) {
          XcpSendEvent(EVC_SESSION_TERMINATED, NULL, 0);
          break;
        }
    }
            
    // Stop XCP
    XcpDisconnect();

    // Stop XCP rx and tx handler threads
    xcpTasksRunning = FALSE;
    printf("Stop XCP rx and tx task\n");
    cancel_thread(rxThread);
    cancel_thread(txThread);

    // Shutdown XCP transport layer
    XcpTlShutdown();

    printf("\nXCPlite terminated. Press any key to close\n");
    while (!_kbhit()) sleepMs(100);
    return 1;
}


