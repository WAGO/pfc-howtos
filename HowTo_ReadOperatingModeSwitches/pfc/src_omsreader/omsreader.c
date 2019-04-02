//-----------------------------------------------------------------------------
// MIT License

// Copyright (c) 2018 WAGO Kontakttechnik GmbH & Co. KG

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
///  \file     omsreader.c
///
///  \version  $Id:
///
///  \brief   C program using liboms for reading START-STOP-RESET switch
///           in OMS_MODE_PASSIVE where application polls current states.
///           It will deliver the current state as return value.
///
///  \notes
///
///  \author   Thomas Brandt : WAGO Kontakttechnik GmbH & Co. KG
//-----------------------------------------------------------------------------
 
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <wago_oms_API.h>

#define BUTTON_ERROR     -1
#define BUTTON_RUN       0x01
#define BUTTON_STOP      0x02
#define BUTTON_RESET     0x08
#define BUTTON_RESET_ALL 0x80

int main(void) {

    // generic vars
    int32_t switchState  = 0;
    uint8_t button_state = 0;

    tOmsReturn retval; //Unused retval of omsDev->Get..()

    // Open in passive mode, active polling for current state of RUN-STOP-RESET switch
    tOmsDevice *omsDev = oms_OpenDevice("/dev/input/event0", OMS_MODE_PASSIVE);
    if(omsDev == NULL)
    {
        //printf("ERROR: oms_OpenDevice() failes\n");
        return BUTTON_ERROR;
    }

    // Check RUN-STOP switch for position "RUN"
    if(OMS_RETURN_OK == omsDev->GetRunKey(omsDev, &switchState))
    {
        if(switchState == 1) // Switch in position "RUN"
        {
            button_state = BUTTON_RUN;
        }
    }

    // Check RUN-STOP switch for position "STOP"
    if(OMS_RETURN_OK == omsDev->GetStopKey(omsDev, &switchState))
    {
        if(switchState == 1)// Switch in position "STOP"
        {
            button_state = BUTTON_STOP;
        }
    }

    // Check RESET switch
    if(OMS_RETURN_OK == omsDev->GetResetKey(omsDev, &switchState))
    {
        if(switchState == 1) // Switch in position "RESET"
        {
            button_state = BUTTON_RESET;
        }
    }

    // Check RST_ALL button
    if(OMS_RETURN_OK == omsDev->GetRstAllKey(omsDev, &switchState))
    {
        if(switchState == 1) // Button RST_ALL is pressed
        {
            button_state |= BUTTON_RESET_ALL;
        }
    }

    oms_CloseDevice(omsDev);
    switch (button_state & ~BUTTON_RESET_ALL) //check for Main-switch only! So remeove RESET_ALL
    {
        case BUTTON_RUN:
        printf("RUN");
        break;
        case BUTTON_STOP:
        printf("STOP");
        break;
        case BUTTON_RESET:
        printf("RESET");
        break;
        default:
        printf("ERROR");
        break;
    }

    //if also BUTTON_RESET_ALL is active add this to output
    if (button_state & BUTTON_RESET_ALL)
    {
        printf("-RESET_ALL");
    }

    printf("\n"); //flush to stdout
    return 0;
}

