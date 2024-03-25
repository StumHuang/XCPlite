/*----------------------------------------------------------------------------
| File:
|   xcpAppl.c
|
| Description:
|   Application specific functions and callbacks for XCP driver
|   
| Copyright (c) Vector Informatik GmbH. All rights reserved.
| Licensed under the MIT license. See LICENSE file in the project root for details.
|
 ----------------------------------------------------------------------------*/

#include "xcp_main.h"
#include "main_cfg.h"
#include "platform.h"
#include "options.h"
#include "util.h"
#include "xcpLite.h"
    

/**************************************************************************/
// General Callbacks from XCPlite.c
/**************************************************************************/


BOOL ApplXcpConnect() {
    XCP_DBG_PRINT1("XCP connect\n");
    return TRUE;
}

#if XCP_PROTOCOL_LAYER_VERSION >= 0x0104
BOOL ApplXcpPrepareDaq() { 
    XCP_DBG_PRINT1("XCP prepare DAQ\n");
    return TRUE;
}
#endif

BOOL ApplXcpStartDaq() {
    XCP_DBG_PRINT1("XCP start DAQ\n");
    return TRUE;
}

void ApplXcpStopDaq() {
    XCP_DBG_PRINT1("XCP stop DAQ\n");
}




/**************************************************************************/
// Clock
// Get clock for DAQ timestamps
/**************************************************************************/

// XCP server clock timestamp resolution defined in xcp_cfg.h
// Clock must be monotonic !!!

uint64_t ApplXcpGetClock64() { 

    return clockGet();
}

uint8_t ApplXcpGetClockState() { 

    return CLOCK_STATE_FREE_RUNNING; // Clock is a free running counter 
}

BOOL ApplXcpGetClockInfoGrandmaster(uint8_t* uuid, uint8_t* epoch, uint8_t* stratum) {

    (void)uuid;
    (void)epoch;
    (void)stratum;

    return FALSE; // No PTP support implemented
}


/**************************************************************************/
// Pointer - Address conversion
/**************************************************************************/

// 64 Bit and 32 Bit platform pointer to XCP/A2L address conversions
// XCP memory access is limited to a 4GB address range (32 Bit)

// The XCP addresses with extension = 0 for Win32 and Win64 versions of XCPlite are defined as relative to the load address of the main module
// This allows using Microsoft linker PDB files for address update
// In Microsoft Visual Studio set option "Generate Debug Information" to "optimized for sharing and publishing (/DEBUG:FULL)"



uint8_t* ApplXcpGetPointer(uint8_t addr_ext, uint32_t addr) {

    if (addr_ext != 0) return NULL;
    return ApplXcpGetBaseAddr() + addr;
}


#ifdef _WIN

static uint8_t* baseAddr = NULL;
static uint8_t baseAddrValid = 0;

// Get base pointer for the XCP address range
// This function is time sensitive, as it is called once on every XCP event
uint8_t* ApplXcpGetBaseAddr() {

    if (!baseAddrValid) {
        baseAddr = (uint8_t*)GetModuleHandle(NULL);
        baseAddrValid = 1;
        XCP_DBG_PRINTF4("ApplXcpGetBaseAddr() = 0x%I64X\n", (uint64_t)baseAddr);
    }
    return baseAddr;
}

uint32_t ApplXcpGetAddr(uint8_t* p) {

    assert(p >= ApplXcpGetBaseAddr());
#ifdef _WIN64
    assert(((uint64_t)p - (uint64_t)ApplXcpGetBaseAddr()) <= 0xffffffff); // be sure that XCP address range is sufficient
#endif
    return (uint32_t)(p - ApplXcpGetBaseAddr());
}

#endif

#if defined(_LINUX64) && !defined(__APPLE__)

#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <link.h>

uint8_t* baseAddr = NULL;
uint8_t baseAddrValid = 0;

static int dump_phdr(struct dl_phdr_info* pinfo, size_t size, void* data)
{
    // XCP_DBG_PRINTF1("name=%s (%d segments)\n", pinfo->dlpi_name, pinfo->dlpi_phnum);

    // Application modules has no name
    if (0 == strlen(pinfo->dlpi_name)) {
        baseAddr = (uint8_t*)pinfo->dlpi_addr;
    }

    (void)size;
    (void)data;
    return 0;
}

uint8_t* ApplXcpGetBaseAddr() {

    if (!baseAddrValid) {
        dl_iterate_phdr(dump_phdr, NULL);
        assert(baseAddr != NULL);
        baseAddrValid = 1;
        XCP_DBG_PRINTF1("BaseAddr = %lX\n", (uint64_t)baseAddr);
    }

    return baseAddr;
}

uint32_t ApplXcpGetAddr(uint8_t* p)
{
    ApplXcpGetBaseAddr();
    return (uint32_t)(p - baseAddr);
}

#endif


#ifdef __APPLE__

static uint8_t __base_addr_val = 0;

uint8_t* ApplXcpGetBaseAddr()
{
    return ((uint8_t*)((uint64_t)(&__base_addr_val)&0xffffffff00000000));
}

uint32_t ApplXcpGetAddr(uint8_t* p)
{
    return ((uint32_t)((uint64_t) p)& 0xffffffff);
}

#endif



#ifdef _LINUX32

uint8_t* ApplXcpGetBaseAddr()
{
    return ((uint8_t*)0);
}

uint32_t ApplXcpGetAddr(uint8_t* p)
{
    return ((uint32_t)(p));
}

#endif





/**************************************************************************/
// Provide infos for GET_ID
// The XCP command GET_ID provides different type of identification
// information to the XCP client (see code below)
/**************************************************************************/


uint32_t ApplXcpGetId(uint8_t id, uint8_t* buf, uint32_t bufLen) {

    uint32_t len = 0;
    switch (id) {

    case IDT_ASCII:
      len = (uint32_t)strlen(APP_NAME);
      if (buf) {
        if (len > bufLen) return 0; // Insufficient buffer space
        strncpy((char*)buf, APP_NAME, len);
      }
      break;

#ifdef OPTION_A2L_NAME
    case IDT_ASAM_NAME:
      len = (uint32_t)strlen(OPTION_A2L_NAME);
      if (buf) {
        if (len > bufLen) return 0; // Insufficient buffer space
        strncpy((char*)buf, OPTION_A2L_NAME, len);
      }
      break;
#endif

#ifdef OPTION_A2L_FILE_NAME
    case IDT_ASAM_PATH:
      len = (uint32_t)strlen(OPTION_A2L_FILE_NAME);
      if (buf) {
        if (len > bufLen) return 0; // Insufficient buffer space
        strncpy((char*)buf, OPTION_A2L_FILE_NAME, len);
      }
      break;
#endif


    }
    return len;
}

// segment = 0
// RAM = page 0, FLASH = page 1

uint8_t ApplXcpGetCalPage(uint8_t segment, uint8_t mode) {
    (void)mode;
    if (segment > 0) return CRC_PAGE_NOT_VALID;
    return 0;
}

uint8_t ApplXcpSetCalPage(uint8_t segment, uint8_t page, uint8_t mode) {
    if (segment > 0) return CRC_SEGMENT_NOT_VALID;
    if (page > 1) return CRC_PAGE_NOT_VALID;
    if ((mode & (CAL_PAGE_MODE_ECU | CAL_PAGE_MODE_XCP)) != (CAL_PAGE_MODE_ECU | CAL_PAGE_MODE_XCP)) return CRC_PAGE_MODE_NOT_VALID;
    return 0;
}

