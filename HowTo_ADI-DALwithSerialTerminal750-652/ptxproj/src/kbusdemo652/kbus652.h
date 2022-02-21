//-----------------------------------------------------------------------------
///  Copyright (c) WAGO Kontakttechnik GmbH & Co. KG
///
///  PROPRIETARY RIGHTS are involved in the subject matter of this material.
///  All manufacturing, reproduction, use and sales rights pertaining to this
///  subject matter are governed by the license agreement. The recipient of this
///  software implicitly accepts the terms of the license.
///-----------------------------------------------------------------------------
///  \file     kbus652.h
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

// Number of data bytes in process image of serial interface module 750-652
#define PISIZE_652 6 //Total 8 bytes in process image, 2 control/status + 6 data

// Local buffer size for rx/tx data of serial interface module 750-652
#define BUFFERSIZE_652 1024

//-- Struct: tty652in ---------------------------------------------------
///
/// Representation of 750-652 terminal in input process image
//
typedef struct 
{
   unsigned short stat0_TxAck:1;   // Data to transmit processed
   unsigned short stat0_RxReq:1;   // Received data are available
   unsigned short stat0_InitAck:1; // Module initialization is done
   unsigned short stat0_RxCountLow:3; // Number of received bytes "low"
   unsigned short stat0_RxBufFull:1;  // Terminals receive buffer is full
   unsigned short stat1_RxCountHigh:3; // Number of received bytes "high"
   unsigned short stat1_TxBufFull:1;  // Terminals transmit buffer is full
   unsigned short stat1_ErrorParity:1; // Parity error detected
   unsigned short stat1_ErrorFrame:1;  // Framing error detected
   unsigned short stat1_ErrorOverrun:1; // Overrun error detected
   unsigned short :0;
   char data[PISIZE_652];   // Bytes in input process image for data exchange
}__attribute__((packed)) tty652in;

//-- Struct: tty652out ---------------------------------------------------
///
/// Representation of 750-652 terminal in output process image
//
typedef struct 
{
   unsigned short crtl0_TxReq:1;   // Transmit data request
   unsigned short crtl0_RxAck:1;   // Received data processed
   unsigned short crtl0_InitReq:1; // Init module request
   unsigned short crtl0_ConSend:1;  // Use "Continuous Send"
   unsigned short crtl0_TxCountLow:3; // Number of bytes to transmit "low"
   unsigned short crtl0_RegCom:1;   // Use "Register communication"
   unsigned short crtl1_TxCountHigh:3; // Number of bytes to transmit "high"
   unsigned short crtl1_Led0:2;
   unsigned short crtl1_Led1:2;
   unsigned short :0;
   char data[PISIZE_652];   //Bytes in output process image for data exchange
}__attribute__((packed)) tty652out;

//-- Enum: Status652 ------------------------------------------------
///
///
//
enum Status652
{
    TTY_INIT,   //Terminal initialisation in progress, rx/tx disabled
    TTY_IDLE,   //Wait for something to transmit, receiving enabled
    TTY_PENDING,//More data to transmit, receiving enabled
    TTY_WAITACK //Wait for TX-ACK of last transmitted chunk, receiving enabled
};

//-- Callback: write652done--------------------------------------------
///
///  Callback for data transfered to 750-652 via kbus
//
typedef void (*done652)(int);

//-- Struct: tty652 ---------------------------------------------------
///
///  Context object of a 750-652 terminal
//
typedef struct 
{
    int status;                   //Current terminal state
    tty652in *pIn;                //Position of serial interface module 750-652 in input process image
    tty652out *pOut;              //Position of serial interface module 750-652 in output process image
    int rxCount;                  //Total number of received bytes
    char rxBuffer[BUFFERSIZE_652];//Local buffer - containing received bytes
    int txCount;                  //Total number of bytes to transmit
    int txCountAct;               //Number of already transmitted bytes
    char txBuffer[BUFFERSIZE_652];//Local buffer - containing bytes to transmit
    done652 txDone;               //Callback for data transfered to 750-652 via kbus
}tty652;

//-- Function: init652 ---------------------------------------------------
///
///  Assigns position of 750-652 in process image to context object "tty"
///
///  \param tty      Context object
///  \param in       Absolute position of 750-652 terminal in input process
///  \param out      Absolute position of 750-652 terminal in output process
///
///   \return 0 on success; -1 on error
//
int init652(tty652 *tty, tty652in *in, tty652out *out);


//-- Function: read652 ---------------------------------------------------------
///
///  Copy received bytes and clear buffer afterwarts
///
///  \param tty     Pointer to context object
///  \param s       Destination
///  \param n       Size of destination(max bytes to read)
///
/// \return
///    >=0 Number of received bytes
///    <0 error
//
int read652(tty652 *tty, char *s, size_t n);

//-- Function: write652 ---------------------------------------------------
///
/// This function copy "data to transmit" into the local txBuffer.
/// Only if txChannel is in state "TTY_IDLE".
///
///  \param tty       Pointer to context object
///  \param s         Data to write
///  \param n         Number of bytes to write
///  \param cb        Callback on data transmitted via KBUS, or NULL
///
/// \return
///    >0 number of bytes written
///    ==0 Channel busy, try again later
///    <0 error
//
int write652(tty652 *tty, char *s, size_t n, done652 cb);

//-- Function: update652 ---------------------------------------------------
///
///  This function read/write local buffer data from/to KBUS.
///  Call this function each time you call "libpackbus_Push" to trigger a KBUS cycle.
///
///  \param tty     Pointer to context object
///
///  \return 0 on success; -1 on error
//
int update652(tty652 *tty);

//eof

