//-----------------------------------------------------------------------------
//  Copyright (c) WAGO Kontakttechnik GmbH & Co. KG
//
//  PROPRIETARY RIGHTS are involved in the subject matter of this material.
//  All manufacturing, reproduction, use and sales rights pertaining to this
//  subject matter are governed by the license agreement. The recipient of this
//  software implicitly accepts the terms of the license.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
///  \file     kbusdemo2.c
///
///  \version  $Id:
///
///  \brief    Simple demo program for using kbus ADI interface .
///            Kbus operates in application state "running" who give you the
///            full control about KBus behaivior. 
///            In this mode each KBus-Cycle have to be triggered manually,
///            with function "libpackbus_Push".
///            Take care for a minimal calling interval of 40ms, otherwise
///            output modules WatchDog(50ms) will shut down outputs.
///
///  \author   cm : WAGO Kontakttechnik GmbH & Co. KG
///  \author   Th. Brandt : WAGO Kontakttechnik GmbH & Co. KG
//----------------------------------------------------------------------------- 
// Prerequisites:
//   Take care that the runtime which is claiming the kbus is
//   not running when run this demo.
//      Temporally:
//          >/etc/init.d/runtime stop
//      Permanent:
//          Delete symbolic link
//          >rm /etc/rc.d/S98_runtime
//          Recover symbolic link
//          >ln -s /etc/init.d/runtime /etc/rc.d/S98_runtime
//
// IO-Node-Configuration:
//    - 750-8206    Programable Fieldbus-Controller(PFC200)
//    - 750-516     4DO
//    - 750-402     4DI
//    - 750-600     End-Module
//-----------------------------------------------------------------------------
// standard include files
//-----------------------------------------------------------------------------

#include <time.h>
#include <sched.h>
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
// defines and test setup
//-----------------------------------------------------------------------------

// priorities
#define KBUS_MAINPRIO 40       // main loop
// runtime of the test
#define KBUS_TESTTIME 30

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
    uint32_t taskId = 0;                 // task Id 
    tApplicationStateChangedEvent event; // var for the event interface of the ADI

    // process data
    uint8_t pd_in[4096];    // kbus input process data
    uint8_t pd_out[4096];   // kbus output process data

    // generic vars
    int i = 0, loops = 0;
    time_t last_t = 0, new_t;
    long unsigned runtime = 0;
    struct sched_param s_param;

    // startup info */
    printf("**************************************************\n");
    printf("***       KBUS Demo Application V3.00          ***\n");
    printf("**************************************************\n");

    // clear process memory
    memset(pd_in, 0, sizeof(pd_in));
    memset(pd_out, 0, sizeof(pd_out));

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

    // switch to RT Priority
    s_param.sched_priority = KBUS_MAINPRIO;
    sched_setscheduler(0, SCHED_FIFO, &s_param);
    printf("switch to RT Priority 'KBUS_MAINPRIO'\n");

    // open kbus device 
    kbusDeviceId = deviceList[nrKbusFound].DeviceId;
    if (adi->OpenDevice(kbusDeviceId) != DAL_SUCCESS)
    {
        printf("Kbus device open failed\n");
        adi->Exit(); // disconnect ADI-Interface
        return -2; // exit program
    }
    printf("KBUS device open OK\n");


    // Set application state to "Running" to drive kbus by your selve.
    event.State = ApplicationState_Running;
    if (adi->ApplicationStateChanged(event) != DAL_SUCCESS)
    {
        // Set application state to "Running" failed
        printf("Set application state to 'Running' failed\n");
        adi->CloseDevice(kbusDeviceId); // close kbus device
        adi->Exit(); // disconnect ADI-Interface
        return -3; // exit programm
    }
    printf("Set application state to 'Running' \n");

    // run main loop for 30s
    while (runtime < KBUS_TESTTIME)
    {
        usleep(10000); // wait 10 ms

        uint32_t retval = 0;

        // Use function "libpackbus_Push" to trigger one KBUS cycle.
        if (adi->CallDeviceSpecificFunction("libpackbus_Push", &retval) != DAL_SUCCESS)
        {
            // CallDeviceSpecificFunction failed
            printf("CallDeviceSpecificFunction failed\n");
            adi->CloseDevice(kbusDeviceId); // close kbus device
            adi->Exit(); // disconnect ADI-Interface
            return -4; // exit programm
        }

        if (retval != DAL_SUCCESS)
        {
            // Function 'libpackbus_Push' failed
            printf("Function 'libpackbus_Push' failed\n");
            adi->CloseDevice(kbusDeviceId); // close kbus device
            adi->Exit(); // disconnect ADI-Interface
            return -5; // exit programm
        }

        loops++;

        // Trigger Watchdog
        adi->WatchdogTrigger();

        // 1s tick for test output
        new_t = time(NULL);
        if (new_t != last_t)
        {
          last_t = new_t;
          runtime++;

          // read inputs
          adi->ReadStart(kbusDeviceId, taskId);     /* lock PD-In data */
          adi->ReadBytes(kbusDeviceId, taskId, 0, 1, (uint8_t *) &pd_in[0]);  /* read 1 byte from address 0 */
          adi->ReadEnd(kbusDeviceId, taskId); /* unlock PD-In data */
          // calculate something
          pd_out[0] += 1;
          // write outputs
          adi->WriteStart(kbusDeviceId, taskId); /* lock PD-out data */
          adi->WriteBytes(kbusDeviceId,taskId,0,1,(uint8_t *) &pd_out[0]); /* write */
          adi->WriteBytes(kbusDeviceId, taskId, 0, 16, (uint8_t *) &pd_out[0]); /* write */
          adi->WriteEnd(kbusDeviceId, taskId); /* unlock PD-out data */
          // print info
          printf("%lu:%02lu:%02lu State = ",runtime/3600ul,(runtime/60ul)%60ul,runtime%60ul);
          // show loops/s
          printf("\n        Loop/s = %i ",loops);
          loops = 0;
          // show process data
          printf(" Input Data = %02X Output data = %02X ",(int) pd_in[0],(int) pd_out[0]);
          printf("\n");
        } 

    } // while ..

    // close kbus device
    adi->CloseDevice(kbusDeviceId);

    adi->Exit(); // disconnect ADI-Interface
    return 0; // exit program
}//eof

