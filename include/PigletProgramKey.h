/**
** Piglet Productions
**
** FileName       : PigletProgramKey.h
**
** Defines        : PigletProgramKey
**
** Description
**
**
**
**
** Initial Coding : Colin Turner
**
** Date           : 16th May 1999
**
**
** Copyright applies on this file, and distribution may be limited.
*/

/*
** Revision 1.00
**
*/

#ifndef Piglet_ProgramKey_H
#define Piglet_ProgramKey_H

#include <time.h>
#include <

class PigletProgramKey
{
  // Data

private:

  char   name[101];
  char   serial[10];
  time_t Creation;
  time_t Expiry;

  // Services

public:

  PigletProgramKey(char * filename = NULL, char * program = NULL);

  char * GetName();
  char * GetSerial();
  time_t GetCreation();
  time_t GetExpiry();
  int    LoadFromFile(char * filename, char * program);

  char * SetName();
  char * SetSerial();
  time_t SetCreation();
  time_t SetExpiry();
  int    SaveToFile(char * filename);



struct KEYSTRUCT *FiletoSerial(char *filename, char *program, struct KEYSTRUCT *k);
int KeytoFile(char * ExpireString, char *name, char *N, unsigned char key[16], unsigned char check[16], unsigned char dur[16]);
char *ExtractSerialInit(unsigned char digest[16]);
long ExtractSerialNumber(unsigned char digest[16], unsigned char digest2[16]);
long ExtractDuration(unsigned char digest[16], int flag);

unsigned char *GenerateKey(char *N, char *PN, char *SI, long SerialNumber, int DurationDays, time_t timer);
unsigned char *GenerateCheck(unsigned char key[16], char *SI, long Serial32, time_t timer);
unsigned char *GenerateDuration(int DurationDays, time_t timer);
char *ReverseString(char *string);
char *IncChar(char *string, char increase);
long htol(char *string);
char *MD5to8(char *string);


  // Implementation

  // Enumeration
};


#endif
