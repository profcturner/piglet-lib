/**
** Piglet Productions
**
** FileName       : PigletByteBuffer.h
**
** Defines        : PigletByteBuffer
**
** Description
**
** This class is a simple buffer for storage of byte values.
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

#ifndef PIGLET_BYTEBUFFER_H
#define PIGLET_BYTEBUFFER_H

class PigletByteBuffer;

class PigletByteBuffer
{
  // Data

private:

  int           Active;
  unsigned char * Buffer;
  size_t        BufferSize;
  size_t        ItemsInContainer;

  // Services

public:

                PigletByteBuffer(size_t NewBufferSize = 1024);
                ~PigletByteBuffer();

  int           SetBufferSize(size_t NewSize);
  size_t        GetBufferSize();

  unsigned char Peek();
  unsigned char Pop();

  int           Push(unsigned char NewByte);
  int           GetItemsInContainer();
  int           IsActive();

  void          Flush();


  // Implementation

private:

  // Enumeration

public:

}

#endif // (PIGLET_BYTEBUFFER_H)
