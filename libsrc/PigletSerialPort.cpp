/**
** Piglet Productions
**
** FileName       : PigletSerialPort.cpp
**
** Implements     : PigletSerialPort
**
** Description
**
** See PigletSerialPort.h for more details.
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

#include <PigletSerialPort.h>


/**
**    PigletSerialPort
**
**
**
**    Parameters
**
**    Returns
**
**/
PigletSerialPort::PigletSerialPort(int newPortAddress)
{
  PortAddress = newPortAddress;
  InterruptEnableAddress = PortAddress + 1;
  InterruptIdentAddress  = PortAddress + 2;
  LineControlAddress     = PortAddress + 3;
  ModemControlAddress    = PortAddress + 4;
  LineStatusAddress      = PortAddress + 5;
  ModemStatusAddress     = PortAddress + 6;
  FIFOControlAddress     = PortAddress + 2;

  DLLB = PortAddress;
  DLHB = PortAddress + 1;
  InitializePort();
}


/**
**    ReceiveByte
**
** This function checks the receive ready condition on the UART repeatedly
** until one second has elapsed or it is true.
**
** An error flag (PigletSerialError_TimeOut) will be set if no byte can
** be received.
**
**    Returns
**
**    Value of received byte if fetched, otherwise it returns 0.
**
**/
unsigned char PigletSerialPort::ReceiveByte()
{
  clock_t start = clock();

  do{
    LineStatus = (unsigned char) inp(LineStatusAddress);
  } while(!(LineStatus & 0x01) && ((((clock() - start) * 1000) / CLOCKS_PER_SEC) < 75));

  // Did we timeout?
  if(!(LineStatus & 0x01)){
    SignalError(PigletSerialError_TimeOut);
    return(0);
  }

  if(LineStatus & 0x8E){
    SignalError(PigletSerialError_RxError);
  }

  return((unsigned char) inp(PortAddress));
}


/**
**    TransmitByte
**
** This function transmit a single byte and then waits for one second, or
** an ack from the UART that the byte is transmitted. It will call an error
** PigletSerialError_TimeOut if it gets no ack.
**
**    Parameters
**
**    byte      byte to transmit
**
**/
void PigletSerialPort::TransmitByte(unsigned char byte)
{
  time_t start = time(NULL);

  outp(PortAddress, byte);

  do{
    LineStatus = (unsigned char) inp(LineStatusAddress);
  } while(!(LineStatus & 0x40) && ((time(NULL) - start) < 1));

  // Did we timeout?
  if(!(LineStatus & 0x40)){
    SignalError(PigletSerialError_TimeOut);
  }

  return;
}


/**
**    Receive
**
** Attempts to receive a number of bytes from the UART. This will quit
** prematurely if a timeout error occurs on any byte fetch.
**
**    Parameters
**
**    buffer      Region of memory into which the received bytes are inserted
**    bytes       Number of bytes to receive
**
**    Returns
**
**    zero indicates success, non zero indicates failure and buffer may
**    then not contain relevant data.
**
**/
int PigletSerialPort::Receive(unsigned char * buffer, size_t bytes)
{
  size_t position;
  int    LocalQuit  = 0;
  int    LocalError = 0;

  ClearError();
  for(position = 0; position < bytes && !LocalQuit; position++){
    buffer[position] = ReceiveByte();
    if(Error) LocalError = 1;
    if(Error == PigletSerialError_TimeOut){
      // Some sort of time out, did we receive anything?
      if(position == 0) SignalError(PigletSerialError_TotalTimeOut);
      LocalQuit = 1;
    }
  }

  return(LocalError);
}


/**
**    Transmit
**
**
**
**    Parameters
**
**    Returns
**
**/
int PigletSerialPort::Transmit(unsigned char * buffer, size_t bytes)
{
  size_t position;
  int    LocalQuit = 0;

  RaiseRTS();

  Delay(5);

  ClearError();
  for(position = 0; position < bytes && !LocalQuit; position++){
    TransmitByte(buffer[position]);
    if(Error == PigletSerialError_TimeOut) LocalQuit = 1;
  }

  Delay(1);

  LowerRTS();

  return(LocalQuit);
}


/**
**    FlushRx
**
**
**
**    Parameters
**
**    Returns
**
**/
void PigletSerialPort::FlushRx()
{
  int RxReady;

  do{
    LineStatus = (unsigned char) inp(LineStatusAddress);
    RxReady    = LineStatus & 0x01;

    // If there's a byte, read and discard
    if(RxReady) inp(PortAddress);

  } while(RxReady);
}


/**
**    InitializePort
**
**
**
**    Parameters
**
**    Returns
**
**/
int PigletSerialPort::InitializePort()
{
  int FIFOTest;

  // No interrupts enabled
  outp(InterruptEnableAddress, 0x00);

  // Set DLB high
  outp(LineControlAddress, 0x80);
  outp(DLHB, 0x00);
  outp(DLLB, 0x60); // 1200 baud
  // Set DLB low
  outp(LineControlAddress, 0x00);

  // Enable FIFOs
  outp(FIFOControlAddress, 0x01);
  outp(FIFOControlAddress, 0x0F);

  // 8N1 - 0x03
  // 8O1 - 0x0B
  // 8E1 - 0x1B
  outp(LineControlAddress, 0x03);
  LowerRTS();

  FIFOTest = inp(InterruptIdentAddress);

  return(FIFOTest & 0xC0);
}


/**
**    RaiseRTS
**
**
**
**    Parameters
**
**    Returns
**
**/
void PigletSerialPort::RaiseRTS()
{
  outp(ModemControlAddress, 0x02);
}


/**
**    LowerRTS
**
**
**
**    Parameters
**
**    Returns
**
**/
void PigletSerialPort::LowerRTS()
{
  outp(ModemControlAddress, 0x00);
}


void PigletSerialPort::SignalError(int newError)
{
  Error = newError;
}


void PigletSerialPort::ClearError()
{
  Error = 0;
}


int PigletSerialPort::GetError()
{
  return(Error);
}
