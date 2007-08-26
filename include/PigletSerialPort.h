/**
** Piglet Productions
**
** FileName       : PigletSerialPort.h
**
** Defines        : PigletSerialPort
**
** Description
**
** Defines a class which can handle a PC serial port, complete with input
** and output routines and buffering.
**
**
** Initial Coding : Colin Turner
**
** Date           : 17th March 1999
**
**
** Copyright applies on this file, and distribution may be limited.
** Copyright Colin Turner 1999, distribution is limited.
*/

/*
** Revision 1.00
**
*/

#ifndef PIGLET_SERIALPORT_H
#define PIGLET_SERIALPORT_H

#include <conio.h>
#include <stddef.h>
#include <time.h>

// Errors

#define PigletSerialError_TimeOut       1
#define PigletSerialError_RxError       2
#define PigletSerialError_TotalTimeOut  3

void Delay(unsigned long milli);

class PigletSerialPort;

class PigletSerialPort
{
  // Data

private:

  // Class administration variables
  int Active;
  int Error;

  // Com port direct handling
  int PortAddress;
  int LineStatusAddress;
  int ModemControlAddress;
  int InterruptEnableAddress;
  int InterruptIdentAddress;
  int LineControlAddress;
  int ModemStatusAddress;
  int FIFOControlAddress;

  int DLLB;
  int DLHB;


  unsigned char LineStatus;

  // Services

public:

  // Class Error Management
  void SignalError(int NewError);
  void ClearError();
  int  GetError();

  unsigned char GetLineStatus();

  // Actual Port Management
  unsigned char ReceiveByte();
  void          TransmitByte(unsigned char byte);

  int Receive(unsigned char * buffer, size_t bytes);
  int Transmit(unsigned char * buffer, size_t bytes);

  void FlushRx();

  inline void          RaiseRTS();
  inline void          LowerRTS();

  PigletSerialPort(int newPortAddress = 0x3F8);

  // Implementation

private:

  int InitializePort();

  // Enumeration
                                      
public:

};


#endif // (PIGLET_SERIALPORT_H)
