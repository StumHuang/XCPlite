/*----------------------------------------------------------------------------
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
#include "xcpLite.h"
#include "xcpEthServer.h"
#if OPTION_ENABLE_DBG_PRINTS
#include "dbg_print.h"
#endif
#if OPTION_ENABLE_A2L_GEN
#include "A2L.h"
#endif


// OPTIONs defined in main_cfg.h


//-------------------------------------------------------------------------------------------------


// measurement event
uint16_t xcp_event_2ms = 0;
uint16_t xcp_event_5ms = 0;
uint16_t xcp_event_10ms = 0;
uint16_t xcp_event_20ms = 0;
uint16_t xcp_event_50ms = 0;
uint16_t xcp_event_100ms = 0;
uint16_t xcp_event_200ms = 0;
uint16_t xcp_event_500ms = 0;
uint16_t xcp_event_1000ms = 0;
uint16_t xcp_event_2000ms = 0;

// measurement signal
double channel1 = 0.0;
uint16_t counter = 0;

// Demo parameters
double ampl = 400.0;
double period = 3.0;
uint32_t cycleTime = 2000; // us


//-------------------------------------------------------------------------------------------------

static BOOL checkKeyboard(void) { if (_kbhit()) { if (_getch()==27) {  return FALSE; } } return TRUE; }

#define xcp_task_ms(time) // ECU cyclic #time ms task\
#ifdef _WIN \
DWORD WINAPI xcp_task_#time ms(LPVOID p) \
#else \
void* xcp_task_#time ms(void* p) \
#endif \
{ \
    (void)p; \
    printf("Start C task (cycle = %dms, XCP event = %d)\n",#time , xcp_event_#time ms); \
    for (;;) { \
        sleepMs(#time ); \
        XcpEvent(xcp_event_#time ms); // Trigger XCP measurement data aquisition event \
    } \
}

// ECU cyclic 2ms task
#ifdef _WIN
DWORD WINAPI xcp_task_2ms(LPVOID p)
#else
void* xcp_task_2ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",2, xcp_event_2ms);
    for (;;) {
        sleepMs(2); 
        XcpEvent(xcp_event_2ms); // Trigger XCP measurement data aquisition event 
    }
}

// ECU cyclic 5ms  task
#ifdef _WIN
DWORD WINAPI xcp_task_5ms(LPVOID p)
#else
void* xcp_task_5ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",5, xcp_event_5ms);
    for (;;) {
        sleepMs(5); 
        XcpEvent(xcp_event_5ms); // Trigger XCP measurement data aquisition event 
    }
}

// ECU cyclic 10ms  task
#ifdef _WIN
DWORD WINAPI xcp_task_10ms(LPVOID p)
#else
void* xcp_task_10ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",10, xcp_event_10ms);
    for (;;) {
        sleepMs(10); 
        XcpEvent(xcp_event_10ms); // Trigger XCP measurement data aquisition event 
    }
}

// ECU cyclic 20ms  task
#ifdef _WIN
DWORD WINAPI xcp_task_20ms(LPVOID p)
#else
void* xcp_task_20ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",20, xcp_event_20ms);
    for (;;) {
        sleepMs(20); 
        XcpEvent(xcp_event_20ms); // Trigger XCP measurement data aquisition event 
    }
}

// ECU cyclic 50ms  task
#ifdef _WIN
DWORD WINAPI xcp_task_50ms(LPVOID p)
#else
void* xcp_task_50ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",50, xcp_event_50ms);
    for (;;) {
        sleepMs(50); 
        XcpEvent(xcp_event_50ms); // Trigger XCP measurement data aquisition event 
    }
}

// ECU cyclic 100ms  task
#ifdef _WIN
DWORD WINAPI xcp_task_100ms(LPVOID p)
#else
void* xcp_task_100ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",100, xcp_event_100ms);
    for (;;) {
        sleepMs(100); 
        XcpEvent(xcp_event_100ms); // Trigger XCP measurement data aquisition event 
    }
}

// ECU cyclic 200ms  task
#ifdef _WIN
DWORD WINAPI xcp_task_200ms(LPVOID p)
#else
void* xcp_task_200ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",200, xcp_event_200ms);
    for (;;) {
        sleepMs(200); 
        XcpEvent(xcp_event_200ms); // Trigger XCP measurement data aquisition event 
    }
}

// ECU cyclic 500ms  task
#ifdef _WIN
DWORD WINAPI xcp_task_500ms(LPVOID p)
#else
void* xcp_task_500ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",500, xcp_event_500ms);
    for (;;) {
        sleepMs(500); 
        XcpEvent(xcp_event_500ms); // Trigger XCP measurement data aquisition event 
    }
}

// ECU cyclic 1000ms  task
#ifdef _WIN
DWORD WINAPI xcp_task_1000ms(LPVOID p)
#else
void* xcp_task_1000ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",1000, xcp_event_1000ms);
    for (;;) {
        sleepMs(1000); 
        XcpEvent(xcp_event_1000ms); // Trigger XCP measurement data aquisition event 
    }
}

// ECU cyclic 2000ms  task
#ifdef _WIN
DWORD WINAPI xcp_task_2000ms(LPVOID p)
#else
void* xcp_task_2000ms(void* p)
#endif
{
    (void)p;
    printf("Start C task (cycle = %dms, XCP event = %d)\n",2000, xcp_event_2000ms);
    for (;;) {
        sleepMs(2000); 
        XcpEvent(xcp_event_2000ms); // Trigger XCP measurement data aquisition event 
    }
}

int *xcp_main(void *data) {

    (void)data;


    // Initialize high resolution clock for measurement event timestamping
    if (!clockInit()) return 0;

    // Init network
    if (!socketStartup()) return 0;
    
    // Initialize the XCP Server
    uint8_t ipAddr[] = OPTION_SERVER_ADDR;
    if (!XcpEthServerInit(ipAddr, OPTION_SERVER_PORT, OPTION_USE_TCP, XCPTL_MAX_SEGMENT_SIZE)) return 0;

    // Test address conversion functions
    // uint8_t* ApplXcpGetPointer(uint8_t addr_ext, uint32_t addr);
    // uint32_t ApplXcpGetAddr(uint8_t * p);
    uint32_t a = ApplXcpGetAddr((uint8_t*)&ampl);
    uint8_t* p = ApplXcpGetPointer(0 /*addr_ext*/, a);
    double val = *(double*)p; // read
    assert(ampl == val); 
    *(double*)p = 100.0; // write
    assert(ampl == 100.0);

    // Create ASAM A2L description file for measurement signals, calibration variables, events and communication parameters 
#if OPTION_ENABLE_A2L_GEN
    if (!A2lOpen(OPTION_A2L_FILE_NAME, OPTION_A2L_NAME)) return 0;
    extern char __data_start[];
    extern char _edata[];
    A2lCreate_MOD_PAR(ApplXcpGetAddr(__data_start), _edata - __data_start, &__data_start);

    // Create an XCP event for the cyclic task
    xcp_event_2ms = XcpCreateEvent("xcp_task_2ms", 2*1000000, 0, 0, 0);
    xcp_event_5ms = XcpCreateEvent("xcp_task_5ms", 5*1000000, 1, 0, 0);
    xcp_event_10ms = XcpCreateEvent("xcp_task_10ms", 10*1000000, 2, 0, 0);
    xcp_event_20ms = XcpCreateEvent("xcp_task_20ms", 20*1000000, 3, 0, 0);
    xcp_event_50ms = XcpCreateEvent("xcp_task_50ms", 50*1000000, 4, 0, 0);
    xcp_event_100ms = XcpCreateEvent("xcp_task_100ms", 100*1000000, 5, 0, 0);
    xcp_event_200ms = XcpCreateEvent("xcp_task_200ms", 200*1000000, 6, 0, 0);
    xcp_event_500ms = XcpCreateEvent("xcp_task_500ms", 500*1000000, 7, 0, 0);
    xcp_event_1000ms = XcpCreateEvent("xcp_task_1000ms", 1000*1000000, 8, 0, 0);
    xcp_event_2000ms = XcpCreateEvent("xcp_task_2000ms", 2000*1000000, 9, 0, 0);
#ifdef __cplusplus // In C++, A2L objects datatype is detected at run time
    A2lCreateParameterWithLimits(ampl, "Amplitude of sinus signal in V", "V", 0, 800);
    A2lCreateParameterWithLimits(period, "Period of sinus signal in s", "s", 0, 10);
    A2lCreateParameterWithLimits(cycleTime, "Cycle time of demo event loop in us", "us", 0, 1000000);
    A2lSetFixedEvent(xcp_event_2ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_5ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_10ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_20ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_50ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_100ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_200ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_500ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_1000ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_2000ms); // Associate to the variables created below
    A2lCreatePhysMeasurement(channel1, "Sinus demo signal", 1.0, 0.0, "V");
    A2lCreateMeasurement(counter, "Event counter");
#else
    A2lCreateParameterWithLimits(ampl, A2L_TYPE_DOUBLE, "Amplitude of sinus signal in V", "V", 0, 800);
    A2lCreateParameterWithLimits(period, A2L_TYPE_DOUBLE, "Period of sinus signal in s", "s", 0, 10);
    A2lCreateParameterWithLimits(cycleTime, A2L_TYPE_DOUBLE, "Cycle time of demo event loop in us", "us", 0, 1000000);
    A2lSetFixedEvent(xcp_event_2ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_5ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_10ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_20ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_50ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_100ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_200ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_500ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_1000ms); // Associate to the variables created below
    A2lSetFixedEvent(xcp_event_2000ms); // Associate to the variables created below
    A2lCreatePhysMeasurement(channel1, A2L_TYPE_DOUBLE, "Sinus demo signal", 1.0, 0.0, "V");
    A2lCreateMeasurement(counter, A2L_TYPE_UINT32, "Event counter");
    #if OPTION_ENABLE_DBG_PRINTS
    A2lCreateParameterWithLimits(gDebugLevel, A2L_TYPE_UINT32, "Debug Level", "emun", 0, 6);
    #endif
#endif
    A2lCreate_ETH_IF_DATA(OPTION_USE_TCP, ipAddr, OPTION_SERVER_PORT);
    A2lClose();
#endif    

    tXcpThread xcp_thread_task_2ms;
    create_thread(&xcp_thread_task_2ms, xcp_task_2ms); // create a cyclic task which produces demo measurement signal events

    tXcpThread xcp_thread_task_5ms;
    create_thread(&xcp_thread_task_5ms, xcp_task_5ms); // create a cyclic task which produces demo measurement signal events

    tXcpThread xcp_thread_task_10ms;
    create_thread(&xcp_thread_task_10ms, xcp_task_10ms); // create a cyclic task which produces demo measurement signal events

    tXcpThread xcp_thread_task_20ms;
    create_thread(&xcp_thread_task_20ms, xcp_task_20ms); // create a cyclic task which produces demo measurement signal events

    tXcpThread xcp_thread_task_50ms;
    create_thread(&xcp_thread_task_50ms, xcp_task_50ms); // create a cyclic task which produces demo measurement signal events

    tXcpThread xcp_thread_task_100ms;
    create_thread(&xcp_thread_task_100ms, xcp_task_100ms); // create a cyclic task which produces demo measurement signal events

    tXcpThread xcp_thread_task_200ms;
    create_thread(&xcp_thread_task_200ms, xcp_task_200ms); // create a cyclic task which produces demo measurement signal events

    tXcpThread xcp_thread_task_500ms;
    create_thread(&xcp_thread_task_500ms, xcp_task_500ms); // create a cyclic task which produces demo measurement signal events

    tXcpThread xcp_thread_task_1000ms;
    create_thread(&xcp_thread_task_1000ms, xcp_task_1000ms); // create a cyclic task which produces demo measurement signal events

    tXcpThread xcp_thread_task_2000ms;
    create_thread(&xcp_thread_task_2000ms, xcp_task_2000ms); // create a cyclic task which produces demo measurement signal events
    // Mainloop 
    for (;;) {

        // XCP Measurement Demo

        sleepMs(500);

        if (!XcpEthServerStatus()) { printf("\nXCP Server failed\n");  break; } // Check if the XCP server is running

        if (!checkKeyboard()) {
          XcpSendEvent(EVC_SESSION_TERMINATED, NULL, 0);
          break;
        }
    }

    // Terminate task
    sleepMs(1000);
    cancel_thread(xcp_thread_task_2ms);
    cancel_thread(xcp_thread_task_5ms);
    cancel_thread(xcp_thread_task_10ms);
    cancel_thread(xcp_thread_task_20ms);
    cancel_thread(xcp_thread_task_50ms);
    cancel_thread(xcp_thread_task_100ms);
    cancel_thread(xcp_thread_task_200ms);
    cancel_thread(xcp_thread_task_500ms);
    cancel_thread(xcp_thread_task_1000ms);
    cancel_thread(xcp_thread_task_2000ms);

    // Stop XCP
    XcpDisconnect();

    // Stop the XCP server
    XcpEthServerShutdown();
    socketCleanup();

    printf("\nXCPlite terminated. Press any key to close\n");
    while (!_kbhit()) sleepMs(100);
    return 1;
}