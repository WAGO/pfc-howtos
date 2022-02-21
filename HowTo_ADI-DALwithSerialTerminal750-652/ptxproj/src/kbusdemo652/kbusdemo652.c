//-----------------------------------------------------------------------------
//  Copyright (c) WAGO Kontakttechnik GmbH & Co. KG
//
//  PROPRIETARY RIGHTS are involved in the subject matter of this material.
//  All manufacturing, reproduction, use and sales rights pertaining to this
//  subject matter are governed by the license agreement. The recipient of this
//  software implicitly accepts the terms of the license.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
///  \file     kbusdemo652.c
///
///  \version  $Id:
///
///  \brief    Demo program for using serial interface terminal 750-652.
///            Kbus operates in application state "running" where each KBus-Cycle
///            have to be triggered manually with function "libpackbus_Push()".
///            Take care for a minimal calling interval of 40ms, otherwise
///            output modules WatchDog(50ms) will shut down outputs.
///
///  \author   cm : WAGO Kontakttechnik GmbH & Co. KG
///  \author   Th. Brandt : WAGO Kontakttechnik GmbH & Co. KG
//-----------------------------------------------------------------------------
// Prerequisites:
//   Take care that process "plclinux_rt" not running when run this demo.
//      Temporally:
//          >/etc/init.d/runtime stop
//      Permanent:
//          Remove symbolic link
//          >mv /etc/rc.d/S98_runtime /etc/rc.d/disabled/
//          Recover symbolic link
//          >mv /etc/rc.d/disabled/S98_runtime /etc/rc.d/
//
// IO-Node-Configuration:
//    - 750-82XX  Programable Fieldbus-Controller(PFC200)
//    - 750-652   Serial interface RS232/RS485
//    - 750-600   End-Module
//-----------------------------------------------------------------------------
// standard include files
//-----------------------------------------------------------------------------

#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
//-----------------------------------------------------------------------------
// include files for KBUS WAGO ADI
//-----------------------------------------------------------------------------
#include <dal/adi_application_interface.h>
#include "kbus652.h"

//-----------------------------------------------------------------------------
// defines and test setup
//-----------------------------------------------------------------------------

// priorities
#define KBUS_MAINPRIO 40       // main loop
// runtime of the test
#define KBUS_TESTTIME 120

//-----------------------------------------------------------------------------
// typedefs to make the whole process image more meaningful
//-----------------------------------------------------------------------------
struct typInputData
{
   //Analog PD
   tty652in tty_in;              //serial interface module 750-652 input data
   //Digital PD
}__attribute__ ((packed));

struct typOutputData
{
   //Analog PD
   tty652out tty_out;            //serial interface module 750-652 output data
   //Digital PD
}__attribute__ ((packed));


void onWrite652done(int result)
{
    printf("Data successful transfered to 750-652 over KBUS \n");
    return;
}

//-----------------------------------------------------------------------
///
/// variable definition
//
//-----------------------------------------------------------------------
const char * serfifo = "/tmp/fifo652";

//-----------------------------------------------------------------------
///
/// main of kbus demo program
///
/// \param none
///
//-----------------------------------------------------------------------
int main(void)
{
    // vars for ADI-interface
    tDeviceInfo deviceList[10];          // the list of devices given by the ADI
    size_t nrDevicesFound;               // number of devices found
    size_t nrKbusFound;                     // position of the kbus in the list
    tDeviceId kbusDeviceId;              // device ID from the ADI
    tApplicationDeviceInterface * adi;   // pointer to the application interface
    uint32_t taskId = 0;                 // task Id
    tApplicationStateChangedEvent event; // var for the event interface of the ADI

    // process image data
    struct typInputData pd_in;    // kbus input process data
    struct typOutputData pd_out;  // kbus output process data

    // 750-652 buffer
    tty652 tty;
    char *request;
    char response[BUFFERSIZE_652];

    // generic vars
    int i = 0, loops = 0;
    time_t last_t = 0, new_t;
    long unsigned runtime = 0;
    struct sched_param s_param;

    // file descriptor
    int fd=0;

    // startup info */
    printf("**************************************************\n");
    printf("***       750-652 KBUS Demo     V1.06          ***\n");
    printf("**************************************************\n");


    //create FIFO
    if ( mkfifo(serfifo, 0666) < 0 )
    {
        printf("Unable to create FIFO\n");
    }

    printf("Waiting for FIFO-receiver!\n");
    //open FIFO in blocking mode wait for receiver
    fd = open(serfifo, O_WRONLY|O_NONBLOCK,0);
    if (fd <= 0)
    {
       printf("Unable to open FIFO\n");
    }

    // clear process memory
    memset(&pd_in, 0, sizeof(pd_in));
    memset(&pd_out, 0, sizeof(pd_out));

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

   //prepare tty buffer for 750-652
   int ret_i = init652(&tty, &pd_in.tty_in, &pd_out.tty_out);
   if (ret_i < 0)
   {
      // prepare tty buffer for 750-652 failed
      printf("Prepare tty buffer for 750-652 failed\n");
        adi->CloseDevice(kbusDeviceId); // close kbus device
        adi->Exit(); // disconnect ADI-Interface
        return -10; // exit program
   }

    // run main loop for 120s ----------------------------------------------------------
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
      // Trigger Watchdog
      adi->WatchdogTrigger();

      // read inputs
      adi->ReadStart(kbusDeviceId, taskId); /* lock PD-In data */
      adi->ReadBytes(kbusDeviceId, taskId, 0, sizeof(pd_in), (uint8_t *) &pd_in);  /* read all input data*/
      adi->ReadEnd(kbusDeviceId, taskId); /* unlock PD-In data */

      // Call 'update652' on every KBUS cycle **************************************************************
      int ret_u = update652(&tty);
      switch(ret_u)
      {
          case -11 : printf("ERROR: Terminals receive buffer is full\n");break;
          case -12 : printf("ERROR: Terminals transmit buffer is full\n");break;
          case -13 : printf("ERROR: Parity error detected\n");break;
          case -14 : printf("ERROR: Framing error detected\n");break;
          case -15 : printf("ERROR: Overrun error detected\n");break;
      }

      loops++; //increment loop counter

      // 1s tick for test output
      new_t = time(NULL);
      if (new_t != last_t)
      {
         last_t = new_t;
         runtime++;

         // send data to serial interface every 5 seconds
         if (runtime%5 == 0)
         {
            request = "Hello world, greetings from pfc200!\n\r";
            int ret_w = write652(&tty,
                        request,
                        strlen(request),
                        onWrite652done);
            //int ret_w = write652(&tty, request, strlen(request), NULL);
            if (ret_w < 0)
            {
               printf(" Error on putting data into send buffer \n");
            }
            else if (ret_w == 0)
            {
               printf(" Send buffer occupied by another Tx job \n");
            }
            else if (ret_w > 0)
            {
               printf(" Tx Data written into local buffer \n");
            }
         }

         // Process received data from serial interface module 750-652
         int ret_r = read652(&tty, response, sizeof(response));
         if (ret_r > 0)
         {
            printf(" Received data: %s \n", response);
            //write received data also to fifo.
            if (fd > 0)
              write(fd, response , strlen(response)+1);
         }

      }
      // write all outputs
      adi->WriteStart(kbusDeviceId, taskId); /* lock PD-out data */
      adi->WriteBytes(kbusDeviceId,taskId,0,sizeof(pd_out),(uint8_t *) &pd_out); /* write all output data*/
      adi->WriteEnd(kbusDeviceId, taskId); /* unlock PD-out data */

    } // while ..

    // close kbus device
    adi->CloseDevice(kbusDeviceId);

    adi->Exit(); // disconnect ADI-Interface
    close(fd); //close file descriptor
    unlink(serfifo); //remove fifo
    return 0; // exit program
}//eof

