//-----------------------------------------------------------------------------
//  Copyright (c) WAGO Kontakttechnik GmbH & Co. KG
//
//  PROPRIETARY RIGHTS are involved in the subject matter of this material.
//  All manufacturing, reproduction, use and sales rights pertaining to this
//  subject matter are governed by the license agreement. The recipient of this
//  software implicitly accepts the terms of the license.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
///  \file     getkbusconfig.c
///
///  \version  $Id:
///
///  \brief    Simple demo program who retrieve information about connected
///            KBUS-IO-Modules via dbus interface.
///
///  \author   cm : WAGO Kontakttechnik GmbH & Co. KG
//----------------------------------------------------------------------------- 
// Binary Coding rules for "u16 terminals[LDKC_KBUS_TERMINAL_COUNT_MAX];"
//      0xxx xxxx xxxx xxxx : Second part of Order-Number
//      1xxx xxxx xxxx xxxx : Stupid Digital IO-Terminal
//      1xxx 1xxx xxxx xxxx : 8-Channels
//      1xxx x1xx xxxx xxxx : 4-Channels
//      1xxx xx1x xxxx xxxx : 2-Channels
//      1xxx xxx1 xxxx xxxx : 1-Channel
//      1xxx xxxx xxxx xxx1 : Digital-Input
//      1xxx xxxx xxxx xx1x : Digital-Output
//-----------------------------------------------------------------------------
// standard include files
//-----------------------------------------------------------------------------

#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
//-----------------------------------------------------------------------------
// include files for KBUS WAGO ADI
//-----------------------------------------------------------------------------
#include <dal/adi_application_interface.h>
//-----------------------------------------------------------------------------
// include files for kbus information via dbus
//-----------------------------------------------------------------------------
#include <ldkc_kbus_information.h>
#include <ldkc_kbus_register_communication.h>

//-----------------------------------------------------------------------
///
/// main of kbus demo program
///
/// \param none
///
//-----------------------------------------------------------------------

int
main(void)
{
    // vars for ADI-interface
    tDeviceInfo deviceList[10];          // the list of devices given by the ADI
    size_t nrDevicesFound;               // number of devices found
    size_t nrKbusFound;                  // position of the kbus in the list
    tDeviceId kbusDeviceId;              // device ID from the ADI
    tApplicationDeviceInterface * adi;   // pointer to the application interface
    tApplicationStateChangedEvent event; // var for the event interface of the ADI

    // generic vars
    int i = 0;

    // startup info */
    printf("**************************************************\n");
    printf("***    KBUS Conf Demo Application V1.00        ***\n");
    printf("**************************************************\n");

    //*** Start KBUS ********************************************

    // connect to ADI-interface
    adi = adi_GetApplicationInterface();

    // init interface
    adi->Init();

    // scan devices
    adi->ScanDevices();
    adi->GetDeviceList(sizeof(deviceList), deviceList, &nrDevicesFound);

    // find kbus device
    nrKbusFound = -1;
    for (i = 0; i < nrDevicesFound; ++i)
    {
        if (strcmp(deviceList[i].DeviceName, "libpackbus") == 0)
        {
            nrKbusFound = i;
            printf("KBUS device found as device %i\n", i);
        }
    }

    // kbus not found > exit
    if (nrKbusFound == -1)
    {
        printf("No KBUS device found \n");
        adi->Exit(); // disconnect ADI-Interface
        return -1; // exit program
    }

    // open kbus device
    kbusDeviceId = deviceList[nrKbusFound].DeviceId;
    if (adi->OpenDevice(kbusDeviceId) != DAL_SUCCESS)
    {
        printf("Kbus device open failed\n");
        adi->Exit(); // disconnect ADI-Interface
        return -2; // exit program
    }
    printf("KBUS device open OK\n");


    // set application state to "Unconfigured" to let library drive kbus by them selve.
    // In this mode library set up a thread who drive the kbus cyclic.
    event.State = ApplicationState_Unconfigured;
    if (adi->ApplicationStateChanged(event) != DAL_SUCCESS)
    {
        // Set application state to "Unconfigured" failed
        printf("Set application state to 'Unconfigured' failed\n");
        adi->CloseDevice(kbusDeviceId); // close kbus device
        adi->Exit(); // disconnect ADI-Interface
        return -3; // exit programm
    }
    printf("Set application state to 'Unconfigured' \n");

    //*** Retrieve KBUS-Config via DBUS ***************************

    if (KbusInfo_Failed == ldkc_KbusInfo_Create())
    {
        printf(" ldkc_KbusInfo_Create() failed\n");
        adi->CloseDevice(kbusDeviceId); // close kbus device
        adi->Exit(); // disconnect ADI-Interface
        return -11;
    }

    // GetStatus
    tldkc_KbusInfo_Status status;
    if (KbusInfo_Failed == ldkc_KbusInfo_GetStatus(&status))
    {
        printf(" ldkc_KbusInfo_GetStatus() failed\n");
        adi->CloseDevice(kbusDeviceId); // close kbus device
        adi->Exit(); // disconnect ADI-Interface
        ldkc_KbusInfo_Destroy();
        return -12;
    }
    printf("\n        .KbusBitCount: %i ",status.KbusBitCount);
    printf("\n        .TerminalCount: %i ",status.TerminalCount);
    printf("\n        .ErrorCode: %i ",status.ErrorCode);
    printf("\n        .ErrorArg: %i ",status.ErrorArg);
    printf("\n        .ErrorPos: %i ",status.ErrorPos);
    printf("\n        .BitCountAnalogInput: %i ",status.BitCountAnalogInput);
    printf("\n        .BitCountAnalogOutput: %i ",status.BitCountAnalogOutput);
    printf("\n        .BitCountDigitalInput: %i ",status.BitCountDigitalInput);
    printf("\n        .BitCountDigitalOutput: %i ",status.BitCountDigitalOutput);

    // GetTerminalInfo
    unsigned char ucPosition;
    unsigned char ucIndex;
    unsigned char ucCntOfTrmnlType;
    unsigned char ucMaxPosition;

    size_t terminalCount;
    u16 terminals[LDKC_KBUS_TERMINAL_COUNT_MAX];
    tldkc_KbusInfo_TerminalInfo terminalDescription[LDKC_KBUS_TERMINAL_COUNT_MAX];

    if ( KbusInfo_Failed == ldkc_KbusInfo_GetTerminalInfo(OS_ARRAY_SIZE(terminalDescription), terminalDescription, &terminalCount) )
    {
        printf(" ldkc_KbusInfo_GetTerminalInfo() failed\n");
        adi->CloseDevice(kbusDeviceId); // close kbus device
        adi->Exit(); // disconnect ADI-Interface
        ldkc_KbusInfo_Destroy();
        return -13;
    }

    if ( KbusInfo_Failed == ldkc_KbusInfo_GetTerminalList(OS_ARRAY_SIZE(terminals), terminals, NULL) )
    {
        printf(" ldkc_KbusInfo_GetTerminalList() failed\n");
        adi->CloseDevice(kbusDeviceId); // close kbus device
        adi->Exit(); // disconnect ADI-Interface
        ldkc_KbusInfo_Destroy();
        return -14;
    }
    ucPosition    = 1;
    ucMaxPosition = terminalCount;

    for (ucIndex = 0; ucPosition <= ucMaxPosition; ucPosition++, ucIndex++)
    {
        const u32 idx = ucPosition - 1;

        printf("\n Pos:%i;", ucPosition);
        if( 0 == (terminals[idx] & 0x8000) )
        {
            // Show second part of order number in "dec" format for complex IO-Terminals
            printf("\t Type:%i;", terminals[idx]);
        }
        else
        {
            // Show bit coded information of stupid digital IO-Terminals in "hex" format
            printf("\t Type:0x%x;", terminals[idx]);
        }
        printf("\t BitOffsetOut:%i;", terminalDescription[idx].OffsetOutput_bits);
        printf("\t BitSizeOut:%i;", terminalDescription[idx].SizeOutput_bits);
        printf("\t BitOffsetIn:%i;", terminalDescription[idx].OffsetInput_bits);
        printf("\t BitSizeIn:%i;", terminalDescription[idx].SizeInput_bits);
        printf("\t Channels:%i;", terminalDescription[idx].AdditionalInfo.ChannelCount);
        printf("\t PiFormat:%i;", terminalDescription[idx].AdditionalInfo.PiFormat);
    }

    usleep(1000);

    adi->CloseDevice(kbusDeviceId); // close kbus device
    adi->Exit(); // disconnect ADI-Interface
    ldkc_KbusInfo_Destroy();
    printf("\n getkbusinfo successful executed\n");
    return 0; // exit program

}//eof

