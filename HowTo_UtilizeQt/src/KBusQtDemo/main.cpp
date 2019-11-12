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
extern "C" {
  #include <dal/adi_application_interface.h>
  #include <ldkc_kbus_information.h>
  #include <ldkc_kbus_register_communication.h>
}

//-----------------------------------------------------------------------------
// defines and test setup
//-----------------------------------------------------------------------------
#define DECOUTFILENAME "kbusinfo.h"
#define DEFOUTFILENAME "kbusinfo.c"
#define SUMMARYINFOFILENAME "kbusinfo.txt"

#define OISSOK                       0
#define EADIINITFAILED              -1
#define EADISCANDEVFAILED           -2
#define EADIGETDEVLISTFAILED        -3
#define ENOKBUSFOUND                -4
#define EKBUSOPENFAILED             -5
#define EUNCONFIGFAILED             -6
#define ELDKCKBINFOCREATEFAILED     -7
#define ELDKCKBINFOGETSTATUSFAILED  -8
#define ELDKCKBINFOGTTERMINFOFAILED -9
#define ELDKCKBINFOGTTERMLISTFAILED -10
#define EOPENDECFILEFAILED          -11
#define EOPENDEFFILEFAILED          -12
#define EOPENSUMFILEFAILED          -13
// priorities
#define KBUS_MAINPRIO 40       // main loop
// runtime of the test
#define KBUS_TESTTIME 30

int KBusDemo()
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

  usleep(1000);

  // close kbus device
  adi->CloseDevice(kbusDeviceId);

  adi->Exit(); // disconnect ADI-Interface

  return 0;
}

int GetKBusInfo()
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
    else
    {
      printf("Found Non-KBUS as device%i: %s \n", i, deviceList[i].DeviceName);
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
}

int getKBusHeader()
{
  // vars for ADI-interface
  tDeviceInfo deviceList[10];          // the list of devices given by the ADI
  size_t nrDevicesFound;               // number of devices found
  size_t nrKbusFound;          // position of the kbus in the list
  tDeviceId kbusDeviceId;              // device ID from the ADI
  tApplicationDeviceInterface * adi;   // pointer to the application interface
  tApplicationStateChangedEvent event; // var for the event interface of the ADI

  // generic vars
  int i = 0;
  int32_t result;

  // startup info */
  printf("*****************************************************\n");
  printf("***        getkbusheader                          ***\n");
  printf("***        Version: %12s %8s         ***\n", __DATE__, __TIME__);
  printf("*****************************************************\n");

  //*** Start KBUS ********************************************

  // connect to ADI-interface
  adi = adi_GetApplicationInterface();


  // init interface
  result = adi->Init();
  if (DAL_FAILURE == result)
  {
      printf("ERROR: ADI->Init failed\n");
      printf("Did you stop the plclinux task?\n");
      return EADIINITFAILED;
  };

  // scan devices
  result = adi->ScanDevices();
  if (DAL_FAILURE == result)
  {
      printf("ERROR: ADI->ScanDevices failed\n");
      return EADISCANDEVFAILED;
  };

  result = adi->GetDeviceList(sizeof(deviceList), deviceList, &nrDevicesFound);
  if (DAL_FAILURE == result)
  {
      printf("ERROR: ADI->GetDeviceList failed\n");
      return EADIGETDEVLISTFAILED;
  };


  // find kbus device
  nrKbusFound = -1;
  for (i = 0; i < nrDevicesFound; ++i)
  {
      if (strcmp(deviceList[i].DeviceName, "libpackbus") == 0)
      {
          nrKbusFound = i;
          printf("KBUS device found as device %i\n", i);
      }
      else
      {
        printf("Found Non-KBUS device: %s", deviceList[i].DeviceName);
      }
  }

  // kbus not found > exit
  if (nrKbusFound == -1)
  {
      printf("ERROR: No KBUS device found \n");
      adi->Exit();            // disconnect ADI-Interface
      return ENOKBUSFOUND;        // exit program
  }

  // open kbus device
  kbusDeviceId = deviceList[nrKbusFound].DeviceId;
  if (adi->OpenDevice(kbusDeviceId) != DAL_SUCCESS)
  {
      printf("ERROR: Kbus device open failed\n");

      adi->Exit();            // disconnect ADI-Interface
      return EKBUSOPENFAILED;     // exit program
  }
  printf("KBUS device open OK\n");

  // set application state to "Unconfigured" to let library drive kbus by them selve.
  // In this mode library set up a thread who drive the kbus cyclic.
  event.State = ApplicationState_Unconfigured;
  if (adi->ApplicationStateChanged(event) != DAL_SUCCESS)
  {
      // Set application state to "Unconfigured" failed
      printf("ERROR: Set application state to 'Unconfigured' failed\n");
      adi->CloseDevice(kbusDeviceId); // close kbus device
      adi->Exit();            // disconnect ADI-Interface
      return EUNCONFIGFAILED;     // exit programm
  }
  printf("Set application state to 'Unconfigured' \n");

  //*** Retrieve KBUS-Config via DBUS ***************************

  if (KbusInfo_Failed == ldkc_KbusInfo_Create())
  {
      printf("ERROR: ldkc_KbusInfo_Create() failed\n");
      adi->CloseDevice(kbusDeviceId); // close kbus device
      adi->Exit();            // disconnect ADI-Interface
      return ELDKCKBINFOCREATEFAILED;
  }

  // GetStatus
  tldkc_KbusInfo_Status status;
  if (KbusInfo_Failed == ldkc_KbusInfo_GetStatus(&status))
  {
      printf("ERROR: ldkc_KbusInfo_GetStatus() failed\n");
      adi->CloseDevice(kbusDeviceId); // close kbus device
      adi->Exit();            // disconnect ADI-Interface
      ldkc_KbusInfo_Destroy();
      return ELDKCKBINFOGETSTATUSFAILED;
  }
  printf("\n        .KbusBitCount: %i ",status.KbusBitCount);
  printf("\n        .TerminalCount: %i ",status.TerminalCount);
  printf("\n        .ErrorCode: %i ",status.ErrorCode);
  printf("\n        .ErrorArg: %i ",status.ErrorArg);
  printf("\n        .ErrorPos: %i ",status.ErrorPos);
  printf("\n        .BitCountAnalogInput: %i ",status.BitCountAnalogInput);
  printf("\n        .BitCountAnalogOutput: %i ",status.BitCountAnalogOutput);
  printf("\n        .BitCountDigitalInput: %i ",status.BitCountDigitalInput);
  printf("\n        .BitCountDigitalOutput: %i \n",status.BitCountDigitalOutput);

  // GetTerminalInfo
  size_t terminalCount;
  u16 terminals[LDKC_KBUS_TERMINAL_COUNT_MAX];
  tldkc_KbusInfo_TerminalInfo terminalDescription[LDKC_KBUS_TERMINAL_COUNT_MAX];

  if ( KbusInfo_Failed == ldkc_KbusInfo_GetTerminalInfo(OS_ARRAY_SIZE(terminalDescription), terminalDescription, &terminalCount) )
  {
      printf("ERROR: ldkc_KbusInfo_GetTerminalInfo() failed\n");
      adi->CloseDevice(kbusDeviceId); // close kbus device
      adi->Exit();            // disconnect ADI-Interface
      ldkc_KbusInfo_Destroy();
      return ELDKCKBINFOGTTERMINFOFAILED;
  }

  if ( KbusInfo_Failed == ldkc_KbusInfo_GetTerminalList(OS_ARRAY_SIZE(terminals), terminals, NULL) )
  {
      printf("ERROR: ldkc_KbusInfo_GetTerminalList() failed\n");
      adi->CloseDevice(kbusDeviceId); // close kbus device
      adi->Exit();            // disconnect ADI-Interface
      ldkc_KbusInfo_Destroy();
      return ELDKCKBINFOGTTERMLISTFAILED;
  }

  unsigned char ucMaxPosition;
  unsigned char ucPosition;
  unsigned char ucChanCtr;

  ucMaxPosition = terminalCount;

  // screeninfo (same as WAGOs getkbusinfo, but fancier)
  printf("\nPosition   Type   BitOffsetOut BitSizeOut BitOffsetIn BitSizeIn Channels PiFormat");
  for (ucPosition = 1; ucPosition <= ucMaxPosition; ucPosition++)
  {
      const u32 idx = ucPosition - 1;


      printf("\n %4i,", ucPosition);
      if( 0 == (terminals[idx] & 0x8000) )
      {
          // Show second part of order number in "dec" format for complex IO-Terminals
          printf("    %6i;", terminals[idx]);
      }
      else
      {
          // Show bit coded information of stupid digital IO-Terminals in "hex" format
          printf("    0x%x;", terminals[idx]);
      }
      printf("    %4i,       %4i,       %4i,       %4i,       %i,       %i",
              terminalDescription[idx].OffsetOutput_bits,
              terminalDescription[idx].SizeOutput_bits,
              terminalDescription[idx].OffsetInput_bits,
              terminalDescription[idx].SizeInput_bits,
              terminalDescription[idx].AdditionalInfo.ChannelCount,
              terminalDescription[idx].AdditionalInfo.PiFormat);
  }

  // Write to Files
  FILE * fPtr;
  time_t tmpTime;
  struct tm * tmPtr;
  int byteZahl;

  // Declaration File (aka Header File)
  printf("\n\nWriting declaration file %s\n",DECOUTFILENAME);
  fPtr = fopen(DECOUTFILENAME, "w");
  if (fPtr == NULL)
  {
      adi->CloseDevice(kbusDeviceId); // close kbus device
      adi->Exit();            // disconnect ADI-Interface
      ldkc_KbusInfo_Destroy();
      printf("ERROR: Cannot open outfile %s\n",DECOUTFILENAME);
      return EOPENDECFILEFAILED;
  }

  tmpTime = time(NULL);
  tmPtr = localtime(&tmpTime);

  fprintf(fPtr,"/******************************************************************************/\n//\n");
  fprintf(fPtr,"// Created by getkbusinfo at %d-%02d-%02d_%02d:%02d:%02d\n//\n",
          tmPtr->tm_year+1900,tmPtr->tm_mon+1,tmPtr->tm_mday,
          tmPtr->tm_hour,tmPtr->tm_min,tmPtr->tm_sec);
  fprintf(fPtr,"/******************************************************************************/\n\n");

  fprintf(fPtr,"#ifndef KBUSINFO_H\n#define KBUSINFO_H\n\n");

  fprintf(fPtr,"// In this array you will find all infos about the scanned kbus\n");
  fprintf(fPtr,"// Position, ModulType, BitOffsetOut, BitSizeOut, BitOffsetIn, BitSizeIn, Channels, PiFormat\n\n");
  fprintf(fPtr,"#define NROFKBUSMODULES %d\n",ucMaxPosition);
  fprintf(fPtr,"#define NROFINFOROWS 8\n");
  fprintf(fPtr,"extern const unsigned short kbusSimpleInfo[NROFKBUSMODULES][NROFINFOROWS];\n\n");

  // write the struct for the kbusInput
  fprintf(fPtr,"// The typedef struct for the scanned kbusInput\n");
  fprintf(fPtr,"typedef struct __attribute__((packed))\n{\n");

  // Byte before bit
  for (ucPosition = 1; ucPosition <= ucMaxPosition; ucPosition++)
  {
      const u32 idx = ucPosition - 1;
      if ((0 != terminalDescription[idx].SizeInput_bits) && (0 == (terminals[idx] & 0x8000)))
      {
          for (ucChanCtr = 1; ucChanCtr <= terminalDescription[idx].AdditionalInfo.ChannelCount;  ucChanCtr++)
          {
              byteZahl = terminalDescription[idx].SizeInput_bits / terminalDescription[idx].AdditionalInfo.ChannelCount / 8;
              switch (byteZahl)
              {
              case 1:
                  fprintf(fPtr,"\tunsigned char p%dt%dc%d;\t// Position %d, Typ 750-%d, Channel %d\n",
                      ucPosition, terminals[idx], ucChanCtr,
                      ucPosition, terminals[idx], ucChanCtr);
              case 2:
                  fprintf(fPtr,"\tunsigned short p%dt%dc%d;\t// Position %d, Typ 750-%d, Channel %d\n",
                      ucPosition, terminals[idx], ucChanCtr,
                      ucPosition, terminals[idx], ucChanCtr);
                  break;
              case 4:
                  fprintf(fPtr,"\tunsigned long p%dt%dc%d;\t// Position %d, Typ 750-%d, Channel %d\n",
                      ucPosition, terminals[idx], ucChanCtr,
                      ucPosition, terminals[idx], ucChanCtr);
                  break;
              // Complex modules having 1 channel and 6 to 48 Bytes
              default:
                  if ((byteZahl>= 6) && (byteZahl <= 48))
                  {
                      fprintf(fPtr,"\tunsigned char p%dt%dc%d[%d];\t// Position %d, Typ 750-%d, Channel %d\n",
                          ucPosition, terminals[idx], ucChanCtr, byteZahl,
                          ucPosition, terminals[idx], ucChanCtr);
                  }
                  // all the rest (3, 5 Bytes and > 48 Bytes) is blocked
                  else
                  {
                      printf("Error creating tKbusInput element for KBUS-Position %d\n", ucPosition);
                      fclose(fPtr);

                      adi->CloseDevice(kbusDeviceId); // close kbus device
                      adi->Exit(); // disconnect ADI-Interface
                      ldkc_KbusInfo_Destroy();
                      return -2;
                  }
                  break;
              }
          }
      }
  }
  // Now the bit Part

  for (ucPosition = 1; ucPosition <= ucMaxPosition; ucPosition++)
  {
      const u32 idx = ucPosition - 1;
      if ((0 != terminalDescription[idx].SizeInput_bits) && (0 != (terminals[idx] & 0x8000)))
      {
          for (ucChanCtr = 1; ucChanCtr <= terminalDescription[idx].SizeInput_bits;   ucChanCtr++)
          {

              fprintf(fPtr,"\tunsigned int p%dt4XXc%d:1;\t// Position %d, Typ 750-4XX, Channel %d\n",
                      ucPosition, ucChanCtr,
                      ucPosition, ucChanCtr);
          }
      }
  }

  fprintf(fPtr,"} tKbusInput;\n\n");


  // Create the struct for the kbusOutput
  fprintf(fPtr,"// The typedef struct for the scanned kbusOutput\n");
  fprintf(fPtr,"typedef struct __attribute__((packed))\n{\n");

  // Byte before bit
  for (ucPosition = 1; ucPosition <= ucMaxPosition; ucPosition++)
  {
      const u32 idx = ucPosition - 1;
      if ((0 != terminalDescription[idx].SizeOutput_bits) && (0 == (terminals[idx] & 0x8000)))
      {
          for (ucChanCtr = 1; ucChanCtr <= terminalDescription[idx].AdditionalInfo.ChannelCount;  ucChanCtr++)
          {
              byteZahl = terminalDescription[idx].SizeOutput_bits / terminalDescription[idx].AdditionalInfo.ChannelCount / 8;
              switch (byteZahl)
              {
              case 1:
                  fprintf(fPtr,"\tunsigned char p%dt%dc%d;\t// Position %d, Typ 750-%d, Channel %d\n",
                      ucPosition, terminals[idx], ucChanCtr,
                      ucPosition, terminals[idx], ucChanCtr);
                  break;
              case 2:
                  fprintf(fPtr,"\tunsigned short p%dt%dc%d;\t// Position %d, Typ 750-%d, Channel %d\n",
                      ucPosition, terminals[idx], ucChanCtr,
                      ucPosition, terminals[idx], ucChanCtr);
                  break;
              case 4:
                  fprintf(fPtr,"\tunsigned long p%dt%dc%d;\t// Position %d, Typ 750-%d, Channel %d\n",
                      ucPosition, terminals[idx], ucChanCtr,
                      ucPosition, terminals[idx], ucChanCtr);
                  break;
              default:
                  // Complex modules having 1 channel and 6 to 48 Bytes
                  if ((byteZahl>= 6) && (byteZahl <= 48))
                  {
                      fprintf(fPtr,"\tunsigned char p%dt%dc%d[%d];\t// Position %d, Typ 750-%d, Channel %d\n",
                          ucPosition, terminals[idx], ucChanCtr, byteZahl,
                          ucPosition, terminals[idx], ucChanCtr);
                  }
                  // all the rest (3, 5 Bytes and > 48 Bytes) is blocked
                  else
                  {
                      printf("Error creating tKbusOutput element for KBUS-Position %d\n", ucPosition);

                      fclose(fPtr);

                      adi->CloseDevice(kbusDeviceId); // close kbus device
                      adi->Exit(); // disconnect ADI-Interface
                      ldkc_KbusInfo_Destroy();
                      return -2;
                  }
                  break;
              };
          }
      }
  }

  // Now the bit Part
  for (ucPosition = 1; ucPosition <= ucMaxPosition; ucPosition++)
  {
      const u32 idx = ucPosition - 1;
      if ((0 != terminalDescription[idx].SizeOutput_bits) && (0 != (terminals[idx] & 0x8000)))
      {
          for (ucChanCtr = 1; ucChanCtr <= terminalDescription[idx].SizeOutput_bits;  ucChanCtr++)
          {

              fprintf(fPtr,"\tunsigned int p%dt5XXc%d:1;\t// Position %d, Typ 750-5XX, Channel %d\n",
                      ucPosition, ucChanCtr,
                      ucPosition, ucChanCtr);
          }
      }
  }

  fprintf(fPtr,"} tKbusOutput;\n\n");

  fprintf(fPtr,"// The byte offset of the bit input field in tKbusInput\n");
  fprintf(fPtr,"#define BYTEOFFSETINPUTBITFIELD %d\n\n",status.BitCountAnalogInput/8);
  fprintf(fPtr,"// The byte offset of the bit output field in tKbusOutput\n");
  fprintf(fPtr,"#define BYTEOFFSETOUTPUTBITFIELD %d\n\n",status.BitCountAnalogOutput/8);

  fprintf(fPtr,"// The structs and the pointer to byte- and bitfields\n");
  fprintf(fPtr,"extern tKbusInput kbusInputData;\n");
  fprintf(fPtr,"extern tKbusOutput kbusOutputData;\n\n");

  fprintf(fPtr,"extern uint8_t * pKbusInputBitData;\n");
  fprintf(fPtr,"extern uint8_t * pKbusInputData;\n\n");

  fprintf(fPtr,"extern uint8_t * pKbusOutputBitData;\n");
  fprintf(fPtr,"extern uint8_t * pKbusOutputData;\n\n");

  fprintf(fPtr,"#endif // KBUSINFO_H\n");

  fclose(fPtr);

  printf("\nWriting definition file %s\n",DEFOUTFILENAME);

  // Now definition stuff in a C-File
  fPtr = fopen(DEFOUTFILENAME, "w");
  if (fPtr == NULL)
  {
      adi->CloseDevice(kbusDeviceId); // close kbus device
      adi->Exit(); // disconnect ADI-Interface
      ldkc_KbusInfo_Destroy();

      printf("ERROR: Cannot open outfile %s\n",DEFOUTFILENAME);
      return EOPENDEFFILEFAILED; // exit program
  }

  tmpTime = time(NULL);
  tmPtr = localtime(&tmpTime);

  fprintf(fPtr,"/******************************************************************************/\n//\n");
  fprintf(fPtr,"// Created by getkbusinfo at %d-%02d-%02d_%02d:%02d:%02d\n",
          tmPtr->tm_year+1900,tmPtr->tm_mon+1,tmPtr->tm_mday,
          tmPtr->tm_hour,tmPtr->tm_min,tmPtr->tm_sec);
  fprintf(fPtr,"// see %s for further information\n//\n", DECOUTFILENAME);
  fprintf(fPtr,"/******************************************************************************/\n\n");

  fprintf(fPtr,"#include \"%s\"\n\n",DECOUTFILENAME);
  fprintf(fPtr,"const unsigned short kbusSimpleInfo[NROFKBUSMODULES][NROFINFOROWS] = {\n");
  fprintf(fPtr,"//Position   Type   BitOffsetOut BitSizeOut BitOffsetIn BitSizeIn Channels PiFormat\n");

  for (ucPosition = 1; ucPosition <= ucMaxPosition; ucPosition++)
  {
      const u32 idx = ucPosition - 1;


      fprintf(fPtr," {%4i,   ", ucPosition);
      if( 0 == (terminals[idx] & 0x8000) )
      {
          // Show second part of order number in "dec" format for complex IO-Terminals
          fprintf(fPtr,"  %6i, ", terminals[idx]);
      }
      else
      {
          // Show bit coded information of stupid digital IO-Terminals in "hex" format
          fprintf(fPtr,"  0x%x, ", terminals[idx]);
      }
      fprintf(fPtr,"   %4i,       %4i,       %4i,      %4i,       %i,       %i}",
              terminalDescription[idx].OffsetOutput_bits,
              terminalDescription[idx].SizeOutput_bits,
              terminalDescription[idx].OffsetInput_bits,
              terminalDescription[idx].SizeInput_bits,
              terminalDescription[idx].AdditionalInfo.ChannelCount,
              terminalDescription[idx].AdditionalInfo.PiFormat);
      if (ucPosition != ucMaxPosition)
      {
          fprintf(fPtr,",\n");
      }
  }
  fprintf(fPtr,"\n};\n\n");

  fprintf(fPtr,"tKbusInput kbusInputData;\n");
  fprintf(fPtr,"tKbusOutput kbusOutputData;\n\n");

  fprintf(fPtr,"uint8_t * pKbusInputData;\n");
  fprintf(fPtr,"uint8_t * pKbusInputBitData;\n\n");

  fprintf(fPtr,"uint8_t * pKbusOutputData;\n");
  fprintf(fPtr,"uint8_t * pKbusOutputBitData;\n\n");

  fclose(fPtr);

  // Now the summary in a simple Ascii-File (target of this file is a simple machine readable form)
  printf("\nWriting Summary file %s\n",SUMMARYINFOFILENAME);
  fPtr = fopen(SUMMARYINFOFILENAME, "w");
  if (fPtr == NULL)
  {
      adi->CloseDevice(kbusDeviceId); // close kbus device
      adi->Exit(); // disconnect ADI-Interface
      ldkc_KbusInfo_Destroy();

      printf("ERROR: Cannot open outfile %s\n",SUMMARYINFOFILENAME);
      return EOPENSUMFILEFAILED; // exit program
  }

  tmpTime = time(NULL);
  tmPtr = localtime(&tmpTime);

  fprintf(fPtr,"%%******************************************************************************\n%%\n");
  fprintf(fPtr,"%% Created by getkbusheader at %d-%02d-%02d_%02d:%02d:%02d\n",
          tmPtr->tm_year+1900,tmPtr->tm_mon+1,tmPtr->tm_mday,
          tmPtr->tm_hour,tmPtr->tm_min,tmPtr->tm_sec);
  fprintf(fPtr,"%% see %s for further information\n%%\n", DECOUTFILENAME);
  fprintf(fPtr,"%%******************************************************************************\n\n");

  fprintf(fPtr,"%%Position   Type   BitOffsetOut BitSizeOut BitOffsetIn BitSizeIn Channels PiFormat\n");

  for (ucPosition = 1; ucPosition <= ucMaxPosition; ucPosition++)
  {
      const u32 idx = ucPosition - 1;

      fprintf(fPtr,"%4i      %6i       %4i       %4i       %4i       %4i        %i        %i\n",
              ucPosition,
              terminals[idx],
              terminalDescription[idx].OffsetOutput_bits,
              terminalDescription[idx].SizeOutput_bits,
              terminalDescription[idx].OffsetInput_bits,
              terminalDescription[idx].SizeInput_bits,
              terminalDescription[idx].AdditionalInfo.ChannelCount,
              terminalDescription[idx].AdditionalInfo.PiFormat);
  }

  fclose(fPtr);

  adi->CloseDevice(kbusDeviceId); // close kbus device
  adi->Exit();            // disconnect ADI-Interface
  ldkc_KbusInfo_Destroy();

  printf("\ngetkbusinfo successful executed\n");

  return OISSOK; // exit program
}

int main(int argc, char* argv[])
{
  int program;

  printf("You have to stop CoDeSys-Runtime *plclinux_rt* before running this demo \n");
  printf("With kill <pid> *plclinux_rt*\n");
  printf("OR /etc/init.d/runtime stop \n");

  if(argc > 1)
  {
    program = *argv[1] - 48;
  }
  else
  {
    printf("The first Argument has to be a number with the Programm you want to start.\n");
    printf("Available programs:\n");
    printf("1: KBusDemo,  Operating mode *running* \n");
    printf("2: GetKBusInfo, Using DBUS to retrieve Kbus details \n");
    printf("3: GetKBusHeader, Build suitable header files for connected IO-Terminals \n");

    return -1;
  }


  switch (program) {
    case 1:
      return KBusDemo();

    case 2:
      return GetKBusInfo();

    case 3:
      return getKBusHeader();

    default:
      printf("Kein Demo-Programm gefunden mit ID: %d", program);
      return 0;
  }
}
