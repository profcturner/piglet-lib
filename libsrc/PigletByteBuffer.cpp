/**
** Piglet Productions
**
** FileName       : PigletByteBuffer.cpp
**
** Implements     : PigletByteBuffer
**
** Description
**
** Implements a simple buffer for dealing with bytes.
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

#include <PigletByteBuffer.h>
  
/**
**    PigletByteBuffer
**
** Constructor for the Byte Buffer. Active will be set to a non zero value
** if the buffer is ready for use. This can be monitored from the public
** function IsActive()
**
**    Parameters
**
**    NewBufferSize     The size in bytes of the buffer to create
**
**/
PigletByteBuffer::PigletByteBuffer(size_t NewBufferSize)
{
  // Initialise the memory of the class
  memset(this, 0, sizeof(PigletByteBuffer));
  Buffer = NULL;

  SetBufferSize(NewBufferSize);
}


/**
**    ~PigletByteBuffer()
**
** Destructor which deallocates the buffer.
**
**/
PigletByteBuffer::~PigletByteBuffer()
{
  if(Buffer) delete(Buffer);
}


/**
**    SetBufferSize
**
** Destructively sizes the buffer, that is, any existing buffer will be
** destroyed and its data lost.
**
**    Parameters
**
**    Returns
**
**/
int PigletByteBuffer::SetBufferSize(size_t NewBufferSize;)
{
  // If a buffer exists already, we destroy it
  if(Buffer) delete Buffer;

  // Allocate the memory for the buffer
  if(NewBufferSize){
    Buffer = new unsigned char[NewBufferSize];
    if(Buffer) BufferSize = NewBufferSize;
    Active = 1;
  }
  else{
    Buffer = NULL;
    BufferSize = 0;
    Active = 0;
  }
  
  ItemsInContainer = 0;
}


/**
**    GetBufferSize
**
** Returns the size of the buffer in bytes.
**
**/
size_t PigletByteBuffer::GetBufferSize()
{
  return(BufferSize);
}


/**
**    IsActive
**
** Determines if the buffer object is ready for use.
**
**    Returns
**
**    zero if the buffer is inactive, non zero otherwise.
**
**/
int PigletByteBuffer::IsActive()
{
  return(IsActive);
}


/**
**    GetItemsInContainer
**
**
**
**    Parameters
**
**    Returns
**
**/
int PigletByteBuffer::GetItemsInContainer();
{
  return(ItemsInContainer);
}


/**
**    Push
**
**
**
**    Parameters
**
**    Returns
**
**/
int PigletByteBuffer::Push(unsigned char NewByte)
{
  if(ItemsInContainer >= BufferSize) return(0);

  ItemsInContainer++;
  BufferSize[ItemsInContainer - 1] = NewByte;

  return(1);
}


/**
**    Pop
**
**
**
**    Parameters
**
**    Returns
**
**/
unsigned char PigletByteBuffer::Pop()
{
  unsigned char Byte;

  if(!ItemsInContainer) return(0);

  Byte = Buffer[ItemsInContainer - 1]);
  ItemsInContainer--;

  return(Byte);
}


/**
**    Peek
**
**
**
**    Parameters
**
**    Returns
**
**/
unsigned char PigletByteBuffer::Peek()
{
  if(!ItemsInContainer) return(0);

  return(Buffer[ItemsInContainer - 1]);
}


/**
**    Flush
**
** Flushes the buffer. That is, it empties it of all contents.
**
**/
void PigletByteBuffer::Flush()
{
  ItemsInContainer = 0;
}
