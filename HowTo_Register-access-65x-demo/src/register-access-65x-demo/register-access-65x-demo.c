/**
 * @file 
 * @brief Demo Programs for configuring serial terminals (750-650, 750-652) via adi/dal + libpackbus
 * 
 * IO-Node-Configuration
 * ---------------------
 *
 *  - 750-82XX      Progra*mable Fieldbus-Controller(PFC200)
 *  - 750-652       Serial interface RS232/RS485
 *  - 750-650-003   Serial interface RS232 (Adj.)
 *  - 750-650-003   Serial interface RS232 (Adj.)
 *  - 750-600       End-Module
 *
 * Prerequisites
 * -------------
 *
 * Take care that process "plclinux_rt" not running when run this demo.
 *  - Temporally:
 *    > /etc/init.d/runtime stop
 *  - Permanent:
 *     - Remove symbolic link
 *       >mv /etc/rc.d/S98_runtime /etc/rc.d/disabled/
 *     - Recover symbolic link
 *       >mv /etc/rc.d/disabled/S98_runtime /etc/rc.d/
 *
 * @author F. Warzecha: WAGO Kontakttechnik GmbH & Co. KG
 */

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
#include <libpackbus.h>

//
//-----------------------------------------------------------------------------
// include files for register API 
//-----------------------------------------------------------------------------
#include "kbus_register_settings.h"

//-----------------------------------------------------------------------------
// defines and test setup
//-----------------------------------------------------------------------------

// priorities
#define KBUS_MAINPRIO 40       

/**
 * @brief Print a configuration set.
 */
void kbus_reg_print_configuration(KbusRegConfigurationSet configuration) {
    printf("Operating Mode: %s\n", KBUS_REG_OPERATING_MODES[configuration.operating_mode]);
    printf("Baud rate: %s\n", KBUS_REG_BAUD_RATES[configuration.baud_rate]);
    printf("Databits: %s\n", KBUS_REG_DATABITS[configuration.databits]);
    printf("Parity: %s\n", KBUS_REG_PARITIES[configuration.parity]);
    printf("Stopbits: %s\n", KBUS_REG_STOPBITS[configuration.stopbits]);
    printf("Flowcontrol: %s\n", KBUS_REG_FLOWCONTROLS[configuration.flowcontrol]);
    printf("Continuous Send: %s\n", KBUS_REG_CONTINUOUS[configuration.continuous_send]);
    printf("Continuous Receive: %s\n", KBUS_REG_CONTINUOUS[configuration.continuous_receive]);
    printf("Continuous Receive Timeout: %s\n", KBUS_REG_CONTINUOUS_RECEIVE_TIMEOUTS[configuration.continuous_receive_timeout]);
    printf("Switchover Time RS-485: %s\n", KBUS_REG_SWITCHOVERS_RS485[configuration.switchover_time_rs485]);
    printf("Output Format: %s\n", KBUS_REG_OUTPUT_FORMATS[configuration.output_format]);
    printf("RTS/CTS: %s\n", KBUS_REG_RTS_CTS[configuration.rts_cts]);
    printf("XON/XOFF Send: %s\n", KBUS_REG_XON_XOFF[configuration.xon_xoff_send]);
    printf("XON/XOFF Receive: %s\n", KBUS_REG_XON_XOFF[configuration.xon_xoff_receive]);
    printf("Copy Status Byte: %s\n", KBUS_REG_COPY_STATUSBYTE[configuration.copy_statusbyte]);
}


/**
 * @brief print a short explanation of the error code.
 */
void print_error(KbusRegErr err) {
    switch (err.status) {
        case KBUS_REG_COM_OK:
            break;
        case KBUS_REG_COM_ERR:
            printf("A general error occurred\n");
            break;
        case KBUS_REG_COM_ERR_DAL: 
            switch (err.underlying_error) {
                case DAL_SUCCESS:
                    printf("BUG! DAL_SUCCESS handled as error!\n");
                    break;
                case DAL_FAILURE:
                    printf("adi/dal failure\n");
                    break;
                case DAL_NOTUSED:
                    printf("BUG: called an unimplemented adi/dal function\n");
                    break;
                default:
                    printf("An unknown adi/dal error occurred\n");
            }
            break;
        case KBUS_REG_COM_ERR_LIBPACKBUS:
            printf("libpackbus error: %i\n", err.underlying_error);
            break;
        case KBUS_REG_COM_UNKNOWN_TERMINAL:
            printf("Unknown terminal type\n");
            break;
        case KBUS_REG_COM_NO_MEM:
            printf("Memory allocation failed\n");
            break;
        case KBUS_REG_COM_INVALID_HANDLE:
            printf("Invalid settings handle passed\n");
            break;
        case KBUS_REG_ERR_SETTING:
            printf("Setting could not be applied. Status code: %u\n", err.underlying_error);
            break;
        default:
            printf("An unknown error occurred!\n");
    }
}

/**
 * @brief print the error, clean up kbus/adi and exit
 */
void handle_error(KbusRegErr err, tApplicationDeviceInterface *adi, tDeviceId kbusDeviceId) {
    print_error(err);
    
    adi->CloseDevice(kbusDeviceId); // close kbus device
    adi->Exit(); // disconnect ADI-Interface
    
    switch (err.status) {
        case KBUS_REG_COM_OK:
            exit(-1);
        case KBUS_REG_COM_ERR:
            exit(-2);
        case KBUS_REG_COM_ERR_DAL:
            exit(-3);
        case KBUS_REG_COM_ERR_LIBPACKBUS:
            exit(-4);
        case KBUS_REG_COM_UNKNOWN_TERMINAL:
            exit(-5);
        case KBUS_REG_COM_NO_MEM:
            exit(-6);
        case KBUS_REG_COM_INVALID_HANDLE:
            exit(-7);
        case KBUS_REG_ERR_SETTING:
            exit(-9);
        default:
            exit(-8);
    }
}


/**
 * @brief initialize kbus, read & write from/to registers
 */
int main(void)
{
    // vars for ADI-interface
    tDeviceInfo deviceList[10];          // the list of devices given by the ADI
    size_t nrDevicesFound;               // number of devices found
    size_t nrKbusFound;                     // position of the kbus in the list
    tDeviceId kbusDeviceId;              // device ID from the ADI
    tApplicationDeviceInterface * adi;   // pointer to the application interface
    
    struct sched_param s_param;
    
    // startup info
    printf("*************************************************************************\n");
    printf("***       750-652/750-650 KBUS Register Access Demo     V0.1          ***\n");
    printf("*************************************************************************\n");
    
    // connect to ADI-interface
    adi = adi_GetApplicationInterface();
    
    // init interface
    adi->Init();
    
    // scan devices
    adi->ScanDevices();
    adi->GetDeviceList(sizeof(deviceList), deviceList, &nrDevicesFound);
    
    // find kbus device
    nrKbusFound = -1;
    for (int i = 0; i < nrDevicesFound; ++i)
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
        return -3; // exit program
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
        return -3; // exit program
    }
    printf("KBUS device open OK\n");
    
    // error code
    KbusRegErr kbus_error; 
    
    // create configuration:
    KbusRegConfigurationSet configuration = KbusRegConfigurationSet_Default; // init with defaults
    configuration.baud_rate = KBUS_REG_BAUD_1200; // configure baudrate to 1200 baud
    
    // set configuration on terminal 1
    if ((kbus_error = kbus_reg_apply_configuration(adi, 1, configuration)).status != KBUS_REG_COM_OK) {
        handle_error(kbus_error, adi, kbusDeviceId); // print error and exit in case of failure
    }
    printf("Configuration:\n");
    kbus_reg_print_configuration(configuration);
    printf("=> Applied configuration\n\n");

    // declare configuration:
    KbusRegConfigurationSet configuration2;

    // extract configuration from terminal 2
    if ((kbus_error = kbus_reg_extract_configuration(adi, 2, &configuration2)).status != KBUS_REG_COM_OK) {
        handle_error(kbus_error, adi, kbusDeviceId); // print error and exit in case of failure
    }
    printf("Read configuration: \n");
    kbus_reg_print_configuration(configuration2);
    
    // close kbus device
    adi->CloseDevice(kbusDeviceId);
    
    printf("Closed device\n");
    
    adi->Exit(); // disconnect ADI-Interface
    return 0; // exit program
}
