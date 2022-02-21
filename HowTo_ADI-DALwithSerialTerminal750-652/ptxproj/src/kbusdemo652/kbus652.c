//-----------------------------------------------------------------------------
///  Copyright (c) WAGO Kontakttechnik GmbH & Co. KG
///
///  PROPRIETARY RIGHTS are involved in the subject matter of this material.
///  All manufacturing, reproduction, use and sales rights pertaining to this
///  subject matter are governed by the license agreement. The recipient of this
///  software implicitly accepts the terms of the license.
///-----------------------------------------------------------------------------
///  \file     kbus652.c
///
///  \version  1.02
///
///  \brief    A set of functions for using serial interface terminal 750-652.
///            Terminal 750-652 use 6, 22 or 46 bytes in process image for data exchange.
///            Data exchange controlled/monitored by 2 additional bytes.
///
///            Set "PISIZE_652" depending on terminals current data width.
///
///            Use tool "Wago-IO-Check.exe" to configure terminals data width:
///            - " 8byte": 2 control/status bytes +  6 data bytes(default)
///            - "24byte": 2 control/status bytes + 22 data bytes
///            - "48byte": 2 control/status bytes + 46 data bytes
///            Use tool "Wago-IO-Check.exe" to configure terminals:
///            - Operating-Mode: RS232, RS422 or RS485(default)
///            - Baudrate, Databytes, Parity, Stopbits, FlowControl, Duplex-Mode
///
///  \author   cm : WAGO Kontakttechnik GmbH & Co. KG
//----------------------------------------------------------------------------- 

//-----------------------------------------------------------------------------
// standard include files
//-----------------------------------------------------------------------------
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

//-----------------------------------------------------------------------------
// special include files
//-----------------------------------------------------------------------------
#include "kbus652.h"

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------

int init652(tty652 *tty, tty652in *in, tty652out *out)
{
    memset(tty, 0, sizeof(tty652));
    tty->pIn = in;
    tty->pOut = out;
    tty->status = TTY_INIT;
    return 0;
}

int read652(tty652 *tty, char *s, size_t n)
{
    int i, responseLength;

    if (tty->status == TTY_INIT)
    {
      return 0; //Terminal initialisation in progrees - Try later again
    }
    responseLength = tty->rxCount;
    //Copy rx data into given buffer
    for (i=0; i<responseLength; i++)
    {
        s[i] = tty->rxBuffer[i];
    }
    s[responseLength] = 0x00;
    // Free receive buffer
    tty->rxCount = 0;
    tty->rxBuffer[0] = 0x00;
    return responseLength;
}

int write652(tty652 *tty, char *s, size_t n,  done652 cb)
{
   int i;
   //Check if another tx operation is pending
   if (tty->status != TTY_IDLE)
   {
      return 0; //Channel already in use - Try later again 
   }
   //Check local buffer size
   if (n >= BUFFERSIZE_652)
   {
       return -1; //Local transmit buffer to small
   }
   //Register callback function
   tty->txDone = cb;
   //Copy tx data into local buffer
   for (i=0; i<n; i++)
   {
      tty->txBuffer[i] = s[i];
   }
   // new Tx job
   tty->txCount = n;
   tty->txCountAct = 0;
   tty->status = TTY_PENDING;
   return n;
}

int update652(tty652 *tty)
{
   int bytesToProcessTx;
   int bytesToProcessRx;
   int i;
   ///char help[PISIZE_652];


   // process terminal initialisation and tx jobs
   switch (tty->status)
   {

       case TTY_INIT : // Request terminal initialisation
           tty->pOut->crtl0_InitReq = 1;
           if (tty->pIn->stat0_InitAck == 1) // Wait for terminal initialisation finshed
           {
               tty->pOut->crtl0_InitReq = 0;
               tty->status = TTY_IDLE; // Enable rx/tx
           }
           break;

       case TTY_PENDING : // process Tx job in chunks
         bytesToProcessTx = tty->txCount - tty->txCountAct;
         if (bytesToProcessTx >= PISIZE_652)
         {
            bytesToProcessTx = PISIZE_652;
         }
         //Copy payload data
         for (i=0; i<bytesToProcessTx; i++)
         {
            tty->pOut->data[i] = tty->txBuffer[tty->txCountAct +i];
         }
         //Set number of bytes to transmit within the next kbus cycle
         tty->pOut->crtl0_TxCountLow = (bytesToProcessTx & 0x07);
         tty->pOut->crtl1_TxCountHigh = (bytesToProcessTx & 0x38) >> 3;
         //Toggle transmit request flag
         tty->pOut->crtl0_TxReq = !tty->pOut->crtl0_TxReq;
         //Remember already processed bytes
         tty->txCountAct += bytesToProcessTx;
         tty->status = TTY_WAITACK;
         break;

      case TTY_WAITACK : // wait for chunk transmitted
         if (tty->pIn->stat0_TxAck == tty->pOut->crtl0_TxReq)
         {
            //check if tx job is done
            if ((tty->txCount - tty->txCountAct) <= 0)
            {
                // Call callback if configured
                if(tty->txDone != NULL)
               {
                   tty->txDone(tty->txCount);
               }
               //Reset rx buffer, ignore already received bytes
               tty->rxCount = 0;
               tty->rxBuffer[0] = 0x00;
               //Get ready for the next job
               tty->status = TTY_IDLE;
            }
            else
            {
               tty->status = TTY_PENDING;
               ///printf("TxMore \n");
            }
         }
         break;

   }

   if (tty->status != TTY_INIT)
   {
       //Check terminal status flags
       if (tty->pIn->stat0_RxBufFull)
           return -11; // Terminals receive buffer is full
       if (tty->pIn->stat1_TxBufFull)
           return -12; // Terminals transmit buffer is full
       if (tty->pIn->stat1_ErrorFrame)
           return -14; // Framing error detected
       if (tty->pIn->stat1_ErrorOverrun)
           return -15; // Overrun error detected
       // Check if receive data available
       if (tty->pIn->stat0_RxReq != tty->pOut->crtl0_RxAck)
       {
           //Calculate number of received bytes to process
           bytesToProcessRx = (tty->pIn->stat1_RxCountHigh << 3) | tty->pIn->stat0_RxCountLow;
           //Check receive buffer is big enough
           if((tty->rxCount + bytesToProcessRx) >= BUFFERSIZE_652)
           {
               return -21; //Local receive buffer to small
           }
           //Copy received data into local buffer
           for (i=0; i<bytesToProcessRx; i++)
           {
               tty->rxBuffer[tty->rxCount +i] = tty->pIn->data[i];
           }
           tty->rxCount += bytesToProcessRx;
           tty->rxBuffer[tty->rxCount] = 0x00;
           //Toggle acknowledge flag to mark rx job as done
           tty->pOut->crtl0_RxAck = tty->pIn->stat0_RxReq;
       }
   }
   return 0;
}

//eof

